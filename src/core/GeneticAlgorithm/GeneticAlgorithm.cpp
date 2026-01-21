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
    double newCrossoverProb,
    int numGroups,
    int newNumEpochs
) 
    : rng(random_device{}()) {
    
    crossoverProb = newCrossoverProb;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;
    numEpochs = newNumEpochs;

    population = new Individual[popSize];
    previousPopulation = new Individual[popSize];
    
    evaluator = new Evaluator(folderName, instanceName, numGroups);

    numCustomers = evaluator->getSolutionSize();
    genomes = new int[popSize * numCustomers];
    prevGenomes = new int[popSize * numCustomers];

    for(int i = 0; i < popSize; i++) {
        population[i] = Individual(
            &genomes[i * numCustomers],
            numGroups,
            *evaluator,
            numCustomers
        );
        
        previousPopulation[i] = Individual(
            &prevGenomes[i * numCustomers],
            numGroups,
            *evaluator,
            numCustomers
        );
    }

    optimizer = new Optimizer(
        *evaluator,
        population,
        previousPopulation,
        popSize,
        numTurns,
        newMutProb,
        crossoverProb,
        &currentBest
    );

    bestGenomeBuffer = new int[numCustomers];
    currentBest = Individual(bestGenomeBuffer, numGroups, *evaluator, numCustomers);
}

GeneticAlgorithm::~GeneticAlgorithm() {
    delete[] genomes;
    delete[] prevGenomes;
    delete[] bestGenomeBuffer;

    delete[] population;
    delete[] previousPopulation;

    delete evaluator;
    delete optimizer;
}

void GeneticAlgorithm::initialize() {
    for(int i = 0; i < popSize; i++) {
        initRandomGenome(const_cast<int*>(population[i].getGenome()), numCustomers);

        previousPopulation[i].copyGenome(population[i].getGenome());

        population[i].recalculateFitness();
        previousPopulation[i].recalculateFitness();
    }
    currentBest.copyGenome(population[0].getGenome());
    currentBest.recalculateFitness();
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