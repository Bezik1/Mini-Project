#include "../Evaluator/Evaluator.hpp"
#include "../ProblemData/ProblemData.hpp"
#include "../ProblemLoader/ProblemLoader.hpp"
#include <algorithm>
#include <numeric>
#include "iostream"

using namespace LcVRPContest;

Evaluator::Evaluator(int numGroups, string folderName, string instanceName)
    : numGroups(numGroups)
{
	ProblemLoader loader(folderName, instanceName);
	problemData = ProblemData(loader.loadProblem());
	numCustomers = problemData.getNumCustomers();

}

double Evaluator::Evaluate(const int* solution) {
	if (!validateConstraints()) {
		return WRONG_VAL;
	}

	// convert to vector<int> for easier processing
	vector<int> grouping(solution, solution + numCustomers);

	if (!isValidSolution(grouping)) {
		return WRONG_VAL;
	}

	vector<vector<int>> routes;
	buildRoutes(grouping, routes);

	double totalCost = 0.0;
	for (const auto& route : routes) {
		double route_cost = calculateRouteCost(route);
		
		if (route_cost < 0.0) {
			return WRONG_VAL;
		}
			
		totalCost += route_cost;
	}

	return totalCost;
}

bool Evaluator::isValidSolution(const vector<int>& grouping) const {
	// basic check if all grouping values are within bounds
	for (int group : grouping) {
		if (group < getLowerBound() || group > getUpperBound()) {
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
	for (int customerId = 2; customerId <= problemData.getDimension(); ++customerId) {
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

void Evaluator::buildRoutes(const vector<int>& grouping, vector<vector<int>>& routes) const {
	routes.clear();
	routes.resize(numGroups);

	const vector<int>& permutation = problemData.getPermutation();

	// distribute customers using permutation and grouping
	// permutation contains customer ids from 2 to dimension (1 is depo)
	// grouping contains customer index (0 to num_customers-1)
		
	for (size_t i = 0; i < permutation.size(); ++i) {
		int customerId = permutation[i];
		// customer id start from 2 (depot is 1), so customer index = customerId - 2
		int customerIndex = customerId - 2;
			
		if (customerIndex >= 0 && customerIndex < numCustomers) {
			int group = grouping[customerIndex];
			routes[group].push_back(customerId);
		}
	}
}

double Evaluator::calculateRouteCost(const vector<int>& route) const {
	if (route.empty()) {
		return 0.0;
	}

	double totalCost = 0.0;
	int depot = problemData.getDepot();
	int depotIndex = depot - 1; // depot index in 0-based for easier calculations
	int capacity = problemData.getCapacity();
	double maxDistance = problemData.hasDistanceConstraint() ? problemData.getDistance() : -1.0;
	const vector<int>& demands = problemData.getDemands();

	int currentLoad = 0;
	double currentDist = 0.0;
	int lastPositionIndex = depotIndex;

	for (size_t i = 0; i < route.size(); ++i) {
		int customerId = route[i];
		int customerIndex = customerId - 1;
		int customer_demand = demands[customerIndex];

		// we check if adding this new customer would exceed CAPACITY constraint
		if (currentLoad + customer_demand > capacity) {
			// return to depot, close current subtour
			double distToDepot = problemData.calculateDistance(lastPositionIndex, depotIndex);
			if (distToDepot < 0.0) return WRONG_VAL;
			
			currentDist += distToDepot;
			totalCost += currentDist;

			// reset for new subtour
			currentLoad = 0;
			currentDist = 0.0;
			lastPositionIndex = depotIndex;
		}

		// calculate distance to the next customer
		double distToCustomer = problemData.calculateDistance(lastPositionIndex, customerIndex);
		if (distToCustomer < 0.0) return WRONG_VAL;

		// check if adding this customer would exceed DISTANCE constraint
		if (maxDistance > 0.0) {
			double dist_back_to_depot = problemData.calculateDistance(customerIndex, depotIndex);
			if (dist_back_to_depot < 0.0) return WRONG_VAL;

			// would this customer + return to depot exceed maxDistance?
			if (currentDist + distToCustomer + dist_back_to_depot > maxDistance) {
				// close current subtour and start new one
				double distToDepot = problemData.calculateDistance(lastPositionIndex, depotIndex);
				if (distToDepot < 0.0) return WRONG_VAL;
				
				currentDist += distToDepot;
				totalCost += currentDist;

				// reset for new subtour starting from depot
				currentLoad = 0;
				currentDist = 0.0;
				lastPositionIndex = depotIndex;

				// recalculate distance to customer from depot
				distToCustomer = problemData.calculateDistance(depotIndex, customerIndex);
				if (distToCustomer < 0.0) return WRONG_VAL;
			}
		}

		// add customer to current subtour
		currentDist += distToCustomer;
		currentLoad += customer_demand;
		lastPositionIndex = customerIndex;
	}

	// close final subtour and return to depot
	double distToDepot = problemData.calculateDistance(lastPositionIndex, depotIndex);
	if (distToDepot < 0.0) return WRONG_VAL;
	
	currentDist += distToDepot;
	totalCost += currentDist;

	return totalCost;
}
