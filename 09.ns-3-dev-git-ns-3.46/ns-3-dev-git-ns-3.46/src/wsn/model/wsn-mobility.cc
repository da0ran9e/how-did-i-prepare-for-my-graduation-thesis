#include "wsn-mobility.h"

namespace ns3 {
namespace wsn {
bool Mobility::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "updateInterval") {
        updateInterval = std::stod(value);
    }
    else if (key == "xCoorDestination") {
        xCoorDestination = std::stod(value);
    }
    else if (key == "yCoorDestination") {
        yCoorDestination = std::stod(value);
    }
    else if (key == "zCoorDestination") {
        zCoorDestination = std::stod(value);
    }
    else if (key == "speed") {
        speed = std::stod(value);
    }
    else {
        return false;
    }

    NotifyAttributeChange(key, value);
    return true;
}

void Mobility::Build()
{
    // Implementation of the Build method
    WsnObject::Build();
}
} // namespace wsn
} // namespace ns3