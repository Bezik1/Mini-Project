#pragma once

#include <iostream>
#include <vector>
#include <random>
#include "../Evaluator/Evaluator.hpp"

namespace LcVRPContest {
    /**
     * @brief Individual it's the class that represents individual solution for the LcVRP problem.
     * 
     * @details Individual stores information about it's genome as raw int* pointer. This class has the methods
     * for crossing with other indivdual, mutation this instance ob object, of copying other genome and to recalculate
     * fitness.
     * 
     * @property evaluator it's the pointer to the evaluator object that measures fitness of individuals
     * @property genome represents individual solution of the individual
     * @property numCustomers it's number of places that are need to be visited by the truck
     * @property numGroups it's the numbers of possible values that individual value in the genome may take
     * @property fitness it's the measure of how good is the solution. Smaller the fitness implies better solution
     */
    class Individual {
        public:
            Individual() : evaluator(NULL), genome(NULL), numCustomers(0), numGroups(0), fitness(1e18) {}

            Individual(int* sharedGenomeSpace, int newNumGroups, Evaluator& newEvaluator, int genomeSize);

            ~Individual() {}

            /**
             * @brief It's the friend function of Individual. It was created in order to
             * override default behaviour of `std::swap`.
             * 
             * @details Basic behaviour of `std::swap` incorrectly swaps the addresses of the
             * genome table, because of how it is implemented (the reference to the memory is taken
             * from the int* genomes table) so this function fixes this behaviour, by swaping genomes
             * manually.
             * 
             * @param first address of one individual
             * @param second address of other individual
             * 
             * @return void
             */
            friend void swap(Individual& first, Individual& second);


            /**
             * @brief Detailed copy the genome of other individual value by value
             * @param otherGenome it reads the const genome of other Individual
             * 
             * @return void
             */
            void copyGenome(const int* otherGenome);

            Individual(const Individual& other);
            Individual& operator = (const Individual& other);

            /**
             * @brief This function crosses the individuals in place, so there is no memory additionally
             * copied, in comparison to implementing this function to return the value. It natively generate
             * children in the given table.
             * 
             * @param otherParent reference to the address, where other individual for crossing is stored
             * @param childOne reference to the address, where first child will be stored
             * @param childTwo reference to the address, where second child will be stored
             * @param rng random number generator object address to generate random gene value
             * 
             * @return void
             */
            void crossover(const Individual& otherParent, Individual& childOne, Individual& childTwo, mt19937 &rng) const;

            /**
             * @brief This function mutates `this` individual if generated chance is smaller, than mutation 
             * probability. Each gene is being processed by the checking if mutation chance for current gene is
             * less, than `mutProb` and mutates it in case of true.
             * 
             * @param mutProb probability of occurance of mutation
             * @param rng random number generator reference, used in order to generate current chance for mutation
             * 
             * @return void
             * 
             */
            void mutate(mt19937 &rng, double mutProb);

            /**
             * @brief This function recalculates fitness of individual based on current modified genome
             * using evaluator.
             * @return void
             */
            void recalculateFitness();

            /**
             * @brief Compare Individual, specificly this operator uses the fitness of current
             * stored genome.
             * 
             * @param other holds reference to other object that will be compared
             * 
             * @return Boolean information about wheter individual is greater than the other
             */
            bool operator > (const Individual& other) const;

            /**
             * @brief Compare Individual, specificly this operator uses the fitness of current
             * stored genome.
             * 
             * @param other holds reference to other object that will be compared
             * 
             * @return Boolean information about wheter individual is smaller than the other
             */
            
            bool operator < (const Individual& other) const;

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