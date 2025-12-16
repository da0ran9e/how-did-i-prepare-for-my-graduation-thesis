#pragma once
#ifndef WSN_OBJECT_REGISTRY_H
#define WSN_OBJECT_REGISTRY_H

namespace ns3 {
namespace wsn {

void RegisterWsnObjects();

class WsnObjectRegistry
{
public:

    struct WildcardRule
    {
        std::string pathPattern;   // "SN.node[*]"
        std::string property;      // "xCoor"
        std::string value;         // "10"
    };

    WsnObjectRegistry();
    ~WsnObjectRegistry() = default;

    /**
     * Resolve or create object by hierarchical path
     * Example: "SN.node[0].mac"
     */
    std::shared_ptr<WsnObject> ResolveOrCreate(const std::string& path);

    /**
     * Get root object by name
     */
    std::shared_ptr<WsnObject> GetRoot(const std::string& name) const;

    /**
     * Clear all objects
     */
    void Clear();
    void AddWildcardRule(const std::string& pathPattern,
                        const std::string& property,
                        const std::string& value);
    void ApplyWildcardRules(std::shared_ptr<WsnObject> obj);

    struct PathSegment {
        std::string type;   // e.g., "node"
        std::string name;   // e.g., "0"
    };

    // Helpers
    static std::vector<PathSegment> ParsePath(const std::string& path);
    static PathSegment ParseSegment(const std::string& segment);

    std::shared_ptr<WsnObject> GetOrCreateRoot(const PathSegment& seg);

    std::unordered_map<std::string, std::shared_ptr<WsnObject>> m_roots;
    WsnObjectFactory m_factory;
    std::vector<WildcardRule> m_wildcardRules;

};

} // namespace wsn
} // namespace ns3

#endif // WSN_OBJECT_REGISTRY_H