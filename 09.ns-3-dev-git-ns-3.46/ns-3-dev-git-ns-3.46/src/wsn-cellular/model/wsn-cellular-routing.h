#ifndef WSN_CELLULAR_ROUTING_H
#define WSN_CELLULAR_ROUTING_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"

#include "wsn-cellular-header.h"

namespace ns3 {
namespace wsn-cellular {

class WsnCellularForwarder;

class WsnCellularRouting : public Object
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

private:
  void HandleData(Ptr<Packet> packet,
                  const WsnCellularHeader &header);

  void ForwardPacket(Ptr<Packet> packet,
                     const WsnCellularHeader &header);

private:
  Ptr<Node> m_node;
  Ptr<WsnCellularForwarder> m_forwarder;
};

} // namespace wsn-cellular
} // namespace ns3

#endif // WSN_CELLULAR_ROUTING_H
