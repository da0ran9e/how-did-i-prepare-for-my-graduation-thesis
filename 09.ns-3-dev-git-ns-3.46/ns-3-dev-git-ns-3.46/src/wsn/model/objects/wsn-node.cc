#include "wsn-node.h"
namespace ns3 {
namespace wsn {

bool Node::SetProperty(const std::string &key, const std::string &value)
{
    if (key == "nodeAddr") {
        nodeAddr = static_cast<uint16_t>(std::stoi(value));
    }
    else if (key == "xCoor") {
        xCoor = std::stod(value);
    }
    else if (key == "yCoor") {
        yCoor = std::stod(value);
    }
    else if (key == "zCoor") {
        zCoor = std::stod(value);
    }
    else if (key == "phi") {
        phi = std::stod(value);
    }
    else if (key == "theta") {
        theta = std::stod(value);
    }
    else if (key == "startupOffset") {
        startupOffset = std::stod(value);
    }
    else if (key == "startupRandomization") {
        startupRandomization = std::stod(value);
    }
    else if (key == "ApplicationName") {
        ApplicationName = value;
    }
    else if (key == "MACProtocolName") {
        MACProtocolName = value;
    }
    else if (key == "RadioProtocolName") {
        RadioProtocolName = value;
    }
    else if (key == "RoutingProtocolName") {
        RoutingProtocolName = value;
    }
    else {
        return false;
    }

    NotifyAttributeChanged(key, value);
    return true;
}

void Node::Build(BuildContext& ctx)
{
    std::cout << "Building node: " << GetAddr() << std::endl;
    

    // // 2. Gắn mobility (tọa độ)
    // MobilityHelper mobility;
    // mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    // mobility.Install(node);

    // Ptr<MobilityModel> mob = node->GetObject<MobilityModel>();
    // mob->SetPosition(Vector(xCoor, yCoor, zCoor));

    // // 3. Lưu runtime object (rất quan trọng)
    // m_runtimeNode = node;

    // // 4. Add vào container chung
    // ctx.nodes.Add(node);

    // NS_LOG_INFO("Node built at position ("
    //             << xCoor << ", "
    //             << yCoor << ", "
    //             << zCoor << ")");
}

} // namespace wsn
} // namespace ns3