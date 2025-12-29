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
		Optimizer(Evaluator& evaluator, int newPopSize, int newNumTurns, double newMutProb);
		~Optimizer();

		void Initialize();
		void RunIteration();

		Individual* tournamentSelection();

		vector<Individual*>* getPopulation();
		Individual* GetCurrentBest() { return currentBest; }
		double GetCurrentBestFitness() const { return currentBestFitness; }
		void PrintIndivual(vector<int>& individual, double fitness) const;
		void PrintGenome(vector<int>& individual) const;

	private:
		static const double DEFAULT_MUT_PROB;
		static const int DEFAULT_POP_SIZE;
		static const int DEFAULT_NUM_TURNS;
		
		Evaluator& evaluator;
		
		int popSize;
		int numTurns;
		vector<Individual*> population;
		Individual* currentBest;
		double currentBestFitness;
		double mutProb;

		mt19937 rng;

		void InitRandomGenome(vector<int>& individual);
	};
}