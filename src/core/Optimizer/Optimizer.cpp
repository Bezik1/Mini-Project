#include "Optimizer.hpp"
#include "../Individual/Individual.hpp"

#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

Optimizer::Optimizer(
    Evaluator& evaluator,
    Individual* newPopulation,
    Individual* newPrevPopulation,
    int newPopSize,
    int newNumTurns,
    double newMutProb,
    double newSurvivalRate
)  : evaluator(evaluator), 
	rng(random_device{}()) {
    
    survivalRate = newSurvivalRate;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;

    population =  newPopulation;
    previousPopulation = newPrevPopulation;

    currentBest = Individual();
}

Optimizer::~Optimizer() {
    delete[] population;
    delete[] previousPopulation;
}

void Optimizer::Initialize() {
    int numCustomers = evaluator.GetSolutionSize();

    for(int i = 0; i < popSize; i++) {
        vector<int> randomGenome(numCustomers);
        InitRandomGenome(randomGenome.data(), numCustomers);

        population[i] = Individual(randomGenome.data(), evaluator.GetNumGroups(), evaluator, numCustomers);
    }
}

void Optimizer::RunIteration() {
    int numSurvivors = static_cast<int>(survivalRate * popSize);

    for(int i = 0; i < popSize; i++) {
        previousPopulation[i] = population[i];
    }

    int i = numSurvivors; 
    while(i < popSize) {
        Individual parentOne = tournamentSelection();
        Individual parentTwo = tournamentSelection();

        pair<Individual, Individual> children = parentOne.crossover(parentTwo, rng);

        children.first.mutate(rng, mutProb);
        children.second.mutate(rng, mutProb);

        population[i] = children.first;
        i++;

        if(i < popSize) {
            population[i] = children.second;
            i++;
        }
    }

    for(int j = 0; j < popSize; j++) {
        double fitness = population[j].getFitness();
        if(fitness < currentBest.getFitness()) {
            currentBest = population[j];
        }
    }
}

Individual Optimizer::tournamentSelection() {
    uniform_int_distribution<int> dist(0, popSize - 1);
    Individual best = population[dist(rng)];

    for (int i = 0; i < numTurns; i++) {
        Individual challenger = population[dist(rng)];
        if (challenger.getFitness() < best.getFitness()) {
            best = challenger;
        }
    }
    return best;
}

void Optimizer::InitRandomGenome(int* individual, int numCustomers) {
    uniform_int_distribution<int> dist(0, evaluator.GetNumGroups() - 1); 
    for (int i = 0; i < numCustomers; ++i) {
        individual[i] = dist(rng);
    }
}

void Optimizer::PrintGenome(int* individual, int numCustomers) const {
	cout << "Genome: [";
    for (size_t i = 0; i < numCustomers; ++i) {
        cout << individual[i] << (i < numCustomers - 1 ? ", " : "");
    }
    cout << "]" << endl;
}

void Optimizer::PrintIndivual(int* individual, int numCustomers, double fitness) const {
	PrintGenome(individual, numCustomers);

    vector<int> group_counts(evaluator.GetNumGroups(), 0);
    for (int i=0; i<numCustomers; i++) {
        int group = individual[i];
        group_counts[group]++;
    }

    cout << "Stats:   fitness: " << fixed << setprecision(5) << fitness 
			<< ", groups distribution: [";
    for (size_t i = 0; i < group_counts.size(); ++i) {
        cout << group_counts[i] << (i < group_counts.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
    cout << "-------------------------------------------------------" << endl;
}