#include "sensor-network.h"
#include "wsn-object.h"

namespace ns3 {
namespace wsn {

SensorNetwork::~SensorNetwork() = default;

bool SensorNetwork::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "field_x") {
        field_x = std::stoi(value);
    }
    else if (key == "field_y") {
        field_y = std::stoi(value);
    }
    else if (key == "field_z") {
        field_z = std::stoi(value);
    }
    else if (key == "numNodes") {
        numNodes = std::stoi(value);
    }
    else if (key == "deployment") {
        deployment = value;
    }
    else if (key == "numPhysicalProcesses") {
        numPhysicalProcesses = std::stoi(value);
    }
    else if (key == "wirelessChannel") {
        wirelessChannelName = value;
    }
    else {
        return false;
    }
    
    NotifyAttributeChanged(key, value);
    return true;
}

void SensorNetwork::Build(BuildContext& ctx)
{
    std::cout << "=== Building Sensor Network ===" << std::endl;
    std::cout << "Field size: "
                << field_x << " x "
                << field_y << " x "
                << field_z << std::endl;
    std::cout << "Number of nodes: " << numNodes << std::endl;

    ctx.nodes.Create(numNodes);
    for (uint32_t i = 0; i < numNodes; ++i) {
        Ptr<ns3::Node> node = CreateObject<ns3::Node>();
        ctx.nodes.Add(node);
        //std::cout << "Created Node " << i << " with ID " << node->GetId() << std::endl;
        ctx.nodeAddr[node->GetId()] = i;
    }

    // auto children = GetChildren("node");
    // for (auto &child : children) {
    //     //std::cout << "Building child node: " << child->GetPath() << std::endl;
    //     // if (auto node = dynamic_cast<Node*>(child.get())) {
    //     //     node->Build(ctx);
    //     //     //std::cout << "Building child node: " << node->GetAddr() << std::endl;
    //     // }
    //     if (child->GetTypeName() == "node"){
    //         static_cast<Node*>(child.get())->Build(ctx);
    //     }
        
    // }
    
     WsnObject::Build(ctx);

    // for (auto &pair : m_children) {
    //     for (auto &child : pair.second) {
    //         //std::cout << "Building child: " << child->GetPath() << std::endl;
    //         child->Build(ctx);
            
    //     }
    // }
    

}


} // namespace wsn
} // namespace ns3
