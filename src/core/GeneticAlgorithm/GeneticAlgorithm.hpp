#pragma once

#include "../Individual/Individual.hpp"
#include "../Evaluator/Evaluator.hpp"
#include "../Optimizer/Optimizer.hpp"

#include <vector>
#include <random>

using namespace std;

namespace LcVRPContest {
	/**
	 * @brief Genetic Algorithm class acts as the orchesterator of all the modules. 
	 * It's the only class that assigns and handles dynamic memory.
	 *
	 * @details GA is the only class that assigns dynamic memory and manages it. It alocates 
	 * data using big raw arrays of int. 
	 *
	 * @param evaluator it's the pointer to the evaluator object that measures fitness of individuals
	 * @param optimizer it's the pointer to optimizer object, that runs single iteration of GA and calculate best genome

	 * @param popSize 	specifies population size
	 * @param numTurn 	specifies number of turns used in Optimizer::tournamentSelection to select best mate
	 * @param numEpochs number of epochs GA loop will be running for
	 * @param crossoverProb probability of crossover occurance, in case it didn't individuals are copied to next generation
	 * @param mutProb 	probability of mutation occurance

	 * @param folderName specifies folder location that points to datasets
	 * @param instnaceName specifies file name in the specific dataset

	 * @property population it's the pointer to table of Individuals that are used to optimize the best solution
	 * @property previousPopulation it's the pointer to table of Individuals that were used one iteration ago, 
		initially it's the same as population

	 * @property numCustomers 	it's number of places that are need to be visited by the truck
	 * @property genomes it's the pointer to share table of int, that stores genomes and previousGenomes
	 * in order to maximise locality of the variables.
	 * @property prevGenomes it's the pointer to previous generation geneome int table 

	 * @property currentBest it's the copy of the best Individual created up to this point by GA
	 * @property bestGenomeBuffer it's the memory buffer for the best individual
	
	 * @property rng it's the pointer to the random number generation objecg
	 */
	class GeneticAlgorithm {
		public:
			GeneticAlgorithm(
				string folderName,
				string instnaceName,
				int newPopSize,
				int newNumTurns,
				double newMutProb,
				double newCrossoverProb,
				int numGroups,
				int newNumEpochs
			);
			~GeneticAlgorithm();

			/**
			 * @brief Intialize individuals with random genoms, copy population genomes to previousPopulation 
			 * ones and assigns first individual as a currentBest.
			 * @return void
			 */
			void initialize();

			/**
			 * @brief Starts GA running loop that optimizies currentBest each time at a epoch.
			 * @return void
			 */
			void runLoop();

		private:			
			Evaluator* evaluator;
			Optimizer* optimizer;
			
			int popSize;
			int numTurns;
			int numEpochs;
			double crossoverProb;
			double mutProb;

			Individual* population;
			Individual* previousPopulation;

			int numCustomers;
			int* genomes;
			int* prevGenomes;

			Individual currentBest;
			int* bestGenomeBuffer;

			mt19937 rng;
			
			/**
			 * @brief initiates random genome for genome of length numCustomers with the
			 * values in the range from 1 to numGroups
			 * 
			 * @param individual it's the pointer to the Individual we want to modify
			 * @param numCustomers it's the legth of the genome table
			 * 
			 * @return void
			 */
			void initRandomGenome(int* individual, int numCustomers);
	};
}