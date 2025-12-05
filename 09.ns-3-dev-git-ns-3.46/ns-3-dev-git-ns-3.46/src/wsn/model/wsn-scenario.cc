#include "wsn-scenario.h"
namespace ns3 
{
namespace wsn
{ 
      
void WsnScenario::onSection(const std::string &section)
{
    m_currentSection = section;

    if (m_config.find(section) == m_config.end())
        m_config[section] = {};
}

void WsnScenario::onKeyValue(const std::string &key,
                             const std::string &value,
                             const std::string &comment,
                             const std::string &baseDir)
{
    m_config[m_currentSection][key] = value;

    if (m_currentSection == "General") {
        if (key == "SN.numNodes")
            m_numNodes = std::stoi(value);
        else if (key == "SN.field_x")
            m_fieldX = std::stod(value);
        else if (key == "SN.field_y")
            m_fieldY = std::stod(value);
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
    IniParser iniParser;

    iniParser.onSection = [this](const std::string &sec) {
        this->onSection(sec);
    };

    iniParser.onKeyValue = [this](const std::string &key,
                              const std::string &value,
                              const std::string &comment,
                              const std::string &baseDir)
    {
        this->onKeyValue(key, value, comment, baseDir);
    };

    // parse file
    iniParser.read(iniFile);

    if (m_traceEnabled)
    {
        WsnTrace::Get().Enable(true);
        WsnTrace::Get().SetTraceFile(m_traceFile);

        WsnTrace::Get().Trace()
            << "Trace started. numNodes=" << m_numNodes
            << " field=" << m_fieldX << "x" << m_fieldY
            << std::endl;
    }
    else
    {
        WsnTrace::Get().Enable(false);
    }

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