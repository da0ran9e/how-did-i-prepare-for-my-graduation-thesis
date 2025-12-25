// wsn-forwarder.h
#ifndef WSN_FORWARDER_H
#define WSN_FORWARDER_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/log.h"

#include "wsn-routing-header.h"

namespace ns3 {
namespace wsn {

class WsnForwarder : public Object
{
public:
    static TypeId GetTypeId();

    WsnForwarder();
    ~WsnForwarder() override;

    void SetNetDevice(Ptr<NetDevice> dev);
    void ToMacLayer(Ptr<Packet> packet, const uint16_t dst);
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void FromMacLayer(Ptr<Packet> pkt, const uint16_t src) = 0;
    };
    Address ResolveMACAddress(uint16_t nodeId);
    void AddListener(Listener* listener);
private:
  bool ReceiveFromMac(Ptr<NetDevice> dev,
                      Ptr<const Packet> packet,
                      uint16_t protocol,
                      const Address& src);

private:
  Ptr<NetDevice> m_dev;
  std::vector<Listener*> m_listeners;
};

} // namespace wsn
} // namespace ns3

#endif // WSN_FORWARDER_H

