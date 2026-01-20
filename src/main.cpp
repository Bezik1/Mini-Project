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
    int numEpochs = 600;
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