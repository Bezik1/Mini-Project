#include "ProblemLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace LcVRPContest;

ProblemLoader::ProblemLoader(const string& folderName, const string& instanceName)
    : folderName(folderName), 
    instanceName(instanceName) 
{
    basePath = "../data/lcvrp_v3/" + folderName + "/";
}

ProblemData ProblemLoader::loadProblem() {
    ProblemData problemData;
    string filePath = basePath + instanceName + ".lcvrp";
    
    parseLcVrpFile(filePath, problemData);
    
    if (problemData.getEdgeWeightType() == "EUC_2D") {
        problemData.buildEdgeWeightMatrix();
    }
    
    return problemData;
}

void ProblemLoader::parseLcVrpFile(const string& filePath, ProblemData& problemData) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << filePath << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("NAME") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string name = line.substr(colonPos + 1);
                size_t start = name.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    name = name.substr(start);
                    size_t end = name.find_last_not_of(" \t\r\n");
                    name = name.substr(0, end + 1);
                }
                problemData.setName(name);
            }
        }
        else if (line.find("DIMENSION") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                int dimension = stoi(line.substr(colonPos + 1));
                problemData.setDimension(dimension);
            }
        }
        else if (line.find("CAPACITY") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                int capacity = stoi(line.substr(colonPos + 1));
                problemData.setCapacity(capacity);
            }
        }
        else if (line.find("DISTANCE") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                double distance = stod(line.substr(colonPos + 1));
                problemData.setDistance(distance);
            }
        }
        else if (line.find("EDGE_WEIGHT_TYPE") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string type = line.substr(colonPos + 1);
                size_t start = type.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    type = type.substr(start);
                    size_t end = type.find_last_not_of(" \t\r\n");
                    type = type.substr(0, end + 1);
                }
                problemData.setEdgeWeightType(type);
            }
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            parseEdgeWeightSection(file, problemData);
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            parseNodeCoordSection(file, problemData);
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            parseDemandSection(file, problemData);
        }
        else if (line.find("DEPOT_SECTION") != string::npos) {
            parseDepotSection(file, problemData);
        }
        else if (line.find("PERMUTATION") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                vector<int> permutation;
                string permData = line.substr(colonPos + 1);
                istringstream iss(permData);
                int customerId;
                while (iss >> customerId) {
                    permutation.push_back(customerId);
                }
                problemData.setPermutation(permutation);
            }
        }
        else if (line.find("EOF") != string::npos) {
            break;
        }
    }
    file.close();
}

void ProblemLoader::parseEdgeWeightSection(ifstream& file, ProblemData& problemData) {
    int dimension = problemData.getDimension();
    vector<vector<double>> edgeWeights(dimension, vector<double>(dimension, 0.0));

    for (int i = 1; i < dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            double weight;
            file >> weight;
            edgeWeights[i][j] = weight;
            edgeWeights[j][i] = weight;
        }
    }
    problemData.setEdgeWeights(edgeWeights);
}

void ProblemLoader::parseNodeCoordSection(ifstream& file, ProblemData& problemData) {
    int dimension = problemData.getDimension();
    vector<Coordinate> coordinates(dimension);

    for (int i = 0; i < dimension; ++i) {
        int nodeId;
        double x, y;
        file >> nodeId >> x >> y;
        coordinates[nodeId - 1] = Coordinate(x, y);
    }
    problemData.setCoordinates(coordinates);
}

void ProblemLoader::parseDemandSection(ifstream& file, ProblemData& problemData) {
    int dimension = problemData.getDimension();
    vector<int> demands(dimension);

    for (int i = 0; i < dimension; ++i) {
        int nodeId, demand;
        file >> nodeId >> demand;
        demands[nodeId - 1] = demand;
    }
    problemData.setDemands(demands);
}

void ProblemLoader::parseDepotSection(ifstream& file, ProblemData& problemData) {
    int depot;
    file >> depot;
    problemData.setDepot(depot);
    int terminator;
    file >> terminator;
}