#include "ProblemLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace LcVRPContest;

ProblemLoader::ProblemLoader(const string& folderName, const string& instanceName)
	: folderName(folderName), 
    instanceName(instanceName) 
{
	basePath = "../data/lcvrp/" + folderName + "/";
}

ProblemData ProblemLoader::LoadProblem() {
    ProblemData problem_data;
    string file_path = basePath + instanceName + ".lcvrp";
    
    parseLcVrpFile(file_path, problem_data);
    
    // we need to build the edge weight matrix if EUC_2D
    if (problem_data.getEdgeWeightType() == "EUC_2D") {
        problem_data.buildEdgeWeightMatrix();
    }
    
    return problem_data;
}

void ProblemLoader::parseLcVrpFile(const string& file_path, ProblemData& problem_data) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << file_path << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        // skip empty lines
        if (line.empty()) continue;

        // parse header fields
        if (line.find("NAME") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string name = line.substr(colon_pos + 1);
                // remove all kind of whitespaces
                size_t start = name.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    name = name.substr(start);
                    size_t end = name.find_last_not_of(" \t\r\n");
                    name = name.substr(0, end + 1);
                }
                problem_data.setName(name);
            }
        }
        else if (line.find("DIMENSION") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                int dimension = stoi(line.substr(colon_pos + 1));
                problem_data.setDimension(dimension);
            }
        }
        else if (line.find("CAPACITY") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                int capacity = stoi(line.substr(colon_pos + 1));
                problem_data.setCapacity(capacity);
            }
        }
        else if (line.find("DISTANCE") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                double distance = stod(line.substr(colon_pos + 1));
                problem_data.setDistance(distance);
            }
        }
        else if (line.find("EDGE_WEIGHT_TYPE") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string type = line.substr(colon_pos + 1);
                size_t start = type.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    type = type.substr(start);
                    size_t end = type.find_last_not_of(" \t\r\n");
                    type = type.substr(0, end + 1);
                }
                problem_data.setEdgeWeightType(type);
            }
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            parseEdgeWeightSection(file, problem_data);
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            parseEdgeWeightSection(file, problem_data);
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            parseEdgeWeightSection(file, problem_data);
        }
        else if (line.find("DEPOT_SECTION") != string::npos) {
            parseEdgeWeightSection(file, problem_data);
        }
        else if (line.find("PERMUTATION") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                vector<int> permutation;
                string perm_data = line.substr(colon_pos + 1);
                istringstream iss(perm_data);
                int customer_id;
                while (iss >> customer_id) {
                    permutation.push_back(customer_id);
                }
                problem_data.setPermutation(permutation);
            }
        }
        else if (line.find("EOF") != string::npos) {
            break;
        }
    }

    file.close();
}

void ProblemLoader::parseEdgeWeightSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.getDimension();
    vector<vector<double>> edge_weights(dimension, vector<double>(dimension, 0.0));

    // for reading triangular matrix
    for (int i = 1; i < dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            double weight;
            file >> weight;
            edge_weights[i][j] = weight;
            edge_weights[j][i] = weight; // we assume its symmetric
        }
    }

    problem_data.setEdgeWeights(edge_weights);
}

void ProblemLoader::parseNodeCoordSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.getDimension();
    vector<Coordinate> coordinates(dimension);

    for (int i = 0; i < dimension; ++i) {
        int node_id;
        double x, y;
        file >> node_id >> x >> y;
        // (again) node ids start from 1, array indices from 0 - i hate this
        coordinates[node_id - 1] = Coordinate(x, y);
    }

    problem_data.setCoordinates(coordinates);
}

void ProblemLoader::parseDemandSection(ifstream& file, ProblemData& problem_data) {
    int dimension = problem_data.getDimension();
    vector<int> demands(dimension);

    for (int i = 0; i < dimension; ++i) {
        int node_id, demand;
        file >> node_id >> demand;
        // (same as above)
        demands[node_id - 1] = demand;
    }

    problem_data.setDemands(demands);
}

void ProblemLoader::parseDepotSection(ifstream& file, ProblemData& problem_data) {
    int depot;
    file >> depot;
    problem_data.setDepot(depot);
    
    // read -1 terminator
    int terminator;
    file >> terminator;
}
