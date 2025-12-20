#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/spectrum-module.h"

#include "ns3/wsn-cellular-routing.h"
#include "ns3/wsn-cellular-forwarder.h"

using namespace ns3;
using namespace ns3::wsn-cellular;

NS_LOG_COMPONENT_DEFINE("WsnCellularTicTocExample");

int
main(int argc, char *argv[])
{
  LogComponentEnable("WsnCellularTicTocExample", LOG_LEVEL_INFO);
  LogComponentEnable("WsnCellularRouting", LOG_LEVEL_INFO);
  LogComponentEnable("WsnCellularForwarder", LOG_LEVEL_INFO);

  // -----------------------------
  // 1. Create nodes
  // -----------------------------
  NodeContainer nodes;
  nodes.Create(2);

  // -----------------------------
  // 2. Mobility (fixed)
  // -----------------------------
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  nodes.Get(0)->GetObject<MobilityModel>()->SetPosition(Vector(0, 0, 0));
  nodes.Get(1)->GetObject<MobilityModel>()->SetPosition(Vector(10, 0, 0));

  // -----------------------------
  // 3. Channel
  // -----------------------------
  Ptr<SingleModelSpectrumChannel> channel =
      CreateObject<SingleModelSpectrumChannel>();

  Ptr<LogDistancePropagationLossModel> loss =
      CreateObject<LogDistancePropagationLossModel>();
  Ptr<ConstantSpeedPropagationDelayModel> delay =
      CreateObject<ConstantSpeedPropagationDelayModel>();

  channel->AddPropagationLossModel(loss);
  channel->SetPropagationDelayModel(delay);

  // -----------------------------
  // 4. LR-WPAN
  // -----------------------------
  LrWpanHelper lrwpan;
  lrwpan.SetChannel(channel);

  NetDeviceContainer devices = lrwpan.Install(nodes);

  for (uint32_t i = 0; i < devices.GetN(); ++i)
  {
    Ptr<lrwpan::LrWpanNetDevice> dev =
        DynamicCast<lrwpan::LrWpanNetDevice>(devices.Get(i));
    dev->GetMac()->SetShortAddress(Mac16Address::Allocate());
  }

  // -----------------------------
  // 5. Install WSN stack
  // -----------------------------
  for (uint32_t i = 0; i < nodes.GetN(); ++i)
  {
    Ptr<Node> node = nodes.Get(i);
    Ptr<NetDevice> dev = devices.Get(i);

    Ptr<WsnCellularForwarder> forwarder =
        CreateObject<WsnCellularForwarder>();
    Ptr<WsnCellularRouting> routing =
        CreateObject<WsnCellularRouting>();

    routing->SetNode(node);
    routing->SetForwarder(forwarder);

    forwarder->SetNetDevice(dev);
    forwarder->SetRouting(routing);

    node->AggregateObject(forwarder);
    node->AggregateObject(routing);
  }

  // -----------------------------
  // 6. Tic → Toc
  // -----------------------------
  Simulator::Schedule(Seconds(1.0), [&]() {
    NS_LOG_INFO("Node 0 sends TIC");
    Ptr<Packet> p = Create<Packet>(20);

    Ptr<WsnCellularRouting> routing0 =
        nodes.Get(0)->GetObject<WsnCellularRouting>();

    routing0->SendData(1 /* dst node id */, p);
  });

  Simulator::Stop(Seconds(5.0));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
