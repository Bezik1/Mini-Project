#include <iostream>
#include "core/Optimizer/Optimizer.hpp"
#include "core/ProblemLoader/ProblemLoader.hpp"
#include "core/Evaluator/Evaluator.hpp"

using namespace std;
using namespace LcVRPContest;

int main() {
    int populationSize = 10000;
    double mutProb = 0.1;
    int numTurns = 5;
    int numEpochs = 200;

    int numGroups = 6;

    ProblemLoader loader("Vrp-Set-A", "A-n33-k6");
    ProblemData data = loader.LoadProblem();

    cout << "Loaded problem: " << data.GetName() << endl;
    cout << "Number of clients: " << data.GetNumCustomers() << endl;

    Evaluator evaluator(data, numGroups);
    Optimizer optimizer(evaluator, populationSize, numTurns, mutProb);
    
    cout << "\n=== INITALIZE OPTIMIZATION ===" << endl;
    optimizer.Initialize();

    cout << "Starting optimization (Max " << numEpochs << " iterations)..." << endl;

    for (int i = 0; i < numEpochs; ++i) {
        optimizer.RunIteration();

        if (i % 50 == 0 || i < 50) {
            double bestFitness = optimizer.GetCurrentBestFitness();
            cout << "Iteration " << i << " | Best fitness: " << bestFitness << endl;

                Individual* best = optimizer.GetCurrentBest();
                if (best != NULL) {
                    optimizer.PrintIndivual(*(best->getGenome()), best->getFitness());
                }
        }
    }

    return 0;
}