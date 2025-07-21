#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

//hard-coded
static std::map<int, Point> allNodesPositions;
static std::vector<int> clusterHeadList;
const std::string networkLayoutData = R"(
SN.node[0].Communication.Routing.isSink = true
SN.node[0].xCoor = 18
SN.node[0].yCoor = 17
SN.node[1].xCoor = 2
SN.node[1].yCoor = 3
SN.node[2].xCoor = 1
SN.node[2].yCoor = 52
SN.node[3].xCoor = 2
SN.node[3].yCoor = 101
SN.node[4].xCoor = 1
SN.node[4].yCoor = 150
SN.node[5].xCoor = 49
SN.node[5].yCoor = 0
SN.node[6].xCoor = 50
SN.node[6].yCoor = 48
SN.node[7].xCoor = 52
SN.node[7].yCoor = 101
SN.node[8].xCoor = 50
SN.node[8].yCoor = 150
SN.node[9].xCoor = 100
SN.node[9].yCoor = 4
SN.node[10].xCoor = 101
SN.node[10].yCoor = 50
SN.node[11].xCoor = 102
SN.node[11].yCoor = 101
SN.node[12].xCoor = 109
SN.node[12].yCoor = 150
SN.node[13].xCoor = 150
SN.node[13].yCoor = 4
SN.node[14].xCoor = 151
SN.node[14].yCoor = 50
SN.node[15].xCoor = 150
SN.node[15].yCoor = 104
SN.node[16].xCoor = 151
SN.node[16].yCoor = 150
)";


void CellularRouting::startup()
{
    helloInterval = par("helloInterval");
    cellRadius = par("cellRadius");
    myCL_id = -1;

// Read parameters from the network layout data
    parseNetworkLayout();

    // Pos
    if (allNodesPositions.count(self)) {
        myX = allNodesPositions[self].x;
        myY = allNodesPositions[self].y;
    } else {
        throw cRuntimeError("Node %d not found in network layout!", self);
    }

    // Role
    amI_CH = false;
    for (int ch_id : clusterHeadList) {
        if (ch_id == self) {
            amI_CH = true;
            break;
        }
    }

    myRole = amI_CH ? CLUSTER_HEAD : NORMAL_NODE;

    calculateCellInfo();
    trace() << "Node " << self << " initialized at (" << myX << ", " << myY
            << "). Cell ID: " << myCellId << ", Color: " << myColor
            << ", Is CH: " << (amI_CH ? "Yes" : "No");

    setTimer(SEND_HELLO_TIMER, uniform(0, helloInterval));
    setTimer(RECONFIGURATION_TIMER, 500);
}

void CellularRouting::timerFiredCallback(int index)
{
    switch (index) {
        case SEND_HELLO_TIMER: {
            trace() << "Timer SEND_HELLO_TIMER fired.";
            sendHelloPacket();
            setTimer(SEND_HELLO_TIMER, helloInterval);
            break;
        }

        case RECONFIGURATION_TIMER: {
            cancelTimer(SEND_HELLO_TIMER);
            trace() << "Timer RECONFIGURATION_TIMER fired.";
            startCLElectionContention();
            break;
        }

        case CL_ANNOUNCEMENT_TIMER: {
            trace() << "My CL announcement timer fired. I AM THE NEW CELL LEADER for cell " << myCellId;

            myRole = CELL_LEADER;
            amI_CL = true;
            myCL_id = self;

            sendCLAnnouncement();

            //startReconfiguration();
            break;
        }

        case LINK_REQUEST_TIMEOUT: {
            trace() << "WARNING: Link Request timed out. Timer index: " << index;

            if (pendingLinkRequests.count(index)) {
                LinkRequestState failed_request = pendingLinkRequests[index];
                trace() << "  -> Failed to establish link from CGW " << failed_request.source_cgw_id
                        << " to NGW " << failed_request.target_ngw_id;

                pendingLinkRequests.erase(index);

                // TODO: ...
            }

            if (amI_CL && pendingLinkRequests.empty()) {
                calculateAndDistributeIntraCellTree();
            }
            break;
        }

        default: {
            trace() << "WARNING: Unknown timer with index " << index << " fired.";
            break;
        }
    }
}

