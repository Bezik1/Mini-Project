#pragma once

#include "../Individual/Individual.hpp"
#include "../Evaluator/Evaluator.hpp"
#include "../Optimizer/Optimizer.hpp"

#include <vector>
#include <random>

using namespace std;

namespace LcVRPContest {
	class GeneticAlgorithm {
		public:
			GeneticAlgorithm(
				string folderName,
				string instnaceName,
				int newPopSize,
				int newNumTurns,
				double newMutProb,
				double newSurvivalRate,
				int numGroups,
				int newNumEpochs
			);
			~GeneticAlgorithm();

			void initialize();
			void runLoop();

		private:			
			Evaluator* evaluator;
			Optimizer* optimizer;
			
			int popSize;
			int numTurns;
			int numEpochs;
			double survivalRate;
			double mutProb;

			Individual* population;
			Individual* previousPopulation;
			Individual currentBest;

			mt19937 rng;

			void initRandomGenome(int* individual, int numCustomers);
	};
}