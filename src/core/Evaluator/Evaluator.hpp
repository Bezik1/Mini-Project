#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <vector>
#include <string>

using namespace std;

namespace LcVRPContest {
	class Evaluator {
	public:
		Evaluator(int numGroups, string folderName, string instanceName);
		
		double Evaluate(const vector<int>* solution);
		double Evaluate(const vector<int>& solution);
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
		
		const double WRONG_VAL = 1e18;

		double calculateRouteCost(const vector<int>& route) const;
		bool isValidSolution(const vector<int>& grouping) const;
		bool validateConstraints() const;
		void buildRoutes(const vector<int>& grouping, vector<vector<int>>& routes) const;
	};
}
