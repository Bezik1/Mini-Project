#include "Individual.hpp"

using namespace LcVRPContest;

void LcVRPContest::swap(Individual& first, Individual& second) {
    using std::swap;

    swap(first.genome, second.genome);
    swap(first.fitness, second.fitness);
    swap(first.evaluator, second.evaluator);
    swap(first.numCustomers, second.numCustomers);
    swap(first.numGroups, second.numGroups);   
}

Individual::Individual(int* sharedGenomeSpace, int newNumGroups, Evaluator &newEvaluator, int genomeSize) 
    : evaluator(&newEvaluator), numCustomers(genomeSize), numGroups(newNumGroups), genome(sharedGenomeSpace) {
}

Individual::Individual(const Individual& other) 
    : evaluator(other.evaluator), genome(NULL), numCustomers(other.numCustomers), 
    numGroups(other.numGroups), fitness(other.fitness) {
}

Individual& Individual::operator=(const Individual& other) {
    if (this != &other) {
        evaluator = other.evaluator;
        numCustomers = other.numCustomers;
        numGroups = other.numGroups;
        fitness = other.fitness;
        
        if (this->genome == NULL) {
            this->genome = other.genome;
        }

        if (this->genome != NULL && other.genome != NULL && this->genome != other.genome) {
            for(int i = 0; i < numCustomers; i++) {
                this->genome[i] = other.genome[i];
            }
        }
    }
    return *this;
}

bool Individual::operator>(const Individual& other) const {
    return this->fitness > other.fitness;
}

bool Individual::operator<(const Individual& other) const {
    return this->fitness < other.fitness;
}

void Individual::crossoverInPlace(const Individual& parent2, Individual& child1, Individual& child2, mt19937 &rng) const {
    uniform_int_distribution<int> dist(1, numCustomers - 1);
    int cutPoint = dist(rng);

    for (int i = 0; i < numCustomers; i++) {
        child1.genome[i] = (i < cutPoint) ? this->genome[i] : parent2.genome[i];
        child2.genome[i] = (i < cutPoint) ? parent2.genome[i] : this->genome[i];
    }
}

void Individual::mutate(mt19937 &rng, double mutProb) {
    if (!genome || !evaluator) return;

    uniform_real_distribution<double> dist(0.0, 1.0);
    uniform_int_distribution<int> genes(0, numGroups - 1);

    for(int i = 0; i < numCustomers; i++) {
        if(dist(rng) < mutProb) {
            genome[i] = genes(rng);
        }
    }
}

void Individual::recalculateFitness() {
    if(genome && evaluator) {
        fitness = evaluator->evaluate(genome);
    }
}