void CellularRouting::fromApplicationLayer(cPacket* pkt, const char* destination)
{
    CellularRoutingPacket* netPacket = new CellularRoutingPacket("CellularRouting data packet", NETWORK_LAYER_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    encapsulatePacket(netPacket, pkt);
    trace() << "fromApplicationLayer received";
    // TODO...

    toMacLayer(netPacket, resolveNetworkAddress(destination));
}

void CellularRouting::fromMacLayer(cPacket* pkt, int macAddress, double rssi, double lqi)
{
    trace() << "Packet received";
    CellularRoutingPacket* netPacket = dynamic_cast<CellularRoutingPacket*>(pkt);
    if (!netPacket) {
        return;
    }
    trace() << "Packet received from MAC layer with type " << netPacket->getPacketType();
    switch (netPacket->getPacketType()) {
        case HELLO_PACKET: {
            handleHelloPacket(netPacket);
            trace() << "Received HELLO packet from " << netPacket->getSource();
            break;
        }
        case CL_ANNOUNCEMENT: {
            handleCLAnnouncementPacket(netPacket);
            trace() << "Received CL_ANNOUNCEMENT packet from " << netPacket->getSource();
            break;
        }
        case BS_UPDATE_PACKET: {
            // TODO
            trace() << "Received BS_UPDATE_PACKET from " << netPacket->getSource();
            break;
        }
        case LINK_REQUEST: {
            //handleLinkRequest(netPacket);
            trace() << "Received LINK_REQUEST from " << netPacket->getSource();
            break;
        }
        case LINK_ACK: {
            //handleLinkAck(netPacket);
            trace() << "Received LINK_ACK from " << netPacket->getSource();
            break;
        }
        case LINK_ESTABLISHED: {
            //handleLinkEstablishedConfirmation(netPacket);
            trace() << "Received LINK_ESTABLISHED from " << netPacket->getSource();
            break;
        }
        case INTRA_CELL_ROUTING_UPDATE: {
            //handleRoutingTableUpdate(netPacket);
            trace() << "Received INTRA_CELL_ROUTING_UPDATE from " << netPacket->getSource();
            break;
        }
        default: {
            trace() << "WARNING: Received unknown packet type.";
            break;
        }
    }
}

void CellularRouting::parseNetworkLayout() {
    if (!allNodesPositions.empty()) {
        return;
    }

    std::stringstream stream(networkLayoutData);
    std::string line;
    int currentNodeId = -1;

    while (std::getline(stream, line)) {
        if (line.find("SN.node[") != std::string::npos) {
            size_t start_pos = line.find("[");
            size_t end_pos = line.find("]");
            std::string id_str = line.substr(start_pos + 1, end_pos - start_pos - 1);
            currentNodeId = std::stoi(id_str);
        }

        if (currentNodeId == -1) continue;

        if (line.find("isSink = true") != std::string::npos || line.find("isCH = true") != std::string::npos) {
            clusterHeadList.push_back(currentNodeId);
        } else if (line.find("xCoor") != std::string::npos) {
            size_t eq_pos = line.find("=");
            allNodesPositions[currentNodeId].x = std::stod(line.substr(eq_pos + 1));
        } else if (line.find("yCoor") != std::string::npos) {
            size_t eq_pos = line.find("=");
            allNodesPositions[currentNodeId].y = std::stod(line.substr(eq_pos + 1));
        }
    }
    EV << "Parsed network layout. Found " << allNodesPositions.size() << " nodes and " << clusterHeadList.size() << " CHs.\n";
}

void CellularRouting::calculateCellInfo() {
    double frac_q = (sqrt(3.0)/3.0 * myX - 1.0/3.0 * myY) / cellRadius;
    double frac_r = (2.0/3.0 * myY) / cellRadius;
    double frac_s = -frac_q - frac_r;

    int q = round(frac_q);
    int r = round(frac_r);
    int s = round(frac_s);

    double q_diff = abs(q - frac_q);
    double r_diff = abs(r - frac_r);
    double s_diff = abs(s - frac_s);

    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    } else {
        s = -q - r;
    }

    const int grid_offset = 10000;
    myCellId = q + r * grid_offset;
    myColor = ((q - r) % 3 + 3) % 3;
}

void CellularRouting::startCLElectionContention() {
    if (amI_CL) {
        return;
    }

    Point cellCenter = calculateCellCenter(myCellId);
    double myDistanceToCenter = sqrt(pow(myX - cellCenter.x, 2) + pow(myY - cellCenter.y, 2));

    double contentionScalingFactor = 0.005;
    simtime_t contentionDelay = (contentionScalingFactor * myDistanceToCenter) + uniform(0, 0.001);

    setTimer(CL_ANNOUNCEMENT_TIMER, myDistanceToCenter+100);

    trace() << "Starting CL election contention. My distance to center is " << myDistanceToCenter
            << "m. Will announce myself as CL in " << contentionDelay << "s.";
}

Point CellularRouting::calculateCellCenter(int cell_id) {

    const int grid_offset = 10000;

    int r = cell_id / grid_offset;
    int q = cell_id % grid_offset;

    if (cell_id < 0 && q > 0) {
        q -= grid_offset;
        r += 1;
    }
    if (cell_id > 0 && q < 0) {
        q += grid_offset;
        r -= 1;
    }

    Point center;
    center.x = cellRadius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r);
    center.y = cellRadius * (3.0 / 2.0 * r);

    return center;
}

void CellularRouting::sendHelloPacket() {
    trace() << "Function sendHelloPacket() called.";
        CellularRoutingPacket *netPacket = new CellularRoutingPacket("CellularRouting packet", NETWORK_LAYER_PACKET);
        netPacket->setPacketType(HELLO_PACKET);
        netPacket->setSource(SELF_NETWORK_ADDRESS);
        netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
        toMacLayer(netPacket, resolveNetworkAddress(BROADCAST_NETWORK_ADDRESS));
}

