#include <iostream>
#include "core/GeneticAlgorithm/GeneticAlgorithm.hpp"
#include <string>

using namespace std;
using namespace LcVRPContest;

int main() {
    int populationSize = 100000;
    double mutProb = 0.03;
    double crossoverProb = 0.85;
    int numTurns = 5;
    int numEpochs = 300;
    int numGroups = 2;

    string folderName = "Vrp-Set-P";
    string instanceName = "P-n19-k2";

    GeneticAlgorithm geneticAlgorithm(
        folderName,
        instanceName,
        populationSize,
        numTurns,
        mutProb,
        crossoverProb,
        numGroups,
        numEpochs
    );

    geneticAlgorithm.initialize();
    geneticAlgorithm.runLoop();

    return 0;
}