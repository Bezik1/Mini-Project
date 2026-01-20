#pragma once

#include <iostream>
#include <vector>
#include <random>
#include "../Evaluator/Evaluator.hpp"

namespace LcVRPContest {
    class Individual {
        public:
            Individual() : evaluator(NULL), genome(NULL), numCustomers(0), numGroups(0), fitness(1e18) {}

            Individual(int* sharedGenomeSpace, int newNumGroups, Evaluator& newEvaluator, int genomeSize);

            ~Individual() {}

            friend void swap(Individual& first, Individual& second);

            void copyGenome(const int* otherGenome);

            Individual(const Individual& other);
            Individual& operator=(const Individual& other);

            void crossoverInPlace(const Individual& parent2, Individual& child1, Individual& child2, mt19937 &rng) const;
            void mutate(mt19937 &rng, double mutProb);
            void recalculateFitness();

            bool operator>(const Individual& other) const;
            bool operator<(const Individual& other) const;

            double getFitness() const { return fitness; }
            const int* getGenome() const { return genome; }
            int getNumCustomers() const { return numCustomers; }

        private:
            Evaluator* evaluator;
            int* genome;
            int numCustomers;
            int numGroups;
            double fitness;
    };
}