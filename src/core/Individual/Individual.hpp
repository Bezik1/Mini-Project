#pragma once

#include <iostream>
#include <vector>
#include <random>
#include "../Evaluator/Evaluator.hpp"

namespace LcVRPContest {
    class Individual {
        public:
            Individual() : evaluator(NULL), numCustomers(0), numGroups(0), genome(NULL), fitness(1e18) {}
            Individual(Individual&& other);

            Individual(int newGenome[], int newNumGroups, Evaluator& newEvaluator, int genomeSize);
            Individual(const Individual &other);
            ~Individual();

            Individual& operator=(Individual&& other);
            Individual& operator=(const Individual& other);

            bool operator>(const Individual& other) const;
            bool operator<(const Individual& other) const;

            void crossoverInPlace(
                const Individual& parent2,
                Individual& child1,
                Individual& child2,
                mt19937 &rng
            ) const;

            void mutate(mt19937 &rng, double mutProb);
            
            const int* getGenome() const;
            double getFitness() const;
            int getNumGroups() const;
            int getNumCustomers() const;

            void recalculateFitness();            
        private:
            Evaluator* evaluator;
            int* genome;
            int numCustomers;
            int numGroups;
            double fitness;
    };
}