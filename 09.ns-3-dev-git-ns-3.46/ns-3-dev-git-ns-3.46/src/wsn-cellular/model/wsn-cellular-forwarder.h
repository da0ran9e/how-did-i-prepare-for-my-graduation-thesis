// wsn-cellular-forwarder.h
#ifndef WSN_CELLULAR_FORWARDER_H
#define WSN_CELLULAR_FORWARDER_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/log.h"

#include "wsn-cellular-header.h"

namespace ns3 {
namespace wsncellular {

class WsnCellularForwarder : public Object
{
public:
  static TypeId GetTypeId();

  WsnCellularForwarder();
  ~WsnCellularForwarder() override;

  void SetNetDevice(Ptr<NetDevice> dev);
  void Send(Ptr<Packet> packet, const Address& dst);

  void SetRouting(Ptr<ns3::Object> routing);
  class Listener
  {
  public:
    virtual ~Listener() {}
    virtual void FromMacLayer(Ptr<Packet> pkt,
                              const uint16_t src) = 0;
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
  Ptr<ns3::Object> m_routing;
  std::vector<Listener*> m_listeners;
};

} // namespace wsncellular
} // namespace ns3

#endif // WSN_CELLULAR_FORWARDER_H

