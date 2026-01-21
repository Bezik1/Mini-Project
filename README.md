# $Mini$ $Project$

## 💡 $Overview$
This project goal was to find the best soultion for the 
`Limited Capacitated Vehicle Routing Problem (LcVRP)`. To 
achieve it we had to used `Genetic Algorithm (GA)` written
in lower level `C++`, with the code optimized down to the last
detail.

## ⚙️ $Commands$
Note:
In case of path error, modify the `ProblemLoader::BASE_PATH`,
`ProblemLoader::BASE_EXTENSION`, folderName or instanceName parameters.
```bash
## Create output directory (once)
mkdir -p bin

## Build Command
/usr/bin/clang++ -std=c++14 -g \
src/main.cpp \
src/core/Evaluator/Evaluator.cpp \
src/core/GeneticAlgorithm/GeneticAlgorithm.cpp \
src/core/Optimizer/Optimizer.cpp \
src/core/Individual/Individual.cpp \
src/core/ProblemData/ProblemData.cpp \
src/core/ProblemLoader/ProblemLoader.cpp \
-o bin/main

## Run Command
./bin/main
```

## $Parameters$
```C++
int populationSize;     // Controls the size of the population
double mutProb;         // Controls probability for mutation occurances
double crossoverProb;   // It's the probability for solution crossing, when it did not occur we simply copy indivduals
int numTurns;           // Number of turns, at which tournament selection will be looking for mate
int numEpochs;          // How many times Optimizer::runIteration will be invoked
int numGroups;          // Number of groups of trucks to optimize
```

## $Documentation$
$Key$ $Features$:
* No allocations on heap during `Optimizer::runIteration()` method;
* Memory locality, while operating on `Individual::genome`, because they are
allocated on the same table `genomes`. As a result genomes for the most of the time
are stored in L1/L2 caches.
* Operations on raw pointers `int*`, instead of using `std::vector`.

### $Genetic$ $Algorithm$
The GeneticAlgorithm class acts as the orchestrator for the entire optimization process. It is designed for maximum memory efficiency by pre-allocating large contiguous blocks of memory for the population.

Memory Management: Instead of each Individual owning its own `std::vector`, the GA allocates two large raw arrays (`genomes` and `prevGenomes`) representing the current and previous generations.

Buffer Strategy: It uses a double-buffering approach (`population` and `previousPopulation`) to avoid redundant allocations during the survival and crossover phases.

Lazy Initialization: Objects are constructed once and their internal pointers are swapped, minimizing constructor/destructor overhead during the evolution loop.

### $Optimizer$
The Optimizer handles the selection and evolution logic. It is optimized for high-throughput processing of individuals.

Tournament Selection: Implements a fast selection process by sampling `numTurns` individuals and picking the best, avoiding the need to sort the entire population unless necessary.

Elitism via HeapSort: To ensure the best solutions are preserved, the optimizer uses a custom `HeapSort` implementation, which works in time $O(nlog(n))$ and does not allocate any additioal memory on heap.

In-Place Evolution: Crossover and mutation operations are performed directly on the raw memory buffers provided by the GA, reducing cache misses and stack overhead.

### $Evaluator$
The Evaluator is the most computationally expensive component. It translates the genome (grouping) into actual VRP routes and calculates their cost.

Raw Pointer Buffers: It utilizes `int** routesBuffer` and `int* routeSizes` to manage routes. This avoids the overhead of `std::vector<std::vector<int>>`, which involves multiple heap allocations and pointer indirections.

Constraint Checking: Implements strict validation for:

Capacity: Ensuring the sum of demands in a route doesn't exceed the vehicle limit.

Distance: Ensuring the total travel distance (including returns to depot) stays within the max allowed distance.

Greedy Decoding: The evaluator uses the project's specific permutation data to decode which customer belongs to which group, effectively partitioning the problem for the GA.

### $Problem$ $Loader$
A robust parser for .lcvrp files. It supports both EUC_2D (coordinate-based) and EXPLICIT (matrix-based) weight types.Pre-computation: If the problem uses EUC_2D, the loader triggers the building of an edge weight matrix within ProblemData to transform $O(\text{dist calculation})$ into $O(1)$ lookups during the evaluation phase.

### $Problem$ $Data$
A data container optimized for fast access.Coordinate System: Uses a lightweight struct Coordinate to store $(x, y)$ pairs.Edge Matrix: Stores distances in a vector<vector<double>> for fast $O(1)$ access. In the EXPLICIT mode, it handles lower-triangular matrix parsing to save space.

### $Individual$
The Individual class is a wrapper around the raw genome data.

Memory Efficiency: It does not own the memory it points to. It operates on a sharedGenomeSpace provided by the GA. This allows thousands of individuals to exist without thousands of independent heap allocations.

Friend Swap: Implements a high-performance friend void swap using the copy-and-swap idiom. This ensures exception-safe assignments and efficient moving of pointers rather than deep-copying data where possible.

```C++

friend void swap(Individual& first, Individual& second);
```
Custom Operators: Overloads < and > to facilitate integration with the HeapSort algorithm.

<p align="center">
    <a href="https://skillicons.dev">
        <img src="https://skillicons.dev/icons?i=cpp,vscode,github,git" />
    </a>
</p>