#include "node.h"
namespace ns3 {
namespace wsn {

bool Node::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "xCoor") {
        xCoor = std::stod(value);
    }
    else if (key == "yCoor") {
        yCoor = std::stod(value);
    }
    else if (key == "zCoor") {
        zCoor = std::stod(value);
    }
    else if (key == "phi") {
        phi = std::stod(value);
    }
    else if (key == "theta") {
        theta = std::stod(value);
    }
    else if (key == "startupOffset") {
        startupOffset = std::stod(value);
    }
    else if (key == "startupRandomization") {
        startupRandomization = std::stod(value);
    }
    else if (key == "ApplicationName") {
        ApplicationName = value;
    }
    else if (key == "MACProtocolName") {
        MACProtocolName = value;
    }
    else if (key == "RadioProtocolName") {
        RadioProtocolName = value;
    }
    else if (key == "RoutingProtocolName") {
        RoutingProtocolName = value;
    }
    else {
        return false;
    }

    NotifyAttributeChange(key, value);
    return true;
}

void Node::Build()
{
    // Implementation of the Build method
    WsnObject::Build();
}

} // namespace wsn
} // namespace ns3