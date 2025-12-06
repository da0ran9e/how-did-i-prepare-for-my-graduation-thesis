#include "wsn-routing-test-protocol.h"

// #include "ns3/adhoc-wifi-mac.h"
// #include "ns3/boolean.h"
// #include "ns3/inet-socket-address.h"
// #include "ns3/log.h"
// #include "ns3/pointer.h"
// #include "ns3/random-variable-stream.h"
// #include "ns3/string.h"
// #include "ns3/trace-source-accessor.h"
// #include "ns3/udp-header.h"
// #include "ns3/udp-l4-protocol.h"
// #include "ns3/udp-socket-factory.h"
// #include "ns3/wifi-mpdu.h"
// #include "ns3/wifi-net-device.h"

// #undef NS_LOG_APPEND_CONTEXT
// #define NS_LOG_APPEND_CONTEXT                                                                      \
//     if (m_ipv4)                                                                                    \
//     {                                                                                              \
//         std::clog << "[node " << m_ipv4->GetObject<Node>()->GetId() << "] ";                       \
//     }

namespace ns3
{

namespace wsn_routing_test
{
NS_OBJECT_ENSURE_REGISTERED(RoutingProtocol);
RoutingProtocol::RoutingProtocol()
    : m_exampleValue(10)
{
    std::cout << "RoutingProtocol created." << std::endl;
}
RoutingProtocol::~RoutingProtocol()
{
    std::cout << "RoutingProtocol destroyed." << std::endl;
}
void RoutingProtocol::setExampleValue(int v)
{
    m_exampleValue = v;
    std::cout << "Example value set to: " << m_exampleValue << std::endl;
}
int RoutingProtocol::getExampleValue()
{
    std::cout << "Example value retrieved: " << m_exampleValue << std::endl;
    return m_exampleValue;
}

} // namespace wsn_routing_test
} // namespace ns3
