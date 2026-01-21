#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <vector>
#include <string>

using namespace std;

namespace LcVRPContest {
	/**
	 * @brief Evaluator class is used to measure the fitness of Individual solution. It measures
	 * how good it is, by calculating the cost of following the path, by the truck using it's genome
	 * as a vertices map.
	 * 
	 * @details Evaluator main function is evaluate function, that returns calculated fitness. It uses
	 * other methods to achieve that. If the solution is invalid, then Evaluator set it's fitness to
	 * `WRONG_VAL` value, which is just really big number to mark big error numerically. Next evaluate
	 * method builds routes based on data stored inside ProblemData object and then Evaluator calculates
	 * the cost of traversing between every route till the end.
	 * 
	 * @param folderName specifies the folder in which dataset is stored
	 * @param instanceName specifies exact file in dataset
	 * @param numGroups it's the number of truck group and also the value range of individual gen in genome
	 * 
	 * @property routesBuffer it's pointer to pointer to int values that acts as a buffer for calclating routes
	 * it's opearting on static memory and does not allocate any additional memory on heap
	 * @property routeSizes stores sizes of routes stored in routesBuffer, also acts as a buffer
	 * 
	 */
	class Evaluator {
	public:
		Evaluator(const string folderName, const string instanceName, int numGroups);
		~Evaluator();
		
		double evaluate(const int* solution);

		int getSolutionSize() const { return numCustomers; }
		int getLowerBound() const { return 0; } 
		int getUpperBound() const { return numGroups - 1; }
		int getNumGroups() const { return numGroups; }

	private:
		ProblemData problemData;

		int numGroups;
		int numCustomers;

		mutable int** routesBuffer;
		mutable int* routeSizes;
		
		const double WRONG_VAL = 1e18;

		/**
         * @brief Calculates the total cost of a single route, considering distance and capacity constraints.
		 * 
         * @details Iterates through customers in a route, checking if adding a customer violates 
         * vehicle capacity or maximum distance constraints. If a violation occurs, the truck 
         * returns to the depot, and a new sub-route starts.
		 * 
         * @param route pointer to an array of customer IDs representing the sequence of visits.
         * @param size the number of customers in the given route.
		 * 
         * @return double calculated total distance (cost) of the route, or negative value if invalid.
         */
        double calculateRouteCost(const int* route, int size) const;

        /**
         * @brief Validates if the provided solution solution contains valid group indices.
		 * 
         * @details Checks if every gene in the genome falls within the allowed range 
         * [lowerBound, upperBound]. This prevents out-of-bounds access to the routesBuffer.
		 * 
         * @param solution pointer to the array of integers representing the group assignments.
		 * 
         * @return bool true if all group indices are valid, false otherwise.
         */
        bool isValidSolution(const int* solution) const;

        /**
         * @brief Validates global problem constraints independent of a specific solution.
		 * 
         * @details Checks if the problem is solvable by verifying if individual customer 
         * demands do not exceed truck capacity and if every customer is reachable from 
         * the depot within the maximum distance limit.
		 * 
         * * @return bool true if the problem constraints are satisfied, false if the instance is unsolvable.
         */
        bool validateConstraints() const;

        /**
         * @brief Distributes customers into specific routes based on the provided solution.
		 * 
         * @details Translates the flat genome (solution) into a structured 2D buffer (`routesBuffer`).
         * It resets the `routeSizes` and assigns each customer from the problem permutation 
         * to its respective truck group.
		 * 
         * @param solution pointer to the array of integers defining which group each customer belongs to.
		 * 
         * @return void
         */
        void buildRoutes(const int* solution) const;

        /**
         * @brief Initializes the evaluator by loading problem data from a file.
		 * 
         * @details Uses the ProblemLoader to fetch instance details such as dimensions, 
         * capacities, and coordinates. This data is then stored in the internal ProblemData object.
		 * 
         * @param folderName name of the directory containing the dataset.
         * @param instanceName specific filename of the problem instance.
		 * 
         * @return void
         */
        void loadProblem(
            string folderName,
            string instanceName
        );
	};
}