void CellularRouting::handleHelloPacket(CellularRoutingPacket* pkt) {
    int sourceId = atoi(pkt->getSource());
    trace() << "Received HELLO from " << sourceId << ". Triggering neighbor table update.";

    neighborTable.clear();

    double communicationRadius = 2.0 * cellRadius;

    for (auto const& [node_id, position] : allNodesPositions) {
        if (node_id == self) {
            continue;
        }

        double distance = sqrt(pow(myX - position.x, 2) + pow(myY - position.y, 2));

        if (distance <= communicationRadius) {
            NeighborRecord newNeighbor;
            newNeighbor.id = node_id;
            newNeighbor.x = position.x;
            newNeighbor.y = position.y;
            // newNeighbor.cellId = ...;
            newNeighbor.lastHeard = simTime();

            neighborTable.push_back(newNeighbor);
        }
    }

    trace() << "Neighbor table updated. Found " << neighborTable.size() << " neighbors.";
}

void CellularRouting::sendCLAnnouncement() {
    CellularRoutingPacket* pkt = new CellularRoutingPacket("CL Announcement", NETWORK_LAYER_PACKET);
    pkt->setPacketType(CL_ANNOUNCEMENT);
    CLAnnouncementInfo newCLAnnouncementInfo;
    newCLAnnouncementInfo.x = myX;
    newCLAnnouncementInfo.y = myY;
    newCLAnnouncementInfo.cellId = myCellId;
    pkt->setClAnnouncementData(newCLAnnouncementInfo);
    for (const auto& neighbor : neighborTable) {
           int neighborId = neighbor.id;
           CellularRoutingPacket* pkt_for_neighbor = pkt->dup();
           pkt_for_neighbor->setSource(SELF_NETWORK_ADDRESS);
           std::stringstream dest_addr;
           dest_addr << neighborId;
           pkt_for_neighbor->setDestination(dest_addr.str().c_str());
           toMacLayer(pkt_for_neighbor, neighborId);
       }
}

void CellularRouting::runCLElection() {
    trace() << "Function runCLElection() called.";
    // TODO:
    // 1. Duyệt qua neighborTable
    // 2. Tìm node có năng lượng cao nhất (hoặc tiêu chí khác)
    // 3. Nếu là mình, tự nhận vai trò CL và gọi send(CL_ANNOUNCEMENT)
}

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt) {
    int announcerId = atoi(pkt->getSource());
    double announcerX = pkt->getClAnnouncementData().x;
    double announcerY = pkt->getClAnnouncementData().y;
    Point cellCenter = calculateCellCenter(pkt->getClAnnouncementData().cellId);
    double announcerDistance = sqrt(pow(announcerX - cellCenter.x, 2) + pow(announcerY - cellCenter.y, 2));
    trace() << "Received CL announcement from " << announcerId
              << " at (" << announcerX << ", " << announcerY << ") with distance to center: "
              << announcerDistance;
    if (myCL_id != -1) {
           return;
    }

    double myDistanceToCenter = sqrt(pow(myX - cellCenter.x, 2) + pow(myY - cellCenter.y, 2));

    if (myDistanceToCenter < announcerDistance) {
        trace() << "Ignoring CL announcement from " << announcerId << " as I am closer to the center.";
        //TODO
    } else {
        trace() << "Accepting node " << announcerId << " as my new CL.";
        myCL_id = announcerId;
        cancelTimer(CL_ANNOUNCEMENT_TIMER);
    }
}

// Giai đoạn 2: Tái cấu trúc
void CellularRouting::startReconfiguration() {
    trace() << "Function startReconfiguration() called.";
    // Đây là hàm chỉ được gọi bởi CL
}

void CellularRouting::findAndEstablishInterCellLinks() {
    trace() << "Function findAndEstablishInterCellLinks() called.";
    // Đây là hàm chỉ được gọi bởi CL
}

void CellularRouting::handleLinkRequest(CellularRoutingPacket* pkt) {
    trace() << "Function handleLinkRequest() called for packet from " << pkt->getSource();
    // Logic của NGW -> chuyển tiếp lên NCL
}

void CellularRouting::handleLinkAck(CellularRoutingPacket* pkt) {
    trace() << "Function handleLinkAck() called for packet from " << pkt->getSource();
    // Logic của CGW -> gửi xác nhận về CL
}

void CellularRouting::handleLinkEstablishedConfirmation(CellularRoutingPacket* pkt) {
    trace() << "Function handleLinkEstablishedConfirmation() called for packet from " << pkt->getSource();
    // Logic của CL -> hủy timer, cập nhật bảng định tuyến liên ô
}

void CellularRouting::calculateAndDistributeIntraCellTree() {
    trace() << "Function calculateAndDistributeIntraCellTree() called.";
    // Logic của CL -> tính toán và gửi INTRA_CELL_ROUTING_UPDATE cho các thành viên
}

void CellularRouting::handleRoutingTableUpdate(CellularRoutingPacket* pkt) {
    trace() << "Function handleRoutingTableUpdate() called for packet from " << pkt->getSource();
    // Logic của node thường -> cập nhật next-hop của mình
}
