#include <iostream>
#include "core/GeneticAlgorithm/GeneticAlgorithm.hpp"

using namespace std;
using namespace LcVRPContest;

int main() {
    int populationSize = 10000;
    double mutProb = 0.1;
    double survivalRate = 0.1;
    int numTurns = 10;
    int numEpochs = 4;
    int numGroups = 16;

    GeneticAlgorithm geneticAlgorithm(
        populationSize,
        numTurns,
        mutProb,
        survivalRate,
        numGroups,
        numEpochs
    );

    geneticAlgorithm.Initialize();
    geneticAlgorithm.RunLoop();

    return 0;
}