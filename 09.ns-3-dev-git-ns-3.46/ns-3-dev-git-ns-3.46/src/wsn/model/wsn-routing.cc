#include "wsn-routing.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include <iostream>

namespace ns3 {
namespace wsn {

NS_LOG_COMPONENT_DEFINE("WsnRouting");
NS_OBJECT_ENSURE_REGISTERED(WsnRoutingProtocol);

TypeId
WsnRoutingProtocol::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::wsn::WsnRoutingProtocol")
            .SetParent<Object>()
            .SetGroupName("Wsn");
    return tid;
}

WsnRoutingProtocol::WsnRoutingProtocol()
{
    NS_LOG_FUNCTION(this);
}

WsnRoutingProtocol::~WsnRoutingProtocol()
{
    NS_LOG_FUNCTION(this);
}

void
WsnRoutingProtocol::DoDispose()
{
    NS_LOG_FUNCTION(this);
    m_dev = nullptr;
    Object::DoDispose();
}

void
WsnRoutingProtocol::SetNetDevice(Ptr<NetDevice> dev)
{
    NS_LOG_FUNCTION(this << dev);
    m_dev = dev;

    // đăng ký callback nhận packet
    m_dev->SetReceiveCallback(
        MakeCallback(&WsnRoutingProtocol::ReceiveFromMac, this));
}

void
WsnRoutingProtocol::Start()
{
    NS_LOG_FUNCTION(this);

    NS_LOG_INFO("WSN Routing starting -> scheduling initial beacon at "
                << m_initialBeaconTime.GetSeconds() << "s");

    // gửi beacon một lần sau khi khởi động
    m_beaconEvent = Simulator::Schedule(
        m_initialBeaconTime,
        &WsnRoutingProtocol::SendBeacon,
        this);
}

void
WsnRoutingProtocol::SendBeacon()
{
    NS_LOG_FUNCTION(this);

    NS_LOG_INFO("WSN Routing sends BEACON from node "
                << m_dev->GetNode()->GetId());

    Ptr<Packet> p = Create<Packet>(m_beaconSize);

    // Broadcast address
    Mac16Address broadcast = Mac16Address("ff:ff");

    m_dev->Send(p, broadcast, 0); // protocol=0 (non-IP WSN)
}

bool
WsnRoutingProtocol::ReceiveFromMac(Ptr<NetDevice> dev,
                                   Ptr<const Packet> p,
                                   uint16_t protocol,
                                   const Address &from)
{
    NS_LOG_INFO("Node " << dev->GetNode()->GetId()
                        << " received packet size=" << p->GetSize());
    std::cout << "Node " << dev->GetNode()->GetId()
              << " received packet size=" << p->GetSize() << std::endl;
    return true;
}


} // namespace wsn
} // namespace ns3
