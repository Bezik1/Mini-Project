#include "Optimizer.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace LcVRPContest;

const double Optimizer::DEFAULT_MUT_PROB = 0.1;
const int Optimizer::DEFAULT_POP_SIZE = 1000;
const int Optimizer::DEFAULT_NUM_TURNS = 10;

Optimizer::Optimizer(Evaluator& evaluator) 
	: evaluator(evaluator), 
	rng(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {
	
	currentBest = NULL;
	numTurns = DEFAULT_NUM_TURNS;
	mutProb = DEFAULT_MUT_PROB;
	popSize = DEFAULT_POP_SIZE;
}

Optimizer::Optimizer(Evaluator& evaluator, int newPopSize, int newNumTurns, double newMutProb) 
	: evaluator(evaluator), 
	rng(random_device{}()), 
	currentBestFitness(numeric_limits<double>::max()) {

	currentBest = NULL;
	numTurns = newNumTurns;
	mutProb = newMutProb;
	popSize = newPopSize;
}

Optimizer::~Optimizer() {
	for(int i=0; i<population.size(); i++)
		delete population[i];

	if(currentBest != NULL) delete currentBest;
}

void Optimizer::Initialize() {
	for(int i=0; i<population.size(); i++)
		delete population[i];
	population.clear();

	for(int i=0; i<popSize; i++) {
		vector<int> randomGenome(evaluator.GetSolutionSize());
        InitRandomGenome(randomGenome);

		population.push_back(new Individual(randomGenome, evaluator.GetNumGroups(), evaluator));
	}
}

void Optimizer::RunIteration() {
	vector<Individual*> nextGeneration;
    nextGeneration.reserve(popSize);

	while(nextGeneration.size() < popSize) {
		Individual* parentOne = tournamentSelection();
		Individual* parentTwo = tournamentSelection();

		pair<Individual, Individual> children = parentOne->crossover(parentTwo, rng);

		children.first.mutate(rng, mutProb);
		children.second.mutate(rng, mutProb);

		nextGeneration.push_back(new Individual(children.first));

		if(nextGeneration.size() < popSize)
			nextGeneration.push_back(new Individual(children.second));
	}

	for(int i=0; i<population.size(); i++)
		delete population[i];
	population = std::move(nextGeneration);

	for(int i=0; i<population.size(); i++) {
		Individual* individual = population[i];
		double fitness = individual->getFitness();

		if(fitness < currentBestFitness) {
			currentBestFitness = fitness;

			if (currentBest != NULL) delete currentBest;
			currentBest = new Individual(*individual);
		}
	}
}

Individual* Optimizer::tournamentSelection() {
    uniform_int_distribution<int> dist(0, population.size() - 1);
    Individual* best = population[dist(rng)];

    for (int i = 0; i < DEFAULT_NUM_TURNS; i++) {
        Individual* challenger = population[dist(rng)];
        if (challenger->getFitness() < best->getFitness()) {
            best = challenger;
        }
    }
    return best;
}

void Optimizer::InitRandomGenome(vector<int>& individual) {
	uniform_int_distribution<int> dist(0, evaluator.GetNumGroups() - 1); 
    for (size_t i = 0; i < individual.size(); ++i) {
        individual[i] = dist(rng);
    }
}

void Optimizer::PrintIndivual(vector<int>& individual, double fitness) const {
    cout << "Genome: [";
    for (size_t i = 0; i < individual.size(); ++i) {
        cout << individual[i] << (i < individual.size() - 1 ? " " : "");
    }
    cout << "]" << endl;

    vector<int> group_counts(evaluator.GetNumGroups(), 0);
    for (int group : individual) {
        group_counts[group]++;
    }

    cout << "Stats:   fitness: " << fixed << setprecision(2) << fitness 
			<< ", groups distribution: [";
    for (size_t i = 0; i < group_counts.size(); ++i) {
        cout << group_counts[i] << (i < group_counts.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
    cout << "-------------------------------------------------------" << endl;
}