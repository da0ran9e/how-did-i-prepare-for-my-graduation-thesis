
#include "ns3/node.h"

namespace ns3 {
namespace wsn-cellular {

NS_LOG_COMPONENT_DEFINE("WsnCellularForwarder");
NS_OBJECT_ENSURE_REGISTERED(WsnCellularForwarder);

TypeId
WsnCellularForwarder::GetTypeId()
{
  static TypeId tid = TypeId("ns3::wsn::WsnCellularForwarder")
    .SetParent<Object>()
    .SetGroupName("Wsn")
    .AddConstructor<WsnCellularForwarder>();
  return tid;
}

WsnCellularForwarder::WsnCellularForwarder() {}

WsnCellularForwarder::~WsnCellularForwarder() {}

void
WsnCellularForwarder::SetNetDevice(Ptr<NetDevice> dev)
{
  m_dev = dev;
  m_dev->SetReceiveCallback(
    MakeCallback(&WsnCellularForwarder::ReceiveFromMac, this));
}

void
WsnCellularForwarder::Send(Ptr<Packet> packet, const Address& dst)
{
  NS_ASSERT(m_dev);
  m_dev->Send(packet, dst, 0);
}

bool
WsnCellularForwarder::ReceiveFromMac(Ptr<NetDevice> dev,
                                     Ptr<const Packet> packet,
                                     uint16_t protocol,
                                     const Address& src)
{
  Ptr<Packet> p = packet->Copy();
  HandlePacket(p, src);
  return true;
}

void
WsnCellularForwarder::HandlePacket(Ptr<Packet> packet, const Address& src)
{
  WsnCellularHeader hdr;
  packet->RemoveHeader(hdr);

  Ptr<Node> node = m_dev->GetNode();
  uint32_t nodeId = node ? node->GetId() : 0xffffffff;

  NS_LOG_INFO("Node " << nodeId
                << " received packet type=" << uint32_t(hdr.GetMsgType())
                << " seq=" << hdr.GetSeq()
                << " from srcNode=" << hdr.GetSrcNodeId());

  // Forwarding / routing decision will be added here later
}

} // namespace wsn-cellular
} // namespace ns3
