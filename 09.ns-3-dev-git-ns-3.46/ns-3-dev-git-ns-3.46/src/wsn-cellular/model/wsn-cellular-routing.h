#ifndef WSN_CELLULAR_ROUTING_H
#define WSN_CELLULAR_ROUTING_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/mac16-address.h"

#include "wsn-cellular-forwarder.h"

#include "wsn-cellular-header.h"

namespace ns3 {
namespace wsncellular {

class WsnCellularForwarder;

class WsnCellularRouting : public Object,
public ns3::wsncellular::WsnCellularForwarder::Listener
{
public:
  static TypeId GetTypeId();

  WsnCellularRouting();
  virtual ~WsnCellularRouting();

  void SetNode(Ptr<Node> node);
  void SetForwarder(Ptr<WsnCellularForwarder> forwarder);

  /**
   * Entry point from forwarder
   */
  void RouteInput(Ptr<Packet> packet,
                  const WsnCellularHeader &header);

  /**
   * Called by application
   */
  void SendData(uint16_t dst, Ptr<Packet> payload);
  void FromMacLayer(Ptr<Packet> pkt,
                    const uint16_t src) override;
  void ToMacLayer(Ptr<Packet> pkt,
                    const uint16_t dst);
                    
private:
  void HandleData(Ptr<Packet> packet,
                  const WsnCellularHeader &header);

  void ForwardPacket(Ptr<Packet> packet,
                     const WsnCellularHeader &header);

  Mac16Address ResolveMacAddress(uint16_t nodeId);

private:
  Ptr<Node> m_node;
  Ptr<WsnCellularForwarder> m_forwarder;
};

} // namespace wsncellular
} // namespace ns3

#endif // WSN_CELLULAR_ROUTING_H
