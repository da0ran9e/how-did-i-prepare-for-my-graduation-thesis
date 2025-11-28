#ifndef WSN_SCENARIO_H
#define WSN_SCENARIO_H

#include "ini-parser.h"
#include <vector>
#include <utility>
#include <string>

class WsnScenario {
public:
    void Configure(const IniParser &ini);
    void Trace(const std::string &path);

private:
    int m_numNodes = 0;
    std::vector<std::pair<double,double>> m_positions;
};

#endif // WSN_SCENARIO_H