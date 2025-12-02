#include "wsn-scenario.h"
namespace ns3 
{
//NS_LOG_COMPONENT_DEFINE("WsnScenario");
namespace wsn
{   

void WsnScenario::Configure(const IniParser &ini)
{
    m_numNodes = ini.GetInt("SN.numNodes", 0);

    m_positions.resize(m_numNodes);

    for (int i = 0; i < m_numNodes; i++) {
        std::string px = "SN.node[" + std::to_string(i) + "].xCoor";
        std::string py = "SN.node[" + std::to_string(i) + "].yCoor";

        m_positions[i].first  = ini.GetDouble(px, 0.0);
        m_positions[i].second = ini.GetDouble(py, 0.0);
    }
}

void WsnScenario::Trace(const std::string &path)
{
    std::ofstream ofs(path);
    for (int i = 0; i < m_numNodes; i++) {
        ofs << "Node " << i 
            << " X=" << m_positions[i].first
            << " Y=" << m_positions[i].second << "\n";
    }
    ofs.close();
}
} // namespace wsn
} // namespace ns3