#include <iostream>
#include <vector>
#include <random>

#include "Individual.hpp"

using namespace std;
using namespace LcVRPContest;

const double Individual::MUT_PROBABILITY = 0.1;


Individual::Individual(vector<int> newGenome, int newNumGroups, double newMutProb, Evaluator &newEvaluator) 
        : evaluator(newEvaluator),
        genome(std::move(newGenome)) {
    
    mutProb = newMutProb;
    numGroups = newNumGroups;
    numCustomers = genome.size();
    fitness = evaluator.Evaluate(genome);
    valid = fitness >= 0;
}

Individual::Individual(vector<int> newGenome, int newNumGroups, Evaluator &newEvaluator) 
        : evaluator(newEvaluator),
        genome(std::move(newGenome)) {
    
    mutProb = MUT_PROBABILITY;
    numGroups = newNumGroups;
    numCustomers = genome.size();
    fitness = evaluator.Evaluate(genome);
    valid = fitness >= 0;
}

Individual::Individual(const Individual &other)
    : evaluator(other.evaluator),
    genome(other.genome) {
    
    numCustomers = other.numCustomers;
    numGroups = other.numGroups;
    fitness = other.fitness;
    valid = other.valid;
}

Individual::~Individual() {};

pair<Individual, Individual> Individual::crossover(const Individual *other, mt19937 &rng) {
    if(!isValid() || !other->isValid()) {

    }
    
    uniform_int_distribution<int> dist(1, numCustomers - 1);
    int cutPoint = dist(rng);


    vector<int> childOneGenome, childTwoGenome;

    for (int i = 0; i < numCustomers; i++) {
        if (i < cutPoint) {
            childOneGenome.push_back(this->genome[i]);
            childTwoGenome.push_back(other->genome[i]);
        } else {
            childOneGenome.push_back(other->genome[i]);
            childTwoGenome.push_back(this->genome[i]);
        }
    }

    Individual childOne(childOneGenome, numGroups, evaluator);
    Individual childTwo(childTwoGenome, numGroups, evaluator);

    return { childOne, childTwo };
}

void Individual::mutate(mt19937 &rng) {
    uniform_real_distribution<double> dist(0.0, 1.0);
    uniform_int_distribution<int> genes(0, numGroups - 1);

    for(size_t i = 0; i < numCustomers; i++) {
        double mutProb = dist(rng);
        if(mutProb >= MUT_PROBABILITY) continue;

        genome[i] = genes(rng);
    }

    fitness = evaluator.Evaluate(genome);
    valid = fitness >= 0;
}

vector<int>* Individual::getGenome() {
    return &genome;
}

double Individual::getFitness() const {
    return valid ? fitness : 1e13;
}

int Individual::getNumGroups() const {
    return numGroups;
}

int Individual::getNumCustomers() const {
    return numCustomers;
}

bool Individual::isValid() const {
    return valid;
}