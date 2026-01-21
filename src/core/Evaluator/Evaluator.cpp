#include "../Evaluator/Evaluator.hpp"
#include "../ProblemData/ProblemData.hpp"
#include "../ProblemLoader/ProblemLoader.hpp"
#include <algorithm>
#include <numeric>
#include "iostream"

using namespace LcVRPContest;

Evaluator::Evaluator(const string folderName, const string instanceName, int numGroups)
    : numGroups(numGroups) {
    loadProblem(folderName, instanceName);

	routesBuffer = new int*[numGroups];
    routeSizes = new int[numGroups];

    for (int i = 0; i < numGroups; ++i) {
        routesBuffer[i] = new int[numCustomers];
        routeSizes[i] = 0;
    }
}

void Evaluator::loadProblem(string folderName, string instanceName) {
    ProblemLoader loader(folderName, instanceName);
	problemData = ProblemData(loader.loadProblem());
	numCustomers = problemData.getNumCustomers();
}

Evaluator::~Evaluator() {
for (int i = 0; i < numGroups; ++i) {
        delete[] routesBuffer[i];
    }
    delete[] routesBuffer;
    delete[] routeSizes;
}

double Evaluator::evaluate(const int* solution) {
    if (!validateConstraints() || !isValidSolution(solution)) return WRONG_VAL;

    buildRoutes(solution);

    double totalCost = 0.0;
    for (int i = 0; i < numGroups; ++i) {
        if (routeSizes[i] > 0) {
            double routeCost = calculateRouteCost(routesBuffer[i], routeSizes[i]);
            if (routeCost < 0.0) return WRONG_VAL;
            totalCost += routeCost;
        }
    }
    return totalCost;
}

bool Evaluator::isValidSolution(const int* solution) const {
    for (int i = 0; i < numCustomers; ++i) {
        if (solution[i] < getLowerBound() || solution[i] > getUpperBound()) {
            return false;
        }
    }
    return true;
}

bool Evaluator::validateConstraints() const {
	int depot = problemData.getDepot();
	int depotIndex = depot - 1; //again, depot index in 0-based
	const vector<int>& demands = problemData.getDemands();
	int capacity = problemData.getCapacity();
	
	// check each customer
	for (int customerId = 2; customerId <= problemData.getDimension(); customerId++) {
		int customerIndex = customerId - 1;
		
		// check if any customer has demand > capacity
		if (demands[customerIndex] > capacity) {
			return false;
		}

		// check if distance constraint exists
		if (problemData.hasDistanceConstraint()) {
			double maxDistance = problemData.getDistance();
			double distToDepot = problemData.calculateDistance(depotIndex, customerIndex);
			
			if (distToDepot < 0.0) {
				return false;
			}
			
			// each customer must be reachable: depot -> customer -> depot <= maxDistance
			// so: distToDepot * 2 <= maxDistance
			if (distToDepot * 2.0 > maxDistance) {
				return false;
			}
		}
	}
	
	return true;
}

void Evaluator::buildRoutes(const int* solution) const {
    for (int i = 0; i < numGroups; ++i) {
        routeSizes[i] = 0;
    }

    const vector<int>& permutation = problemData.getPermutation();

    for (size_t i = 0; i < permutation.size(); ++i) {
        int customerId = permutation[i];
        int customerIndex = customerId - 2;
        
        int group = solution[customerIndex];
        
        int pos = routeSizes[group];
        routesBuffer[group][pos] = customerId;
        routeSizes[group]++;
    }
}

double Evaluator::calculateRouteCost(const int* route, int size) const {
    double totalCost = 0.0;
    int depotIdx = problemData.getDepot() - 1;
    int capacity = problemData.getCapacity();
    const vector<int>& demands = problemData.getDemands();
    double maxDist = problemData.hasDistanceConstraint() ? problemData.getDistance() : -1.0;

    int currentLoad = 0;
    double currentDist = 0.0;
    int lastPos = depotIdx;

    for (int i = 0; i < size; ++i) {
        int customerId = route[i];
        int customerIdx = customerId - 1;
        int demand = demands[customerIdx];
        double dNext = problemData.calculateDistance(lastPos, customerIdx);

        bool capEx = (currentLoad + demand > capacity);
        bool distEx = false;
        
        if (maxDist > 0.0) {
            double dBack = problemData.calculateDistance(customerIdx, depotIdx);
            if (currentDist + dNext + dBack > maxDist) distEx = true;
        }

        if (capEx || distEx) {
            totalCost += (currentDist + problemData.calculateDistance(lastPos, depotIdx));
            currentLoad = 0;
            currentDist = 0.0;
            lastPos = depotIdx;
            dNext = problemData.calculateDistance(depotIdx, customerIdx);
        }

        currentDist += dNext;
        currentLoad += demand;
        lastPos = customerIdx;
    }

    totalCost += (currentDist + problemData.calculateDistance(lastPos, depotIdx));
    return totalCost;
}