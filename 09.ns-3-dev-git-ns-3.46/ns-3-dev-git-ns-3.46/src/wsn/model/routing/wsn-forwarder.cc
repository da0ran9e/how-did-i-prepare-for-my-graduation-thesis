#include "wsn-forwarder.h"
#include "ns3/node.h"
#include "ns3/mac16-address.h"

namespace ns3 {
namespace wsn {

NS_LOG_COMPONENT_DEFINE("WsnForwarder");
NS_OBJECT_ENSURE_REGISTERED(WsnForwarder);

TypeId
WsnForwarder::GetTypeId()
{
  static TypeId tid = TypeId("ns3::wsn::WsnForwarder")
    .SetParent<Object>()
    .SetGroupName("Wsn")
    .AddConstructor<ns3::wsn::WsnForwarder>();
  return tid;
}

WsnForwarder::WsnForwarder() {}
WsnForwarder::~WsnForwarder() {}

void
WsnForwarder::SetNetDevice(Ptr<NetDevice> dev)
{
  m_dev = dev;
  m_dev->SetReceiveCallback(
    MakeCallback(&WsnForwarder::ReceiveFromMac, this));
}

void
WsnForwarder::ToMacLayer(Ptr<Packet> packet, const uint16_t dst)
{
  NS_ASSERT(m_dev);
  m_dev->Send(packet, ResolveMACAddress(dst), 0);
}

bool
WsnForwarder::ReceiveFromMac(Ptr<NetDevice> dev,
                                     Ptr<const Packet> pkt,
                                     uint16_t,
                                     const Address& src)
{
  WsnRoutingHeader hdr;
  pkt->PeekHeader(hdr);
  for (auto l : m_listeners)
  {
    l->FromMacLayer(pkt->Copy(), hdr.GetSource());
    // std::cout << "[Forwarder] Node received pkt from node " << hdr.GetSrc()
    //           << " type=" << (uint32_t)hdr.GetType() << " seq " << std::endl;
  }

  return true;
}

Address
WsnForwarder::ResolveMACAddress(uint16_t nodeId)
{
  return Mac16Address(nodeId+1);
}

void
WsnForwarder::AddListener(Listener* listener)
{
  NS_ASSERT(listener);
  m_listeners.push_back(listener);
}


} // namespace wsncellular
} // namespace ns3
