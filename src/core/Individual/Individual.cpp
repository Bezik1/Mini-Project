#include "Individual.hpp"

using namespace LcVRPContest;

Individual::Individual(int newGenome[], int newNumGroups, Evaluator &newEvaluator, int genomSize) 
    : evaluator(&newEvaluator), numCustomers(genomSize), numGroups(newNumGroups) {
    
    genome = new int[numCustomers];
    for(int i = 0; i < numCustomers; i++) {
        genome[i] = newGenome[i];
    }
    fitness = evaluator->evaluate(genome);
}

Individual::Individual(Individual&& other)
    : evaluator(other.evaluator),
    numCustomers(other.numCustomers),
    numGroups(other.numGroups),
    genome(other.genome),
    fitness(other.fitness)
{
    other.genome = NULL;
    other.numCustomers = 0;
}

Individual& Individual::operator=(Individual&& other) {
    if (this != &other) {
        delete[] genome;

        evaluator = other.evaluator;
        numCustomers = other.numCustomers;
        numGroups = other.numGroups;
        genome = other.genome;
        fitness = other.fitness;

        other.genome = NULL;
        other.numCustomers = 0;
    }
    return *this;
}

Individual::Individual(const Individual& other) 
    : evaluator(other.evaluator), 
    numCustomers(other.numCustomers), 
    numGroups(other.numGroups), 
    fitness(other.fitness) {
    
    if (other.genome != nullptr) {
        genome = new int[numCustomers];
        for (int i = 0; i < numCustomers; i++) {
            genome[i] = other.genome[i];
        }
    } else {
        genome = nullptr;
    }
}

Individual& Individual::operator=(const Individual& other) {
    if (this != &other) {
        if(genome != NULL) delete[] genome;
        
        evaluator = other.evaluator;
        numCustomers = other.numCustomers;
        numGroups = other.numGroups;
        fitness = other.fitness;
        
        if (other.genome != NULL) {
            genome = new int[numCustomers];
            for(int i = 0; i < numCustomers; i++) {
                genome[i] = other.genome[i];
            }
        } else {
            genome = NULL;
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

Individual::~Individual() {
    if(genome != NULL) delete[] genome;
}

void Individual::crossoverInPlace(const Individual& parent2, Individual& child1, Individual& child2, mt19937 &rng) const {
    uniform_int_distribution<int> dist(1, numCustomers - 1);
    int cutPoint = dist(rng);

    for (int i = 0; i < numCustomers; i++) {
        child1.genome[i] = (i < cutPoint) ? this->genome[i] : parent2.genome[i];
        child2.genome[i] = (i < cutPoint) ? parent2.genome[i] : this->genome[i];
    }
    child1.fitness = evaluator->evaluate(child1.genome);
    child2.fitness = evaluator->evaluate(child2.genome);
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
    fitness = evaluator->evaluate(genome);
}

const int* Individual::getGenome() const {
    return genome;
}
double Individual::getFitness() const {
    return fitness;
}
int Individual::getNumGroups() const {
    return numGroups;
}
int Individual::getNumCustomers() const {
    return numCustomers;
}

void Individual::recalculateFitness() {
    if(genome && evaluator) {
        fitness = evaluator->evaluate(genome);
    }
}