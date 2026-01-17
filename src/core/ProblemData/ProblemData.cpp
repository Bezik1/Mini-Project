#include "ProblemData.hpp"

using namespace LcVRPContest;

ProblemData::ProblemData()
    : dimension(0), capacity(0), distance(0.0), 
    hasDistanceConstraintVal(false), depot(1) {}

ProblemData::ProblemData(const ProblemData& other) 
    : name(other.name), dimension(other.dimension), capacity(other.capacity),
    distance(other.distance), hasDistanceConstraintVal(other.hasDistanceConstraintVal),
    edgeWeightType(other.edgeWeightType), depot(other.depot),
    coordinates(other.coordinates), demands(other.demands),
    permutation(other.permutation), edgeWeights(other.edgeWeights) {}

double ProblemData::calculateDistance(int i, int j) const {
    if (i < 0 || i >= dimension || j < 0 || j >= dimension) return WRONG_VAL;
    if (i == j) return 0.0;
    
    if (edgeWeightType == "EUC_2D") {
        double dx = coordinates[i].x - coordinates[j].x;
        double dy = coordinates[i].y - coordinates[j].y;
        return sqrt(dx * dx + dy * dy);
    } 
    else if (edgeWeightType == "EXPLICIT") {
        if (edgeWeights.empty()) return WRONG_VAL;
        // Obsługa macierzy trójkątnej symetrycznej
        return (i > j) ? edgeWeights[i][j] : edgeWeights[j][i];
    }
    return WRONG_VAL;
}

void ProblemData::buildEdgeWeightMatrix() {
    if (edgeWeightType == "EUC_2D") {
        edgeWeights.assign(dimension, vector<double>(dimension, 0.0));
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                edgeWeights[i][j] = calculateDistance(i, j);
            }
        }
    }
}