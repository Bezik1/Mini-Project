#pragma once

#include "../ProblemData/ProblemData.hpp"
#include <string>

using namespace std;

namespace LcVRPContest {
    class ProblemLoader {
    public:
        ProblemLoader(const string& folderName, const string& instanceName);

        ProblemData loadProblem();

    private:
        string folderName;
        string instanceName;
        string basePath;

        void parseLcVrpFile(const string& filePath, ProblemData& problemData);
        void parseEdgeWeightSection(ifstream& file, ProblemData& problemData);
        void parseNodeCoordSection(ifstream& file, ProblemData& problemData);
        void parseDemandSection(ifstream& file, ProblemData& problemData);
        void parseDepotSection(ifstream& file, ProblemData& problemData);
    };
}