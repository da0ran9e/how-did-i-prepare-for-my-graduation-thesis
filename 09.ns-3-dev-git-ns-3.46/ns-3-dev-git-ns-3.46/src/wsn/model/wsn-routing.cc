#include "wsn-routing.h"

namespace ns3 {
namespace wsn {

NS_OBJECT_ENSURE_REGISTERED(WsnRoutingProtocol);

TypeId
WsnRoutingProtocol::GetTypeId()
{
    static TypeId tid = TypeId("ns3::wsn::WsnRoutingProtocol")
        .SetParent<Object>()
        .SetGroupName("Wsn");
    return tid;
}

WsnRoutingProtocol::WsnRoutingProtocol() {}
void WsnRoutingProtocol::DoDispose() { Object::DoDispose(); }


} // namespace wsn
} // namespace ns3