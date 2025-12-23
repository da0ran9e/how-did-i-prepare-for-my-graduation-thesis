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
                                     Ptr<const Packet> pkt,
                                     uint16_t,
                                     const Address& src)
{
  WsnCellularHeader hdr;
  pkt->PeekHeader(hdr);
  for (auto l : m_listeners)
  {
    l->FromMacLayer(pkt->Copy(), hdr.GetSrc());
    // std::cout << "[Forwarder] Node received pkt from node " << hdr.GetSrc()
    //           << " type=" << (uint32_t)hdr.GetType() << " seq " << std::endl;
  }

  return true;
}

void
WsnCellularForwarder::SetRouting(Ptr<ns3::Object> routing)
{
  m_routing = routing;
}

Address
WsnCellularForwarder::ResolveMACAddress(uint16_t nodeId)
{
  return Mac16Address(nodeId+1);
}

void
WsnCellularForwarder::AddListener(Listener* listener)
{
  NS_ASSERT(listener);
  m_listeners.push_back(listener);
}


} // namespace wsncellular
} // namespace ns3
