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
    double newSurvivalRate,
    Individual* newCurrentBest
)  : evaluator(evaluator), 
	rng(random_device{}()) {
    
    survivalRate = newSurvivalRate;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;

    population =  newPopulation;
    previousPopulation = newPrevPopulation;

    currentBest = newCurrentBest;
}

Optimizer::~Optimizer() {}

void Optimizer::runIteration() {
    std::swap(population, previousPopulation);

    int numSurvivors = static_cast<int>(survivalRate * popSize);

    for (int j = 0; j < numSurvivors; j++) {
        std::swap(population[j], previousPopulation[j]);
    }

    int i = numSurvivors; 
    while(i < popSize) {
        Individual &parentOne = tournamentSelection();
        Individual &parentTwo = tournamentSelection();

        if (i+1 < popSize) {
            parentOne.crossoverInPlace(parentTwo, population[i], population[i+1], rng);
            
            population[i].mutate(rng, mutProb);
            population[i+1].mutate(rng, mutProb);

            if(population[i].getFitness() < currentBest->getFitness())
                *currentBest = population[i]; 
            
            if(population[i+1].getFitness() < currentBest->getFitness())
                *currentBest = population[i+1]; 

            i += 2;
        } else {
            i++;
        }
    }
}

Individual& Optimizer::tournamentSelection() {
    uniform_int_distribution<int> dist(0, popSize - 1);
    Individual *best = &previousPopulation[dist(rng)];

    for (int i = 0; i < numTurns; i++) {
        Individual *challenger = &previousPopulation[dist(rng)];
        if (challenger->getFitness() < best->getFitness()) {
            best = challenger;
        }
    }
    return *best;
}

void Optimizer::initRandomGenome(int* individual, int numCustomers) {
    uniform_int_distribution<int> dist(0, evaluator.getNumGroups() - 1); 
    for (int i = 0; i < numCustomers; ++i) {
        individual[i] = dist(rng);
    }
}

void Optimizer::printGenome(const int* individual, int numCustomers) const {
	cout << "Genome: [";
    for (size_t i = 0; i < numCustomers; ++i) {
        cout << individual[i] << (i < numCustomers - 1 ? ", " : "");
    }
    cout << "]" << endl;
}

void Optimizer::printIndivual(const int* individual, int numCustomers, double fitness) const {
	printGenome(individual, numCustomers);

    vector<int> group_counts(evaluator.getNumGroups(), 0);
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