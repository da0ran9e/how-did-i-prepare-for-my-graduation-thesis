#ifndef WSN_SCENARIO_H
#define WSN_SCENARIO_H

#pragma once
#include "ini-parser.h"
#include "wsn-trace.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/vector.h"
#include "ns3/log.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/propagation-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"

#include "wsn-routing.h"      
#include "wsn-app.h"          // app non-IP

#include <string>
#include <unordered_map>
namespace ns3 {
namespace wsn {

class WsnScenario : public IniParser::Listener 
{
public:
    void configure(std::string iniFile);
    NodeContainer createNodesAndStack();

private:
    // callbacks
    void onSection(const std::string &section) override;
    void onKeyValue(const std::string &key,
                    const std::string &value,
                    const std::string &comment,
                    const std::string &baseDir) override;

private:
    std::string m_currentSection;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_config; // key-value 
    
    bool m_traceEnabled = false;
    std::string m_traceFile = "wsn-trace.txt";
    WsnTrace m_trace;

    // Parsed parameters
    uint32_t m_numNodes = 0;
    double m_fieldX = 0;
    double m_fieldY = 0;
    std::vector<std::pair<double, double>> m_nodeCoords;
};

} // namespace wsn
} // namespace ns3

#endif // WSN_SCENARIO_H