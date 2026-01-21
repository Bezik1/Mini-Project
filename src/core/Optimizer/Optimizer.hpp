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
	 * @details Optimizer holds the methods of choosing mate for each individual and single iteration 
	 * runnning function.
	 * 
     * @param evaluator it's the reference to the evaluator object that measures fitness of individuals
     * @param newPopulation it's the pointer to the individual population table
     * @param newPrevPopulation it's the pointer to the previous individual population table
     * @param newPopSize specifies population size
     * @param newNumTurns specifies number of toruns used i. tournament selection function
     * @param newMutProb mutation probability
     * @param newCrossoverProb crossover probability
     * @param newCurrentBest it's the reference to variable that holds current best individual 
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
                double newCrossoverProb,
                Individual* newCurrentBest
            );
			~Optimizer();

			/**
			 * @brief Starts the GA iteration with population swapping, torunament selection,
			 * crossing and mutation.
			 * 
			 * @details To achieve single iteration this function firstly performs sort operation
			 * using `HeapSort<T>::sort(previousPopulation, popSize))`. Usage of heap sort allows
			 * program to completly get rid of allocation, while sorting and the sorting operation
			 * is done in place, so we did not create any additional copy of the table. This algorithm
			 * also sorts in efficient. and stable time of O(nlog(n)), so it is perfect match for this
			 * project. When it uses `std::swap` to just swap populations in time O(1). Next it starts 
			 * crossing and mutating population, from index equal to number of individuals in which 
			 * crossing didn't occur.
			 * 
			 * @return void
			 */
			void runIteration();

			/**
			 * @brief This function is used as a way to choose appropraite mate for choosen Individual.
			 * It randomly choose `numTurns` Individuals and saves the one with the best fitness as a
			 * potential partner. Finally after `numTurns` iterations function returns address of the 
			 * most suitable partner.
			 * 
			 * @return reference to choosen Individual
			 */
			Individual& tournamentSelection();

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
	};
}