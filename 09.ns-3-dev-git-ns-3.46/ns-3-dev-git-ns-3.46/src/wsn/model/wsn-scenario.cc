#include "wsn-scenario.h"
#include <regex>
#include <utility>
namespace ns3 {
namespace wsn {

void WsnScenario::onSection(const std::string &section)
{
    m_currentSection = section;
    std::cout << "Callback Section: " << section << std::endl;
    m_trace.Trace("Section: " + section);
    if (m_config.find(section) == m_config.end())
        m_config[section] = {};
}

void WsnScenario::onKeyValue(const std::string &key,
                             const std::string &value,
                             const std::string &comment,
                             const std::string &baseDir)
{
    m_config[m_currentSection][key] = value;
    //std::cout << "Callback KeyValue: [" << m_currentSection << "] " << key << " = " << value << std::endl;
    m_trace.Trace("KeyValue: [" + m_currentSection + "] " + key + " = " + value);
    if (m_currentSection == "General") {
        if (key == "SN.numNodes") {
            m_numNodes = std::stoi(value);
            return;
        }
        else if (key == "SN.field_x") {
            m_fieldX = std::stod(value);
            return;
        }
        else if (key == "SN.field_y") {
            m_fieldY = std::stod(value);
            return;
        }

        static const std::regex xCoorRegex(R"(SN\.node\[(\d+)\]\.xCoor)");
        static const std::regex yCoorRegex(R"(SN\.node\[(\d+)\]\.yCoor)");
        std::smatch match;

        if (std::regex_match(key, match, xCoorRegex)) {
            size_t idx = std::stoul(match[1]);
            if (m_nodeCoords.size() <= idx)
                m_nodeCoords.resize(idx + 1);

            m_nodeCoords[idx].first = std::stod(value);
            //std::cout << "Parsed node[" << idx << "] xCoor = " << value << std::endl;
            return;
        }
        else if (std::regex_match(key, match, yCoorRegex)) {
            size_t idx = std::stoul(match[1]);
            if (m_nodeCoords.size() <= idx)
                m_nodeCoords.resize(idx + 1);

            m_nodeCoords[idx].second = std::stod(value);
            //std::cout << "Parsed node[" << idx << "] yCoor = " << value << std::endl;
            return;
        }
    }

    else if (m_currentSection == "Trace") {
        if (key == "enable")
            m_traceEnabled = (value == "true" || value == "1");
        else if (key == "file")
            m_traceFile = value;
    }
}


void WsnScenario::configure(std::string iniFile)
{
    m_trace.Open(m_traceFile);
    IniParser iniParser;
    iniParser.setListener(this);
    
    iniParser.read(iniFile);

    // Debug print
    std::cout << "Number of nodes = " << m_numNodes << std::endl;
    std::cout << "Field size = " << m_fieldX << " x " << m_fieldY << std::endl;

    // TODO:
    //   - NodeContainer 
    //   - positions
    //   - MAC/PHY
}

NodeContainer WsnScenario::CreateNodesAndMobility()
{
    NS_LOG_INFO("Creating " << m_numNodes << " WSN nodes...");

    // 1. nodes
    NodeContainer nodes;
    nodes.Create(m_numNodes);

    // 2. Alloc m_nodeCoords 
    Ptr<ListPositionAllocator> posAlloc = CreateObject<ListPositionAllocator>();

    if (m_nodeCoords.size() != m_numNodes)
    {
        NS_LOG_WARN("Node coordinate list size (" << m_nodeCoords.size()
                     << ") does not match m_numNodes (" << m_numNodes << ")");
    }

    for (uint32_t i = 0; i < m_nodeCoords.size(); i++)
    {
        double x = m_nodeCoords[i].first;
        double y = m_nodeCoords[i].second;

        posAlloc->Add(Vector(x, y, 0.0));
    }

    // 3. Mobility model
    MobilityHelper mobility;
    mobility.SetPositionAllocator(posAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    NS_LOG_INFO("Mobility assigned for all WSN nodes.");

    return nodes;
}

} // namespace wsn
} // namespace ns3