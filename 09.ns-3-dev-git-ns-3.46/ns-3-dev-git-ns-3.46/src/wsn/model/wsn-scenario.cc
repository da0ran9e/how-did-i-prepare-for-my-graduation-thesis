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
        if (key == "SN.numNodes")
            m_numNodes = std::stoi(value);
        else if (key == "SN.field_x")
            m_fieldX = std::stod(value);
        else if (key == "SN.field_y")
        // parse node coordinates
        
        std::regex xCoorRegex(R"(SN\.node\[(\d+)\]\.xCoor)");
        std::regex yCoorRegex(R"(SN\.node\[(\d+)\]\.yCoor)");
        std::smatch match;
        if (std::regex_match(key, match, xCoorRegex)) {
            size_t idx = std::stoul(match[1]);
            if (m_nodeCoords.size() <= idx) m_nodeCoords.resize(idx + 1);
            m_nodeCoords[idx].first = std::stod(value);
            std::cout << "Parsed node[" << idx << "] xCoor = " << value << std::endl;
        } else if (std::regex_match(key, match, yCoorRegex)) {
            size_t idx = std::stoul(match[1]);
            if (m_nodeCoords.size() <= idx) m_nodeCoords.resize(idx + 1);
            m_nodeCoords[idx].second = std::stod(value);
            std::cout << "Parsed node[" << idx << "] yCoor = " << value << std::endl;
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
} // namespace wsn
} // namespace ns3