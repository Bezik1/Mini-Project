#include <iostream>
#include "core/GeneticAlgorithm/GeneticAlgorithm.hpp"
#include <string>

using namespace std;
using namespace LcVRPContest;

int main() {
    int populationSize = 10000;
    double mutProb = 0.06;
    double crossoverProb = 0.05;
    int numTurns = 5;
    int numEpochs = 300;
    int numGroups = 16;

    string folderName = "Vrp-Set-X";
    string instanceName = "X-n209-k16";

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