#pragma once

#include <string>
#include <vector>
#include <cmath>

using namespace std;

namespace LcVRPContest {
    struct Coordinate {
        double x;
        double y;

        Coordinate() : x(0.0), y(0.0) {}
        Coordinate(double x, double y) : x(x), y(y) {}
    };

    class ProblemData {
    public:
        ProblemData();
		ProblemData(const ProblemData& other);

        string getName() const { return name; }
        int getDimension() const { return dimension; }
        int getCapacity() const { return capacity; }
        double getDistance() const { return distance; }
        bool hasDistanceConstraint() const { return hasDistanceConstraintVal; }
        string getEdgeWeightType() const { return edgeWeightType; }
        int getDepot() const { return depot; }
        int getNumCustomers() const { return dimension - 1; }
        
        const vector<Coordinate>& getCoordinates() const { return coordinates; }
        const vector<int>& getDemands() const { return demands; }
        const vector<int>& getPermutation() const { return permutation; }
        const vector<vector<double>>& getEdgeWeights() const { return edgeWeights; }

        void setName(const string& newName) { name = newName; }
        void setDimension(int newDimension) { 
            dimension = newDimension;
            coordinates.resize(dimension);
            demands.resize(dimension);
        }
        void setCapacity(int newCapacity) { capacity = newCapacity; }
        void setDistance(double newDistance) { 
            distance = newDistance; 
            hasDistanceConstraintVal = true;
        }
        void setEdgeWeightType(const string& type) { edgeWeightType = type; }
        void setDepot(int newDepot) { depot = newDepot; }
        
        void setCoordinates(const vector<Coordinate>& newCoordinates) { coordinates = newCoordinates; }
        void setDemands(const vector<int>& newDemands) { demands = newDemands; }
        void setPermutation(const vector<int>& newPermutation) { permutation = newPermutation; }
        void setEdgeWeights(const vector<vector<double>>& newEdgeWeights) { edgeWeights = newEdgeWeights; }

        double calculateDistance(int i, int j) const;
        void buildEdgeWeightMatrix();

        static constexpr double WRONG_VAL = -1.0;

    private:
        string name;
        int dimension;
        int capacity;
        double distance;
        bool hasDistanceConstraintVal;
        string edgeWeightType;
        int depot;

        vector<Coordinate> coordinates;
        vector<int> demands;
        vector<int> permutation;
        vector<vector<double>> edgeWeights;
    };
}