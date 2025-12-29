#include <iostream>
#include "core/Optimizer/Optimizer.hpp"
#include "core/ProblemLoader/ProblemLoader.hpp"
#include "core/Evaluator/Evaluator.hpp"

using namespace std;
using namespace LcVRPContest;

int main() {
    ProblemLoader loader("Vrp-Set-A", "A-n33-k5");
    ProblemData data = loader.LoadProblem();

    cout << "Loaded problem: " << data.GetName() << endl;
    cout << "Number of clients: " << data.GetNumCustomers() << endl;

    int numGroups = 5; 
    Evaluator evaluator(data, numGroups);

    cout << evaluator.GetSolutionSize() << endl;
    cout << evaluator.GetNumGroups() << endl;

    int populationSize = 100;
    Optimizer optimizer(evaluator, populationSize);
    
    cout << "\n=== INITALIZE OPTIMIZATION ===" << endl;
    optimizer.Initialize();

    int maxIterations = 500;
    cout << "Starting optimization (Max " << maxIterations << " iterations)..." << endl;

    for (int i = 0; i < maxIterations; ++i) {
        optimizer.RunIteration();

        if (i % 50 == 0 || i < 50) {
            double bestF = optimizer.GetCurrentBestFitness();
            cout << "Iteration " << i << " | Best fitness: " << bestF << endl;
        }
    }

    Individual* best = optimizer.GetCurrentBest();
    if (best != nullptr) {
        cout << "\n=== FINISHED OPTIMIZATION ===" << endl;
        cout << "Final Fitness: " << optimizer.GetCurrentBestFitness() << endl;
        
        optimizer.PrintIndivual(*(best->getGenome()), best->getFitness());
    }

    return 0;
}