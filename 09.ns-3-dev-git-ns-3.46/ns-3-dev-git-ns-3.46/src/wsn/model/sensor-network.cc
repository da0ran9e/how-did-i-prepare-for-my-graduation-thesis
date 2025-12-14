#include "sensor-network.h"

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
    
    NotifyAttributeChange(key, value);
    return true;
}


void SensorNetwork::Build()
{
    // Implementation of the Build method
    WsnObject::Build();
}

} // namespace wsn
} // namespace ns3
