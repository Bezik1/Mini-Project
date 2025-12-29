#pragma once

#include "../Individual/Individual.hpp"
#include "../Evaluator/Evaluator.hpp"

#include <vector>
#include <random>

using namespace std;

namespace LcVRPContest {
	class Optimizer {
	public:
		Optimizer(Evaluator& evaluator);
		Optimizer(Evaluator& evaluator, int newPopSize);

		void Initialize();
		void RunIteration();

		vector<Individual*>* getPopulation();
		Individual* GetCurrentBest() { return currentBest; }
		double GetCurrentBestFitness() const { return currentBestFitness; }

	private:
		static const int DEFAULT_POP_SIZE;
		
		Evaluator& evaluator;
		
		int popSize;
		vector<Individual*> population;
		Individual* currentBest;
		double currentBestFitness;

		mt19937 rng_;

		void InitRandomIndividual(vector<int>& individual);
		void PrintIndivual(vector<int>& individual, double fitness) const;
	};
}