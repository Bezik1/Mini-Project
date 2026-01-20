# 🧠  Mini Project
<p align="center">
    <a href="https://skillicons.dev">
        <img src="https://skillicons.dev/icons?i=cpp,vscode,github,git" />
    </a>
</p>

## 💡 Overview
This project goal was to find the best soultion for the 
`Limited Capacitated Vehicle Routing Problem (LcVRP)`. To 
achieve it we had to used `Genetic Algorithm (GA)` written
in lower level `C++`, with the code optimized down to the last
detail.

## ⚙️ Commands
Note in case of path error, modify the `ProblemLoader::BASE_PATH`,
`ProblemLoader::BASE_EXTENSION`, folderName or instanceName parameters.
```bash
    ## Build Command
    /usr/bin/clang++ -std=c++14 -g \
    src/main.cpp \
    src/core/Evaluator/Evaluator.cpp \
    src/core/GeneticAlgorithm/GeneticAlgorithm.cpp \
    src/core/Optimizer/Optimizer.cpp \
    src/core/Individual/Individual.cpp \
    src/core/ProblemData/ProblemData.cpp \
    src/core/ProblemLoader/ProblemLoader.cpp \
    -o src/main

    ## Run Command
    ./src/main
```

## Documentation

### Genetic Algorithm

### Optimizer


### Evaluator


### Problem Loader


### Problem Data


### Individual

