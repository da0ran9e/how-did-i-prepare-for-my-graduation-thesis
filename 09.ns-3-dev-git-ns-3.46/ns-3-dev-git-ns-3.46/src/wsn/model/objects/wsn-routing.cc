#include "wsn-routing.h"

namespace ns3 {
namespace wsn {

bool WsnRouting::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "collectTraceInfo") {
        collectTraceInfo = (value == "true" || value == "1");
    }
    else if (key == "maxNetFrameSize") {
        maxNetFrameSize = std::stoi(value);
    }
    else if (key == "netDataFrameOverhead") {
        netDataFrameOverhead = std::stoi(value);
    }
    else if (key == "netBufferSize") {
        netBufferSize = std::stoi(value);
    }
    else {
        return false;
    }

    NotifyAttributeChanged(key, value);
    return true;
}

void WsnRouting::Build()
{
    // Implementation of the Build method
    WsnObject::Build();
}


} // namespace wsn
} // namespace ns3
