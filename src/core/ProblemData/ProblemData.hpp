#pragma once

#include <string>
#include <vector>

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
			ProblemData(const ProblemData &other);

			// getters
			string getName() const { return name_; }
			int getDimension() const { return dimension_; }
			int getCapacity() const { return capacity_; }
			double getDistance() const { return distance_; }
			bool hasDistanceConstraint() const { return has_distance_constraint_; }
			string getEdgeWeightType() const { return edge_weight_type_; }
			int getDepot() const { return depot_; }
			int getNumCustomers() const { return dimension_ - 1; }
			
			const vector<Coordinate>& getCoordinates() const { return coordinates_; }
			const vector<int>& getDemands() const { return demands_; }
			const vector<int>& getPermutation() const { return permutation_; }
			const vector<vector<double>>& getEdgeWeights() const { return edge_weights_; }

			// setters
			void setName(const string& name) { name_ = name; }
			void setDimension(int dimension) { 
				dimension_ = dimension;
				coordinates_.resize(dimension);
				demands_.resize(dimension);
			}
			void setCapacity(int capacity) { capacity_ = capacity; }
			void setDistance(double distance) { 
				distance_ = distance; 
				has_distance_constraint_ = true;
			}
			void setEdgeWeightType(const string& type) { edge_weight_type_ = type; }
			void setDepot(int depot) { depot_ = depot; }
			
			void setCoordinates(const vector<Coordinate>& coordinates) { coordinates_ = coordinates; }
			void setDemands(const vector<int>& demands) { demands_ = demands; }
			void setPermutation(const vector<int>& permutation) { permutation_ = permutation; }
			void setEdgeWeights(const vector<vector<double>>& edge_weights) { edge_weights_ = edge_weights; }

		// utility methods
		double calculateDistance(int i, int j) const;
		void buildEdgeWeightMatrix();

		static constexpr double WRONG_VAL = -1.0;

	private:
			string name_;
			int dimension_; // total number of nodes (customers + depot)
			int capacity_; // singular vehicle capacity
			double distance_; // maximum distance constraint (optional)
			bool has_distance_constraint_;
			string edge_weight_type_; // EUC_2D, EXPLICIT, maybe more?
			int depot_; // depot node id (usually 1)

			vector<Coordinate> coordinates_; // node coordinates (indexed from 0, but node 1 is at index 0)
			vector<int> demands_; // customer demands (indexed from 0, but node 1 is at index 0)
			vector<int> permutation_; // fixed permutation for LcVRP (customer indexes 1..n-1)
			vector<vector<double>> edge_weights_; // distance matrix
		};
}