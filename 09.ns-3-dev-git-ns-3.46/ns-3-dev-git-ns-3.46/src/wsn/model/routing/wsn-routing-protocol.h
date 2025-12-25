#ifndef WSN_ROUTING_PROTOCOL_H
#define WSN_ROUTING_PROTOCOL_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/mac16-address.h"
#include "ns3/simulator.h"

#include "wsn-forwarder.h"

#include "wsn-routing-header.h"

namespace ns3 {
namespace wsn {

class WsnForwarder;

class WsnRoutingProtocol : public Object,
public ns3::wsn::WsnForwarder::Listener
{
public:
  static TypeId GetTypeId();

  WsnRoutingProtocol();
  virtual ~WsnRoutingProtocol();

  //void SetNode(Ptr<Node> node){ m_node = node; }
  void SetForwarder(Ptr<WsnForwarder> forwarder){ m_forwarder = forwarder; }

  void FromMacLayer(Ptr<Packet> pkt,
                    const uint16_t src) override;

private:
  void HandlePacket(Ptr<Packet> packet,
                  const WsnRoutingHeader &header);
private:
  Ptr<Node> m_node;
  Ptr<WsnForwarder> m_forwarder;
};

} // namespace wsn
} // namespace ns3

#endif // WSN_ROUTING_PROTOCOL_H
