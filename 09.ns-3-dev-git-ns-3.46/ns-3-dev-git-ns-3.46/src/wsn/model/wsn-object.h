#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <mutex>
#include <optional>

namespace ns3 {
namespace wsn {

class WsnObject;
using WsnObjectPtr = std::shared_ptr<WsnObject>;
using WeakWsnObjectPtr = std::weak_ptr<WsnObject>;
using FactoryFunc = std::function<WsnObjectPtr(void)>;

// Base configurable object for WSN configuration tree
class WsnObject : public std::enable_shared_from_this<WsnObject>
{
public:
    WsnObject(const std::string &typeName = "WsnObject", const std::string &instanceName = "");
    virtual ~WsnObject();

    // Identification
    const std::string & GetTypeName() const { return m_typeName; }
    const std::string & GetInstanceName() const { return m_instanceName; }
    void SetInstanceName(const std::string &name) { m_instanceName = name; }

    // Parent / path helpers
    void SetParent(WeakWsnObjectPtr parent);
    WeakWsnObjectPtr GetParent() const;
    std::string GetPath() const; // e.g. "SN.node[3].Communication.MAC"

    WsnObjectPtr GetChild(const std::string &name, bool createIfMissing = false);
    WsnObjectPtr GetChildIndexed(const std::string &name, size_t idx, bool createIfMissing = false);
    void AddChild(const std::string &name, WsnObjectPtr child);
    std::vector<std::string> GetChildNames() const;
    std::vector<WsnObjectPtr> GetChildren(const std::string &name) const;
   
    // Returns true if property handled (consumed).
    virtual bool SetProperty(const std::string &key, const std::string &value);
    virtual std::optional<std::string> GetProperty(const std::string &key) const;

    // Lifecycle hooks
    virtual void Initialize(); // called after parsing full tree
    virtual void Validate();   // validate parameters, throw or log errors
    virtual void Build(); // Build() may convert config -> runtime (ns-3) objects; default: no-op      

    // Factory registry: register per-type factory
    static void RegisterFactory(const std::string &typeName, FactoryFunc f);
    static WsnObjectPtr CreateByType(const std::string &typeName);
    
    // Listener interface
    class Listener {
    public:
        virtual ~Listener() = default;

        // Khi một thuộc tính được parse: name="node[0].xCoor", value="1"
        virtual void OnAttributeChanged(
            WsnObject* obj,
            const std::string& attrName,
            const std::string& attrValue
        ) = 0;

        // Khi một object con được thêm (ví dụ Node thuộc về Network)
        virtual void OnChildAdded(
            WsnObject* parent,
            WsnObject* child
        ) = 0;
    };

    void AddListener(std::shared_ptr<Listener> listener) {
        m_listeners.push_back(listener);
    }

    void RemoveListener(std::shared_ptr<Listener> listener) {
        m_listeners.erase(
            std::remove(m_listeners.begin(), m_listeners.end(), listener),
            m_listeners.end()
        );
    }

protected:
    // Helper: ensure vector for child name exists
    std::vector<WsnObjectPtr> & EnsureChildVector(const std::string &name);

    // Raw property store, default handling
    std::map<std::string, std::string> m_properties;

    // children: name -> vector<instances>
    std::map<std::string, std::vector<WsnObjectPtr>> m_children;

    // Backlink to parent (weak to avoid cycles)
    WeakWsnObjectPtr m_parent;

    // Identification
    std::string m_typeName;     // e.g. "node", "MAC", "SensorNetwork"
    std::string m_instanceName; // instance label (optional) e.g. "3" or user label

    // Synchronization (optional)
    mutable std::mutex m_mutex;

    // Static factory registry
    static std::map<std::string, FactoryFunc> s_factoryRegistry;

private:
    std::vector<std::shared_ptr<Listener>> m_listeners;

protected:
    // Notify listeners about attribute change
    void NotifyAttributeChanged(const std::string& attrName, const std::string& attrValue) {
        for (const auto& listener : m_listeners) {
            listener->OnAttributeChanged(this, attrName, attrValue);
        }
    }

    // Notify listeners about child addition
    void NotifyChildAdded(WsnObjectPtr child) {
        for (const auto& listener : m_listeners) {
            listener->OnChildAdded(this, child.get());
        }
    }
};

} // namespace wsn
} // namespace ns3
