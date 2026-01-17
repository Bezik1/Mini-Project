#pragma once

#include <iostream>
#include <vector>
#include <random>
#include "../Evaluator/Evaluator.hpp"

namespace LcVRPContest {
    class Individual {
        public:
            Individual() : evaluator(NULL), numCustomers(0), numGroups(0), genome(NULL), fitness(1e18) {}

            Individual(int newGenome[], int newNumGroups, Evaluator& newEvaluator, int genomeSize);
            Individual(const Individual &other);
            ~Individual();

            Individual& operator=(const Individual& other);

            pair<Individual, Individual> crossover(const Individual& other, mt19937 &rng) const;
            void mutate(mt19937 &rng, double mutProb);
            
            int* getGenome() const;
            double getFitness() const;
            int getNumGroups() const;
            int getNumCustomers() const;
            
        private:
            Evaluator* evaluator;
            int numCustomers;
            int numGroups;
            int* genome;
            double fitness;
    };
}