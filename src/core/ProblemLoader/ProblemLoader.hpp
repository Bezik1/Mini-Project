#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <string>

using namespace std;

namespace LcVRPContest {
	class ProblemLoader {
	public:
		ProblemLoader(const string& folderName, const string& instanceName);

		ProblemData LoadProblem();

	private:
		string folderName;
		string instanceName;
		string basePath;

		void parseLcVrpFile(const string& file_path, ProblemData& problem_data);
		void parseEdgeWeightSection(ifstream& file, ProblemData& problem_data);
		void parseNodeCoordSection(ifstream& file, ProblemData& problem_data);
		void parseDemandSection(ifstream& file, ProblemData& problem_data);
		void parseDepotSection(ifstream& file, ProblemData& problem_data);
	};
}
