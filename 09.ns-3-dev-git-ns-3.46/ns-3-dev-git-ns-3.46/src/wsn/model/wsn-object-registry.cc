#include "wsn-object-registry.h"

namespace ns3 {
namespace wsn {

void RegisterWsnObjects()
{
    auto& factory = WsnObjectFactory::Instance();

    factory.RegisterType("SN",
        [](const std::string& name) {
            return std::make_shared<SensorNetwork>(name);
        });

    factory.RegisterType("WirelessChannel",
        [](const std::string& name) {
            return std::make_shared<WirelessChannel>(name);
        });

    factory.RegisterType("node",
        [](const std::string& name) {
            return std::make_shared<Node>(name);
        });

    factory.RegisterType("Mobility",
        [](const std::string& name) {
            return std::make_shared<Mobility>(name);
        });

    factory.RegisterType("MAC",
        [](const std::string& name) {
            return std::make_shared<WsnMac>(name);
        });

    factory.RegisterType("Routing",
        [](const std::string& name) {
            return std::make_shared<WsnRouting>(name);
        });

    factory.RegisterType("Application",
        [](const std::string& name) {
            return std::make_shared<WsnApp>(name);
        });

    factory.RegisterType("ResourceManager",
        [](const std::string& name) {
            return std::make_shared<ResourceManager>(name);
        });
}

bool MatchWildcard(const std::string& pattern,
                   const std::string& path)
{
    auto starPos = pattern.find("[*]");
    if (starPos == std::string::npos)
        return pattern == path;

    std::string prefix = pattern.substr(0, starPos);
    std::string suffix = pattern.substr(starPos + 3);

    return path.compare(0, prefix.size(), prefix) == 0 &&
           path.size() >= prefix.size() + suffix.size() &&
           path.compare(path.size() - suffix.size(),
                         suffix.size(),
                         suffix) == 0;
}

WsnObjectRegistry::WsnObjectRegistry() : m_factory(WsnObjectFactory::Instance())
{
}

std::shared_ptr<ns3::wsn::WsnObject>
WsnObjectRegistry::ResolveOrCreate(const std::string& path)
{
    if (path.empty())
        return nullptr;

    auto segments = ParsePath(path);
    if (segments.empty())
        return nullptr;

    std::shared_ptr<ns3::wsn::WsnObject> current;

    for (size_t i = 0; i < segments.size(); ++i) {
        const auto& seg = segments[i];

        if (i == 0) {
            // Root object (e.g., SN)
            current = GetOrCreateRoot(seg);
            continue;
        }

        // Child object
        auto child = current->GetChild(seg.type, seg.name);
        if (!child) {
            child = m_factory.Create(seg.type, seg.name);
            current->AddChild(child);
        }
        ApplyWildcardRules(child);
        current = child;
    }

    return current;
}

void WsnObjectRegistry::AddWildcardRule(
        const std::string& pathPattern,
        const std::string& property,
        const std::string& value)
{
    m_wildcardRules.push_back({ pathPattern, property, value });
}

void WsnObjectRegistry::ApplyWildcardRules(
        std::shared_ptr<ns3::wsn::WsnObject> obj)
{
    std::string objPath = obj->GetPath();

    for (const auto& rule : m_wildcardRules) {
        if (MatchWildcard(rule.pathPattern, objPath)) {
            obj->SetProperty(rule.property, rule.value);
        }
    }
}


std::shared_ptr<WsnObject>
WsnObjectRegistry::GetRoot(const std::string& name) const
{
    auto it = m_roots.find(name);
    return (it != m_roots.end()) ? it->second : nullptr;
}

void
WsnObjectRegistry::Clear()
{
    m_roots.clear();
}

/* =========================
 * Path parsing helpers
 * ========================= */

WsnObjectRegistry::PathSegment
WsnObjectRegistry::ParseSegment(const std::string& segment)
{
    PathSegment seg;

    auto lb = segment.find('[');
    auto rb = segment.find(']');

    if (lb != std::string::npos && rb != std::string::npos && rb > lb) {
        seg.type = segment.substr(0, lb);
        seg.name = segment.substr(lb + 1, rb - lb - 1);
    } else {
        seg.type = segment;
        seg.name = "";
    }

    return seg;
}

std::vector<WsnObjectRegistry::PathSegment>
WsnObjectRegistry::ParsePath(const std::string& path)
{
    std::vector<PathSegment> segments;
    std::stringstream ss(path);
    std::string token;

    while (std::getline(ss, token, '.')) {
        if (!token.empty())
            segments.push_back(ParseSegment(token));
    }

    return segments;
}

/* =========================
 * Root handling
 * ========================= */

std::shared_ptr<WsnObject>
WsnObjectRegistry::GetOrCreateRoot(const PathSegment& seg)
{
    auto it = m_roots.find(seg.type);
    if (it != m_roots.end())
        return it->second;

    auto root = m_factory.Create(seg.type, seg.name);
    m_roots[seg.type] = root;
    return root;
}

} // namespace wsn
} // namespace ns3
