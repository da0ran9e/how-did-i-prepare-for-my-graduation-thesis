#pragma once
#ifndef WSN_ROUTING_H
#define WSN_ROUTING_H

#include "ns3/object.h"
#include "ns3/type-id.h"

namespace ns3 {
namespace wsn {
class WsnRoutingProtocol : public Object
{
public:
    static TypeId GetTypeId();
    WsnRoutingProtocol();
    void DoDispose();

    // TODO: Send(), Receive(), Forward(), Table, ...
};
} // namespace wsn
} // namespace ns3

#endif // WSN_ROUTING_H