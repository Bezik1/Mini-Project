#pragma once

#include "../Individual/Individual.hpp"
#include "../Evaluator/Evaluator.hpp"

#include <vector>
#include <random>

using namespace std;

namespace LcVRPContest {
	/**
	 * @brief Optimizer holds the methods used in order to run single iteration of GA instance. 
	 * 
	 * @details
	 * 
	 * 
	 */
	class Optimizer {
		public:
			Optimizer(
                Evaluator& evaluator,
                Individual* newPopulation,
                Individual* newPrevPopulation,
                int newPopSize,
                int newNumTurns,
                double newMutProb,
                double newSurvivalRate,
                Individual* newCurrentBest
            );
			~Optimizer();

			void runIteration();

			Individual& tournamentSelection();

			vector<Individual*>* getPopulation();
			void printIndivual(const int* individual, int numCustomers, double fitness) const;
			void printGenome(const int* individual, int numCustomers) const;

		private:			
			Evaluator& evaluator;
			
			int popSize;
			int numTurns;
			double crossoverProb;
			double mutProb;

			Individual* population;
			Individual* previousPopulation;
			Individual* currentBest;

			mt19937 rng;

			void initRandomGenome(int* individual, int numCustomers);
	};
}