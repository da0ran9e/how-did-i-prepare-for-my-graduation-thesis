#ifndef WSN_ROUTING_TEST_PROTOCOL_H
#define WSN_ROUTING_TEST_PROTOCOL_H

// #include "ns3/ipv4-interface.h"
// #include "ns3/ipv4-l3-protocol.h"
// #include "ns3/ipv4-routing-protocol.h"
// #include "ns3/node.h"
// #include "ns3/output-stream-wrapper.h"
// #include "ns3/random-variable-stream.h"
#include <iostream>

namespace ns3
{

namespace wsn_routing_test
{

class RoutingProtocol : public Ipv4RoutingProtocol
{
    public:
        void setExampleValue(int v);
        int getExampleValue();
        RoutingProtocol();
        virtual ~RoutingProtocol();

    private:
        int m_exampleValue;
};

} // namespace wsn_routing_test
} // namespace ns3

#endif /* WSN_ROUTING_TEST_PROTOCOL_H */
