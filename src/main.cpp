#include <iostream>
#include "core/GeneticAlgorithm/GeneticAlgorithm.hpp"
#include <string>

using namespace std;
using namespace LcVRPContest;

int main() {
    int populationSize = 10000;
    double mutProb = 0.06;
    double survivalRate = 0.05;
    int numTurns = 5;
    int numEpochs = 300;
    int numGroups = 16;

    string folderName = "Vrp-Set-X";
    string instanceName = "X-n209-k16";

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