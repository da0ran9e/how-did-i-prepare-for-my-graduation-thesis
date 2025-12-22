#include "wsn-cellular-forwarder.h"
#include "ns3/node.h"
#include "ns3/mac16-address.h"

namespace ns3 {
namespace wsncellular {

NS_LOG_COMPONENT_DEFINE("WsnCellularForwarder");
NS_OBJECT_ENSURE_REGISTERED(WsnCellularForwarder);

TypeId
WsnCellularForwarder::GetTypeId()
{
  static TypeId tid = TypeId("ns3::wsn::WsnCellularForwarder")
    .SetParent<Object>()
    .SetGroupName("Wsn")
    .AddConstructor<ns3::wsncellular::WsnCellularForwarder>();
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
  uint32_t srcNodeId = hdr.GetSrc();
  uint32_t dstNodeId = hdr.GetDst();
  uint32_t seq = hdr.GetSeq();
  uint32_t type = hdr.GetMsgType();

  if (type == WsnCellularHeader::BEACON)
  {
    // Send data
    hdr.SetSrc(nodeId);
    hdr.SetDst(srcNodeId);
    hdr.SetSeq(seq + 1);
    hdr.SetMsgType(WsnCellularHeader::DATA);
    Ptr<Packet> payload = Create<Packet>(100); // dummy payload
    payload->AddHeader(hdr);
    Send(payload, Mac16Address(srcNodeId+1));
  } else {
    // beacon
    hdr.SetSrc(nodeId);
    hdr.SetDst(srcNodeId);
    hdr.SetSeq(seq + 1);
    hdr.SetMsgType(WsnCellularHeader::BEACON);
    Ptr<Packet> payload = Create<Packet>(50); // dummy payload
    payload->AddHeader(hdr);
    Send(payload, Mac16Address(srcNodeId+1));
  }

  std::cout << "[Forwarder] Node " << nodeId
            << " received packet type=" << uint32_t(hdr.GetMsgType())
            << " seq=" << hdr.GetSeq()
            << " from srcNode=" << srcNodeId << std::endl;

  // Forwarding / routing decision will be added here later
}

void
WsnCellularForwarder::SetRouting(Ptr<ns3::Object> routing)
{
  m_routing = routing;
}


} // namespace wsncellular
} // namespace ns3
