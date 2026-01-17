#pragma once

#include "../Individual/Individual.hpp"
#include "../Evaluator/Evaluator.hpp"

#include <vector>
#include <random>

using namespace std;

namespace LcVRPContest {
	class Optimizer {
		public:
			Optimizer(
                Evaluator& evaluator,
                Individual* newPopulation,
                Individual* newPrevPopulation,
                int newPopSize,
                int newNumTurns,
                double newMutProb,
                double newSurvivalRate
            );
			~Optimizer();

			void Initialize();
			void RunIteration();

			Individual tournamentSelection();

			vector<Individual*>* getPopulation();
			Individual GetCurrentBest() { return currentBest; }
			void PrintIndivual(int* individual, int numCustomers, double fitness) const;
			void PrintGenome(int* individual, int numCustomers) const;

		private:			
			Evaluator& evaluator;
			
			int popSize;
			int numTurns;
			double survivalRate;
			double mutProb;

			Individual* population;
			Individual* previousPopulation;
			Individual currentBest;

			mt19937 rng;

			void InitRandomGenome(int* individual, int numCustomers);
	};
}