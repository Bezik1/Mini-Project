#include "Optimizer.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

const int Optimizer::DEFAULT_POP_SIZE = 1000;
const int Optimizer::DEFAULT_TORUNAMENT_TURN = 3;

Optimizer::Optimizer(Evaluator& evaluator) 
	: evaluator(evaluator), 
	rng(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {
	
	currentBest = NULL;
	popSize = DEFAULT_POP_SIZE;
}

Optimizer::Optimizer(Evaluator& evaluator, int newPopSize) 
	: evaluator(evaluator), 
	rng(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {
	
	currentBest = NULL;
	popSize = newPopSize;
}

void Optimizer::Initialize() {
	population.clear();

	for(int i=0; i<popSize; i++) {
		vector<int> randomGenome(evaluator.GetSolutionSize());
        InitRandomGenome(randomGenome);

		population[i] = new Individual(randomGenome, evaluator.GetNumGroups(), evaluator);
	}
}

void Optimizer::RunIteration() {
	vector<Individual*> nextGeneration;
    nextGeneration.reserve(popSize);

	while(nextGeneration.size() < popSize) {
		Individual* parentOne = tournamentSelection();
		Individual* parentTwo = tournamentSelection();

		pair<Individual, Individual> children = parentOne->crossover(parentTwo, rng);

		children.first.mutate(rng);
		children.second.mutate(rng);

		nextGeneration.push_back(&children.first);

		if(nextGeneration.size() < popSize)
			nextGeneration.push_back(&children.second);
	}

	population = std::move(nextGeneration);

	for(int i=0; i<population.size(); i++) {
		Individual* individual = population[i];
		double fitness = individual->getFitness();

		if(fitness > currentBestFitness) {
			currentBestFitness = fitness;
			currentBest = individual;
		}
	}
}

Individual* Optimizer::tournamentSelection() {
	return tournamentSelection(DEFAULT_TORUNAMENT_TURN, NULL);
}

Individual* Optimizer::tournamentSelection(int turnsLeft, Individual *currentParent) {
	if(turnsLeft == 0) return currentParent;

	uniform_int_distribution<int> dist(0, popSize - 1);
    Individual* challenger = population[dist(rng)];

	tournamentSelection(
		turnsLeft-1, 
		currentParent == NULL || challenger->getFitness() > currentParent->getFitness()
			? challenger
			: currentParent
	);
}

void Optimizer::InitRandomGenome(vector<int>& individual) {
	uniform_int_distribution<int> dist(evaluator.GetLowerBound(), evaluator.GetUpperBound());
	for (size_t i = 0; i < individual.size(); ++i) {
		individual[i] = dist(rng);
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