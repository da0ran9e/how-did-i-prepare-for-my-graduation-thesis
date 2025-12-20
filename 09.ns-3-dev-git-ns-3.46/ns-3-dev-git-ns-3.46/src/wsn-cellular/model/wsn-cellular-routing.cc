#include "wsn-cellular-routing.h"
#include "wsn-cellular-forwarder.h"

#include "ns3/simulator.h"

namespace ns3 {
namespace wsncellular {

NS_LOG_COMPONENT_DEFINE("WsnCellularRouting");
NS_OBJECT_ENSURE_REGISTERED(WsnCellularRouting);

TypeId
WsnCellularRouting::GetTypeId()
{
  static TypeId tid = TypeId("ns3::wsn::WsnCellularRouting")
      .SetParent<Object>()
      .SetGroupName("Wsn")
      .AddConstructor<WsnCellularRouting>();
  return tid;
}

WsnCellularRouting::WsnCellularRouting() {}
WsnCellularRouting::~WsnCellularRouting() {}

void
WsnCellularRouting::SetNode(Ptr<Node> node)
{
  m_node = node;
}

void
WsnCellularRouting::SetForwarder(Ptr<WsnCellularForwarder> forwarder)
{
  m_forwarder = forwarder;
}

void
WsnCellularRouting::RouteInput(Ptr<Packet> packet,
                               const WsnCellularHeader &header)
{
  uint32_t myId = m_node->GetId();

  NS_LOG_INFO("[Routing] Node " << myId
              << " received pkt src=" << header.GetSrc()
              << " dst=" << header.GetDst()
              << " type=" << (uint32_t)header.GetType());

  if (header.GetDst() == myId ||
      header.GetDst() == 0xFFFF) // broadcast
  {
    HandleData(packet, header);
    return;
  }

  ForwardPacket(packet, header);
}

void
WsnCellularRouting::HandleData(Ptr<Packet> packet,
                               const WsnCellularHeader &header)
{
  NS_LOG_INFO("[Routing] Node " << m_node->GetId()
              << " DELIVER packet from " << header.GetSrc());

  // tictoc logic:
  if (header.GetType() == WsnCellularHeader::DATA)
  {
    Ptr<Packet> reply = Create<Packet>(packet->GetSize());

    WsnCellularHeader hdr;
    hdr.SetSrc(m_node->GetId());
    hdr.SetDst(header.GetSrc());
    hdr.SetType(WsnCellularHeader::DATA);

    reply->AddHeader(hdr);
    m_forwarder->Send(reply, Mac16Address::GetBroadcast());
  }
}

void
WsnCellularRouting::ForwardPacket(Ptr<Packet> packet,
                                   const WsnCellularHeader &header)
{
  NS_LOG_INFO("[Routing] Node " << m_node->GetId()
              << " FORWARD packet to dst=" << header.GetDst());

  m_forwarder->Send(packet, Mac16Address::GetBroadcast());
}

void
WsnCellularRouting::SendData(uint16_t dst, Ptr<Packet> payload)
{
  WsnCellularHeader hdr;
  hdr.SetSrc(m_node->GetId());
  hdr.SetDst(dst);
  hdr.SetType(WsnCellularHeader::DATA);

  payload->AddHeader(hdr);

  NS_LOG_INFO("[Routing] Node " << m_node->GetId()
              << " SEND data to " << dst);

  m_forwarder->Send(payload, Mac16Address::GetBroadcast());
}

} // namespace wsncellular
} // namespace ns3
