#include "Individual.hpp"

using namespace LcVRPContest;

Individual::Individual(int newGenome[], int newNumGroups, Evaluator &newEvaluator, int genomSize) 
    : evaluator(&newEvaluator), numCustomers(genomSize), numGroups(newNumGroups) {
    
    genome = new int[numCustomers];
    for(int i = 0; i < numCustomers; i++) {
        genome[i] = newGenome[i];
    }
    fitness = evaluator->Evaluate(genome);
}

Individual::Individual(const Individual &other)
    : evaluator(other.evaluator), numCustomers(other.numCustomers), 
    numGroups(other.numGroups), fitness(other.fitness) {
    
    if (other.genome != nullptr) {
        genome = new int[numCustomers];
        for(int i = 0; i < numCustomers; i++) {
            genome[i] = other.genome[i];
        }
    } else {
        genome = nullptr;
    }
}

Individual& Individual::operator=(const Individual& other) {
    if (this != &other) {
        delete[] genome;
        
        evaluator = other.evaluator;
        numCustomers = other.numCustomers;
        numGroups = other.numGroups;
        fitness = other.fitness;
        
        if (other.genome != nullptr) {
            genome = new int[numCustomers];
            for(int i = 0; i < numCustomers; i++) {
                genome[i] = other.genome[i];
            }
        } else {
            genome = nullptr;
        }
    }
    return *this;
}

Individual::~Individual() {
    if(genome == NULL) return;
    
    delete[] genome;
}

pair<Individual, Individual> Individual::crossover(const Individual& other, mt19937 &rng) const {    
    uniform_int_distribution<int> dist(1, numCustomers - 1);
    int cutPoint = dist(rng);

    vector<int> childOneGenome(numCustomers);
    vector<int> childTwoGenome(numCustomers);

    for (int i = 0; i < numCustomers; i++) {
        if (i < cutPoint) {
            childOneGenome[i] = genome[i];
            childTwoGenome[i] = other.genome[i];
        } else {
            childOneGenome[i] = other.genome[i];
            childTwoGenome[i] = genome[i];
        }
    }

    return {
        Individual(childOneGenome.data(), numGroups, *evaluator, numCustomers),
        Individual(childTwoGenome.data(), numGroups, *evaluator, numCustomers)
    };
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
    fitness = evaluator->Evaluate(genome);
}

int* Individual::getGenome() const {
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