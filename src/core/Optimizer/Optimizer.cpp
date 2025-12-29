#include "Optimizer.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

const int Optimizer::DEFAULT_POP_SIZE = 1000;

Optimizer::Optimizer(Evaluator& evaluator) 
	: evaluator(evaluator), 
	rng_(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {
	
	currentBest = NULL;
	popSize = DEFAULT_POP_SIZE;
}

Optimizer::Optimizer(Evaluator& evaluator, int newPopSize) 
	: evaluator(evaluator), 
	rng_(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {
	
	currentBest = NULL;
	popSize = newPopSize;
}

void Optimizer::Initialize() {
	population.clear();

	for(int i=0; i<popSize; i++) {
		vector<int> randomGenome(evaluator.GetSolutionSize());
        InitRandomIndividual(randomGenome);

		population[i] = new Individual(randomGenome, evaluator.GetNumGroups(), evaluator);
	}
}

void Optimizer::RunIteration() {
	
	// vector<int> new_individual(evaluator.GetSolutionSize());
	// InitRandomIndividual(new_individual);
	// double new_fitness = evaluator.Evaluate(new_individual);
		
	// if (new_fitness < currentBestFitness) {
	// 	currentBest = new_individual;
	// 	currentBestFitness = new_fitness;
	// }

	// PrintIndivual(new_individual, new_fitness);
}

void Optimizer::InitRandomIndividual(vector<int>& individual) {
	uniform_int_distribution<int> dist(evaluator.GetLowerBound(), evaluator.GetUpperBound());
	for (size_t i = 0; i < individual.size(); ++i) {
		individual[i] = dist(rng_);
	}
}

void Optimizer::PrintIndivual(vector<int>& individual, double fitness) const {
	vector<int> group_counts(evaluator.GetNumGroups(), 0);
	for (int group : individual) {
		group_counts[group]++;
	}

	cout << "current fitness: " << fixed << setprecision(2) << fitness << ", groups: [";
	for (size_t i = 0; i < group_counts.size(); ++i) {
		cout << group_counts[i] << (i < group_counts.size() - 1 ? ", " : "");
	}
	cout << "]" << endl;
}