#include <iostream>

#include "core/ProblemLoader/ProblemLoader.hpp"
#include "core/Evaluator/Evaluator.hpp"
// #include "GeneticAlgorithm.hpp"

using namespace std;

int main() {
    using namespace LcVRPContest;

    ProblemLoader loader("data/lcvrp/Vrp-Set-A", "A-n32-k5");
    ProblemData data = loader.LoadProblem();

    cout << data.GetCapacity() << endl;

    vector<Coordinate> coordinate = data.GetCoordinates();
    for(int i=0; i<coordinate.size(); i++) {

    }

    cout << data.GetCapacity() << endl;
    cout << data.GetCapacity() << endl;
    cout << data.GetCapacity() << endl;
    cout << data.GetCapacity() << endl;


    int numGroups = 5;
    Evaluator evaluator(data, numGroups);

    // CGeneticAlgorithm ga(100, 0.6, 0.1, &evaluator);
    
    // ga.Initialize();
    
    // for(int i = 0; i < 1000; ++i) {
    //     ga.RunIteration();
    // }

    // vector<int>* bestSolution = ga.GetCurrentBest();
    // double bestFitness = ga.GetCurrentBestFitness();

    return 0;
}