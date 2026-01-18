#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <vector>
#include <string>

using namespace std;

namespace LcVRPContest {
	class Evaluator {
	public:
		Evaluator(const string folderName, const string instanceName, int numGroups);
		~Evaluator();
		
		double Evaluate(const int* solution);

		int getSolutionSize() const { return numCustomers; }
		int getLowerBound() const { return 0; } 
		int getUpperBound() const { return numGroups - 1; }
		int getNumGroups() const { return numGroups; }

	private:
		string folderName;
		string instanceName;
		ProblemData problemData;

		int numGroups;
		int numCustomers;

		mutable int** routesBuffer;
		mutable int* routeSizes;
		
		const double WRONG_VAL = 1e18;

		double calculateRouteCost(const int* route, int size) const;
		bool isValidSolution(const int* grouping) const;
		bool validateConstraints() const;
		void buildRoutes(const int* grouping) const;
	};
}
