#include "../Evaluator/Evaluator.hpp"
#include "../ProblemData/ProblemData.hpp"
#include "../ProblemLoader/ProblemLoader.hpp"
#include <algorithm>
#include <numeric>
#include "iostream"

using namespace LcVRPContest;

Evaluator::Evaluator(int numGroups)
    : numGroups(numGroups) {
	ProblemLoader loader("Vrp-Set-X", "X-n209-k16");
	problemData = ProblemData(loader.LoadProblem());
	numCustomers = problemData.getNumCustomers();

}

double Evaluator::Evaluate(const vector<int>* solution) {
	if (!solution) {
		return WRONG_VAL;
	}
	return Evaluate(*solution);
}

double Evaluator::Evaluate(const vector<int>& solution) {
	if (solution.size() != static_cast<size_t>(getSolutionSize())) {
		return WRONG_VAL;
	}
	return Evaluate(solution.data());
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

	double total_cost = 0.0;
	for (const auto& route : routes) {
		double route_cost = calculateRouteCost(route);
		
		if (route_cost < 0.0) {
			return WRONG_VAL;
		}
			
		total_cost += route_cost;
	}

	return total_cost;
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
	int depot_index = depot - 1; //again, depot index in 0-based
	const vector<int>& demands = problemData.getDemands();
	int capacity = problemData.getCapacity();
	
	// check each customer
	for (int customer_id = 2; customer_id <= problemData.getDimension(); ++customer_id) {
		int customer_index = customer_id - 1;
		
		// check if any customer has demand > capacity
		if (demands[customer_index] > capacity) {
			return false;
		}

		// check if distance constraint exists
		if (problemData.hasDistanceConstraint()) {
			double max_distance = problemData.getDistance();
			double dist_to_depot = problemData.calculateDistance(depot_index, customer_index);
			
			if (dist_to_depot < 0.0) {
				return false;
			}
			
			// each customer must be reachable: depot -> customer -> depot <= max_distance
			// so: dist_to_depot * 2 <= max_distance
			if (dist_to_depot * 2.0 > max_distance) {
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
		int customer_id = permutation[i];
		// customer id start from 2 (depot is 1), so customer index = customer_id - 2
		int customer_index = customer_id - 2;
			
		if (customer_index >= 0 && customer_index < numCustomers) {
			int group = grouping[customer_index];
			routes[group].push_back(customer_id);
		}
	}
}

double Evaluator::calculateRouteCost(const vector<int>& route) const {
	if (route.empty()) {
		return 0.0;
	}

	double total_cost = 0.0;
	int depot = problemData.getDepot();
	int depot_index = depot - 1; // depot index in 0-based for easier calculations
	int capacity = problemData.getCapacity();
	double max_distance = problemData.hasDistanceConstraint() ? problemData.getDistance() : -1.0;
	const vector<int>& demands = problemData.getDemands();

	int current_load = 0;
	double current_distance = 0.0;
	int last_position_index = depot_index;

	for (size_t i = 0; i < route.size(); ++i) {
		int customer_id = route[i];
		int customer_index = customer_id - 1;
		int customer_demand = demands[customer_index];

		// we check if adding this new customer would exceed CAPACITY constraint
		if (current_load + customer_demand > capacity) {
			// return to depot, close current subtour
			double dist_to_depot = problemData.calculateDistance(last_position_index, depot_index);
			if (dist_to_depot < 0.0) return WRONG_VAL;
			
			current_distance += dist_to_depot;
			total_cost += current_distance;

			// reset for new subtour
			current_load = 0;
			current_distance = 0.0;
			last_position_index = depot_index;
		}

		// calculate distance to the next customer
		double dist_to_customer = problemData.calculateDistance(last_position_index, customer_index);
		if (dist_to_customer < 0.0) return WRONG_VAL;

		// check if adding this customer would exceed DISTANCE constraint
		if (max_distance > 0.0) {
			double dist_back_to_depot = problemData.calculateDistance(customer_index, depot_index);
			if (dist_back_to_depot < 0.0) return WRONG_VAL;

			// would this customer + return to depot exceed max_distance?
			if (current_distance + dist_to_customer + dist_back_to_depot > max_distance) {
				// close current subtour and start new one
				double dist_to_depot = problemData.calculateDistance(last_position_index, depot_index);
				if (dist_to_depot < 0.0) return WRONG_VAL;
				
				current_distance += dist_to_depot;
				total_cost += current_distance;

				// reset for new subtour starting from depot
				current_load = 0;
				current_distance = 0.0;
				last_position_index = depot_index;

				// recalculate distance to customer from depot
				dist_to_customer = problemData.calculateDistance(depot_index, customer_index);
				if (dist_to_customer < 0.0) return WRONG_VAL;
			}
		}

		// add customer to current subtour
		current_distance += dist_to_customer;
		current_load += customer_demand;
		last_position_index = customer_index;
	}

	// close final subtour and return to depot
	double dist_to_depot = problemData.calculateDistance(last_position_index, depot_index);
	if (dist_to_depot < 0.0) return WRONG_VAL;
	
	current_distance += dist_to_depot;
	total_cost += current_distance;

	return total_cost;
}
