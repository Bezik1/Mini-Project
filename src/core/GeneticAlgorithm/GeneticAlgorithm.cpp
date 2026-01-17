#include "GeneticAlgorithm.hpp"
#include "../Individual/Individual.hpp"

#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

GeneticAlgorithm::GeneticAlgorithm(int newPopSize, int newNumTurns, double newMutProb, double newSurvivalRate, int numGroups, int newNumEpochs) 
    : rng(random_device{}()) {
    
    survivalRate = newSurvivalRate;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;
    numEpochs = newNumEpochs;

    population = new Individual[popSize];
    previousPopulation = new Individual[popSize];

    currentBest = population[0];
    
    evaluator = new Evaluator(numGroups);
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

void GeneticAlgorithm::Initialize() {
    int numCustomers = evaluator->GetSolutionSize();

    for(int i = 0; i < popSize; i++) {
        int* randomGenome = new int[numCustomers];
        InitRandomGenome(randomGenome, numCustomers);

        population[i] = Individual(randomGenome, evaluator->GetNumGroups(), *evaluator, numCustomers);
        previousPopulation[i] = population[i];
    }
}

void GeneticAlgorithm::RunLoop() {
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

void GeneticAlgorithm::InitRandomGenome(int* individual, int numCustomers) {
    uniform_int_distribution<int> dist(0, evaluator->GetNumGroups() - 1); 
    for (int i = 0; i < numCustomers; ++i) {
        individual[i] = dist(rng);
    }
}