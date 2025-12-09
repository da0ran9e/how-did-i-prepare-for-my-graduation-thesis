#pragma once
#ifndef WSN_ROUTING_H
#define WSN_ROUTING_H

#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/mac16-address.h"
#include "ns3/callback.h"
#include "ns3/event-id.h"

namespace ns3 {
namespace wsn {

class WsnRoutingProtocol : public Object
{
public:
    static TypeId GetTypeId();

    WsnRoutingProtocol();
    virtual ~WsnRoutingProtocol() override;
    virtual void DoDispose() override;
    
    void SetNetDevice(Ptr<NetDevice> dev);
    void Start();

private:
    void SendBeacon();
    bool ReceiveFromMac(Ptr<NetDevice> dev, Ptr<const Packet> p, uint16_t protocol,
                        const Address &from, const Address &to);
    Ptr<NetDevice> m_dev;
    EventId m_beaconEvent;
    Time m_initialBeaconTime = Seconds(1.0);
    uint32_t m_beaconSize = 10;
};

} // namespace wsn
} // namespace ns3

#endif // WSN_ROUTING_H
