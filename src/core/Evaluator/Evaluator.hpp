#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <vector>

using namespace std;

namespace LcVRPContest {
	class Evaluator {
	public:
		Evaluator(int numGroups);
		
		double Evaluate(const vector<int>* solution);
		double Evaluate(const vector<int>& solution);
		double Evaluate(const int* solution);

		int GetSolutionSize() const { return num_customers_; }
		int GetLowerBound() const { return 0; } 
		int GetUpperBound() const { return numGroups_ - 1; }
		int GetNumGroups() const { return numGroups_; }

	private:
		ProblemData problemData;
		int numGroups_;
		int num_customers_;
		
		const double WRONG_VAL = 100000.0;

		// helper methods
		double CalculateRouteCost(const vector<int>& route) const;
		bool IsValidSolution(const vector<int>& grouping) const;
		bool ValidateConstraints() const;
		void BuildRoutes(const vector<int>& grouping, vector<vector<int>>& routes) const;
	};
}
