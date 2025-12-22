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

  std::cout << "[Routing] Node " << myId
              << " received pkt src=" << header.GetSrc()
              << " dst=" << header.GetDst()
              << " type=" << (uint32_t)header.GetType() << std::endl;

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
  int srcNodeId = header.GetSrc();
  WsnCellularHeader::MsgType type = header.GetType();
  std::cout << "[Routing] Node " << m_node->GetId()
              << " DELIVER packet from " << srcNodeId << std::endl;

  // tictoc logic:
  if (type == WsnCellularHeader::DATA)
  {
    Ptr<Packet> reply = Create<Packet>(packet->GetSize());

    WsnCellularHeader hdr;
    hdr.SetSrc(m_node->GetId());
    hdr.SetDst(header.GetSrc());
    hdr.SetType(WsnCellularHeader::DATA);

    reply->AddHeader(hdr);
    m_forwarder->Send(reply, Mac16Address::GetBroadcast());
  }

  // if (type == TIC)
  // {
  //   NS_LOG_INFO("[Routing] Node " << m_nodeId
  //               << " replying TOC to node " << srcNodeId);

  //   Ptr<Packet> reply = Create<Packet>(10);
  //   SendData(srcNodeId, reply, TOC);
  // }
  // else if (type == TOC)
  // {
  //   NS_LOG_INFO("[Routing] Node " << m_nodeId
  //               << " received TOC from node " << srcNodeId);
  // }
}

void
WsnCellularRouting::ForwardPacket(Ptr<Packet> packet,
                                   const WsnCellularHeader &header)
{
  std::cout << "[Routing] Node " << m_node->GetId()
            << " FORWARD packet to dst=" << header.GetDst() << std::endl;

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

  Mac16Address nextHop = ResolveMacAddress(dst);

  std::cout << "[Routing] Node " << m_node->GetId()
            << " SEND data to node " << dst
            << " (MAC=" << nextHop << ")" << std::endl;

  m_forwarder->Send(payload, nextHop);
}

Mac16Address
WsnCellularRouting::ResolveMacAddress(uint16_t nodeId)
{
  return Mac16Address(nodeId + 1);
}


} // namespace wsncellular
} // namespace ns3
