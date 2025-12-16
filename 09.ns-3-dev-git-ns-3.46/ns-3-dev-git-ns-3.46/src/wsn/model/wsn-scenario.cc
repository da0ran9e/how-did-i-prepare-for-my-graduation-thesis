#include "wsn-scenario.h"

namespace ns3 {
namespace wsn {

void WsnScenario::onSection(const std::string &section)
{
    m_currentSection = section;
    std::cout << "Callback Section: " << section << std::endl;
    m_trace.Trace("Section: " + section);
    // if (m_config.find(section) == m_config.end())
    //     m_config[section] = {};
}

bool HasWildcard(const std::string& path)
{
    return path.find("[*]") != std::string::npos;
}


void WsnScenario::onKeyValue(const std::string &key,
                             const std::string &value,
                             const std::string &comment,
                             const std::string &baseDir)
{
    if (m_currentSection != "General")
        return;

    ParsedKey parsed = ParseIniKey(key);

    if (HasWildcard(parsed.objectPath)) {
        m_registry.AddWildcardRule(parsed.objectPath,
                                parsed.property,
                                value);
        return;
    }

    auto obj = m_registry.ResolveOrCreate(parsed.objectPath);

    if (!obj) {
        //NS_FATAL_ERROR("Cannot resolve object path: " << parsed.objectPath);
        std::cout << "Cannot resolve object path: " << parsed.objectPath << std::endl;
        return;
    }

    // set property
    if (!obj->SetProperty(parsed.property, value)) {
        std::cout << "Warning: Unknown property '" << parsed.property
                  << "' for object at path '" << parsed.objectPath << "'" << std::endl;
    }
}

ParsedKey WsnScenario::ParseIniKey(const std::string& key)
{
    auto pos = key.rfind('.');
    if (pos == std::string::npos) {
        // no dot → property belongs to root object
        return { "", key };
    }

    ParsedKey result;
    result.objectPath = key.substr(0, pos);
    result.property   = key.substr(pos + 1);
    return result;
}


void WsnScenario::configure(std::string iniFile)
{
    RegisterWsnObjects();
    m_trace.Open(m_traceFile);
    IniParser iniParser;
    iniParser.setListener(this);
    
    iniParser.read(iniFile);

    auto root = m_registry.GetRoot("SN");
    if (root){
        std::ostringstream os;
        root->DebugPrint(os);
        std::cout << os.str();
    }
        

    // // Debug print
    // std::cout << "Number of nodes = " << m_numNodes << std::endl;
    // std::cout << "Field size = " << m_fieldX << " x " << m_fieldY << std::endl;

    // TODO:
    //   - NodeContainer 
    //   - positions
    //   - MAC/PHY
}


} // namespace wsn
} // namespace ns3