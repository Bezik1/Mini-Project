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
		~Optimizer();

		void Initialize();
		void RunIteration();

		Individual* tournamentSelection();

		vector<Individual*>* getPopulation();
		Individual* GetCurrentBest() { return currentBest; }
		double GetCurrentBestFitness() const { return currentBestFitness; }
		void PrintIndivual(vector<int>& individual, double fitness) const;

	private:
		static const int DEFAULT_POP_SIZE;
		static const int DEFAULT_TORUNAMENT_TURN;
		
		Evaluator& evaluator;
		
		int popSize;
		vector<Individual*> population;
		Individual* currentBest;
		double currentBestFitness;

		mt19937 rng;

		void InitRandomGenome(vector<int>& individual);
	};
}