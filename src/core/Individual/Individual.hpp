#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "../Evaluator/Evaluator.hpp"

using namespace std;

namespace LcVRPContest {
    class Individual {
        public:
            Individual(vector<int> newGenome, int newNumGroups, Evaluator& newEvaluator);
            Individual(const Individual &other);
            ~Individual();

            pair<Individual, Individual> crossover(const Individual &other, mt19937 &rng);
            void mutate(mt19937 &rng);
            
            vector<int>* getGenome();
            double getFitness() const;
            int getNumGroups() const;
            int getNumCustomers() const;
            bool isValid() const;
            
        private:
            static const double MUT_PROBABILITY = 0.01;

            Evaluator& evaluator;

            int numCustomers;
            int numGroups;
            vector<int> genome;
            double fitness;
            bool valid;
    };
}