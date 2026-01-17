#include "GeneticAlgorithm.hpp"
#include "../Individual/Individual.hpp"

#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

GeneticAlgorithm::GeneticAlgorithm(
    string folderName,
    string instanceName,
    int newPopSize,
    int newNumTurns,
    double newMutProb,
    double newSurvivalRate,
    int numGroups,
    int newNumEpochs
) 
    : rng(random_device{}()) {
    
    survivalRate = newSurvivalRate;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;
    numEpochs = newNumEpochs;

    population = new Individual[popSize];
    previousPopulation = new Individual[popSize];

    currentBest = Individual();
    
    evaluator = new Evaluator(numGroups, folderName, instanceName);
    optimizer = new Optimizer(
        *evaluator,
        population,
        previousPopulation,
        popSize,
        numTurns,
        newMutProb,
        survivalRate,
        &currentBest
    );
}

GeneticAlgorithm::~GeneticAlgorithm() {
    delete[] population;
    delete[] previousPopulation;

    delete evaluator;
    delete optimizer;
}

void GeneticAlgorithm::initialize() {
    int numCustomers = evaluator->getSolutionSize();

    for(int i = 0; i < popSize; i++) {
        int* randomGenome = new int[numCustomers];
        initRandomGenome(randomGenome, numCustomers);

        population[i] = Individual(randomGenome, evaluator->getNumGroups(), *evaluator, numCustomers);
        previousPopulation[i] = Individual(randomGenome, evaluator->getNumGroups(), *evaluator, numCustomers);

        delete[] randomGenome;
    }
    currentBest = population[0];
}

void GeneticAlgorithm::runLoop() {
    for (int i = 0; i < numEpochs; i++) {
        optimizer->runIteration();

        if (i % 2 == 0) {
            double bestFitness = currentBest.getFitness();
            cout << "Iteration " << i << " | Best fitness: " << bestFitness << endl;

            optimizer->printIndivual(
                currentBest.getGenome(),
                currentBest.getNumCustomers(),
                currentBest.getFitness()
            );
        }
    }
}

void GeneticAlgorithm::initRandomGenome(int* individual, int numCustomers) {
    uniform_int_distribution<int> dist(0, evaluator->getNumGroups() - 1); 
    for (int i = 0; i < numCustomers; ++i) {
        individual[i] = dist(rng);
    }
}