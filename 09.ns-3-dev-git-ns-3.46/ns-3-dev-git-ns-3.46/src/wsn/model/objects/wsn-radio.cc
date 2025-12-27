#include "wsn-radio.h"
#include "wsn-node.h"

namespace ns3 {
namespace wsn {
bool Radio::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "radioTxPowerDbm") {
        radioTxPowerDbm = std::stod(value);
    }
    else if (key == "radioRxSensitivityDbm") {
        radioRxSensitivityDbm = std::stod(value);
    }
    else if (key == "radioChannelBandwidthKbps") {
        radioChannelBandwidthKbps = std::stod(value);
    }
    else if (key == "radioHeaderOverhead") {
        radioHeaderOverhead = std::stoi(value);     
    }
    else {
        return false;
    }   
    NotifyAttributeChanged(key, value);
    return true;
}

void Radio::Build(BuildContext& ctx)
{
    if(m_built) {
        return;
    }
    m_built = true;
    
    std::cout << "Building Radio: " << GetInstanceName() << std::endl;
    // Implementation of the Build method
    // WsnObject::Build(ctx);
    ctx.mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    ctx.mobility.Install(ctx.nodes); 

    ns3::LrWpanHelper lrwpan;
    lrwpan.SetChannel(ctx.spectrumChannel);
    if (!ctx.netDevInitialized) {
        ctx.netDevices = lrwpan.Install(ctx.nodes);
        ctx.netDevInitialized = true;
    }
    int nodeAddr = ctx.initializedDev++;
    m_selfDetDevice = ctx.netDevices.Get(nodeAddr);
    std::cout << "Radio built for Node " << nodeAddr << " with NetDevice ID " << m_selfDetDevice->GetIfIndex() << std::endl;
}

} // namespace wsn
} // namespace ns3