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

    currentBest = Individual();
    
    evaluator = new Evaluator(numGroups);
    optimizer = new Optimizer(
        *evaluator,
        population,
        previousPopulation,
        popSize,
        numTurns,
        newMutProb,
        survivalRate
    );
}

GeneticAlgorithm::~GeneticAlgorithm() {
    delete[] population;
    delete[] previousPopulation;

    delete evaluator;
    delete optimizer;
}

void GeneticAlgorithm::Initialize() {
    optimizer->Initialize();
}

void GeneticAlgorithm::RunLoop() {
    for (int i = 0; i < numEpochs; i++) {
        optimizer->RunIteration();

        if (i % 2 == 0) {
            Individual best = optimizer->GetCurrentBest();
            double bestFitness = best.getFitness();
            cout << "Iteration " << i << " | Best fitness: " << bestFitness << endl;

            optimizer->PrintIndivual(best.getGenome(), best.getNumCustomers(), best.getFitness());
        }
    }
}

void GeneticAlgorithm::InitRandomGenome(int* individual, int numCustomers) {
    uniform_int_distribution<int> dist(0, evaluator->GetNumGroups() - 1); 
    for (int i = 0; i < numCustomers; ++i) {
        individual[i] = dist(rng);
    }
}