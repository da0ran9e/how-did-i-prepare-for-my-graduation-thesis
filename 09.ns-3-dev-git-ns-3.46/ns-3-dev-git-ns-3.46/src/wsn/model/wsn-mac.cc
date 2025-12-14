#include "wsn-mac.h"

namespace ns3 {
namespace wsn {
bool WsnMac::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "macMaxPacketSize") {
        macMaxPacketSize = std::stoi(value);
    }
    else if (key == "macBufferSize") {
        macBufferSize = std::stoi(value);
    }
    else if (key == "macPacketOverhead") {
        macPacketOverhead = std::stoi(value);
    }
    else {
        return false;
    }

    NotifyAttributeChange(key, value);
    return true;
}
void WsnMac::Build()
{
    // Implementation of the Build method
    WsnObject::Build();
}

} // namespace wsn
} // namespace ns3