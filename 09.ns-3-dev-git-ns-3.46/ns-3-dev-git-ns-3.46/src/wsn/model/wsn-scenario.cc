#include "wsn-scenario.h"
#include <regex>
#include <utility>
namespace ns3 {
namespace wsn {

NS_LOG_COMPONENT_DEFINE ("WsnScenario");

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
     // Bỏ qua các section không cần
    if (m_currentSection != "General")
        return;

    // Tách key thành path tokens
    auto tokens = SplitKey(key);
    if (tokens.empty())
        return;

    // Property name là token cuối
    std::string property = tokens.back();
    tokens.pop_back();

    // Resolve object theo path
    WsnObject* target = ResolveObject(tokens);
    if (!target) {
        // optional: log warning
        return;
    }

    // Gán thuộc tính
    target->SetProperty(property, value);
}

std::vector<std::string> WsnScenario::SplitKey(const std::string &key)
{
    std::vector<std::string> tokens;
    std::stringstream ss(key);
    std::string token;

    while (std::getline(ss, token, '.')) {
        tokens.push_back(token);
    }
    return tokens;
}

WsnObject* WsnScenario::ResolveObject(
    const std::vector<std::string> &tokens)
{
    WsnObject* current = m_root.get();

    for (const auto &t : tokens)
    {
        current = current->GetOrCreateChild(t);
        if (!current)
            return nullptr;
    }
    return current;
}

WsnObject* WsnObject::GetOrCreateChild(const std::string &token)
{
    // token: "node[3]" hoặc "Communication"

    // 1. Tách name + optional index
    std::string name;
    int index = -1;
    ParseIndexedName(token, name, index);

    // 2. Tìm child tương ứng
    auto child = FindChild(name, index);
    if (child)
        return child.get();

    // 3. Chưa có → tạo mới
    auto newChild = CreateChild(name, index);
    AddChild(newChild);
    return newChild.get();
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

NodeContainer WsnScenario::createNodesAndStack()
{
    NodeContainer nodes;
    nodes.Create(m_numNodes);

    return nodes;
}


} // namespace wsn
} // namespace ns3