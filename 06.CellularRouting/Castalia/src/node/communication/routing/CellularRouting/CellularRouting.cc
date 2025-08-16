#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

static vector<NodeData> g_nodeDataList;
static vector<CellData> g_cellDataList;
static bool g_isPrecalculated = false;
static map<int, map<int, int>> g_routingTable; // <nodeId, <cellId, nextHopId>>

void CellularRouting::startup()
{
    // Initialize parameters: coordinates, communication interval, routing table, etc.
    helloInterval = par("helloInterval");
    cellRadius = par("cellRadius");
    clElectionDelayInterval = par("clElectionDelayInterval");

    myCL_id = -1;
    myCH_id = -1;
    myRole = NORMAL_NODE;

    cModule *parentNode = getParentModule();
    myX = parentNode->getParentModule()->par("xCoor");
    myY = parentNode->getParentModule()->par("yCoor");
    bool isCH = false;
    isCH = par("isCH");

    if (isCH) {
        myCH_id = self;
        myRole = CLUSTER_HEAD;
    }

    myCellId = -1;
    myColor = -1;
    myNextHopId = -1;
    neighborTable.clear();

    calculateCellInfo();
    grid_offset = par("gridOffset");

    NodeData nodeData;
    nodeData.id = self;
    nodeData.x = myX;
    nodeData.y = myY;
    nodeData.role = myRole;
    nodeData.cellId = myCellId;
    nodeData.color = myColor;
    nodeData.clId = myCL_id;
    nodeData.chId = myCH_id;
    nodeData.nextHopId = myNextHopId;
    nodeData.neighbors.clear();

    g_nodeDataList.push_back(nodeData);

    trace() << nodeData.id << " (" << nodeData.x << ", " << nodeData.y
            << ") - " << nodeData.role << " <" << nodeData.cellId << "> - {" << nodeData.color << "}"
            << " - CL: " << nodeData.clId << " - CH: " << nodeData.chId << " - Next Hop: " << nodeData.nextHopId
            << " - Neighbors: " << nodeData.neighbors.size();

            if (!g_isPrecalculated){
        g_isPrecalculated = true;
        setTimer(PRECALCULATE_TIMERS, 20);
    }


    // Set initial timer for STATE_0
    setTimer(STATE_0, uniform(0, 10));
    // Set timer for STATE_1
}

void CellularRouting::timerFiredCallback(int index)
{
    switch (index) {
        case PRECALCULATE_TIMERS:
            // Precalculate the simulation results
            PrecalculateSimulationResults();
            break;

        case STATE_0:
            setTimer(SEND_HELLO_TIMER, helloInterval);
            calculateFitnessScore();
            //setTimer(CL_ELECTION_TIMER, clElectionDelayInterval + fitnessScore*10);
            // setTimer(CL_CONFIRMATION_TIMER, uniform(0, 10));
            // setTimer(GATEWAY_SELECTION_TIMER, uniform(0, 10));
            // setTimer(ROUTING_TREE_CALCULATION_TIMER, uniform(0, 10));
            // setTimer(ROUTING_TABLE_ANNOUNCEMENT_TIMER, uniform(0, 10));
            // setTimer(FINALIZE_ROUTING_TIMER, uniform(0, 10));
            break;

        case SEND_HELLO_TIMER:
            sendHelloPacket();
            setTimer(SEND_HELLO_TIMER, helloInterval);
            break;

        case CL_ELECTION_TIMER:
            calculateFitnessScore();
            break;

        case CL_CONFIRMATION_TIMER:
            sendCLConfirmationPacket();
            break;

        case GATEWAY_SELECTION_TIMER:
            if (myRole == CLUSTER_HEAD) {
                gatewaySelection();
            }
            break;


    }

    // case CL_ELECTION_TIMER:
        // calculateFitnessScore();
        // start CL_ADVERTISING_TIMER base on fitness score

    // case CL_ADVERTISING_TIMER:
        // sendCLAnnouncement();

    // case CL_CONFIRMATION_TIMER:
        // if not CL: sendCLConfirmationPacket();

    // case GATEWAY_SELECTION_TIMER:
        // if CL: gatewaySelection();

    // case ROUTING_TREE_CALCULATION_TIMER:
        // if CL: calculateRoutingTree();

    // case ROUTING_TABLE_ANNOUNCEMENT_TIMER:
        // if CL: announceRoutingTable();

    // case FINALIZE_ROUTING_TIMER:
        // if CL: finalizeRouting();

    // case STAGE_1:
        // start the CH_ADVERTISING_TIMER
        // start the CH_ELECTION_TIMER

    // case CH_ADVERTISING_TIMER:
        // sendCHAnnouncement();

    // case CH_ELECTION_TIMER:
        // if CL: sellectClusterHead();
}

double CellularRouting::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void CellularRouting::PrecalculateSimulationResults()
{
    // Calculate node neighbors
    for (auto& nodeData : g_nodeDataList) {
        for (auto& otherNodeData : g_nodeDataList) {
            if (nodeData.id != otherNodeData.id) {
                double distance = sqrt(pow(nodeData.x - otherNodeData.x, 2) + pow(nodeData.y - otherNodeData.y, 2));
                if (distance <= cellRadius) {
                    NeighborRecord neighbor;
                    neighbor.id = otherNodeData.id;
                    neighbor.cellId = otherNodeData.cellId;
                    neighbor.x = otherNodeData.x;
                    neighbor.y = otherNodeData.y;
                    neighbor.lastHeard = simTime();
                    nodeData.neighbors.push_back(neighbor);
                }
            }
        }
    }
    // Calculate cell members
    for (auto& nodeData : g_nodeDataList) {
        // check if the cell already exists, add the node to the cell members
        bool cellExists = false;
        for (auto& cellData : g_cellDataList) {
            if (cellData.cellId == nodeData.cellId) {
                cellExists = true;
                CellMemberRecord member;
                member.id = nodeData.id;
                member.x = nodeData.x;
                member.y = nodeData.y;
                member.neighbors = nodeData.neighbors;
                cellData.members.push_back(member);
                break;
            }
        }
        // else create a new cell and add the node as a member
        if (!cellExists) {
            CellData cellData;
            cellData.cellId = nodeData.cellId;
            cellData.color = nodeData.color;
            cellData.clId = nodeData.clId;
            CellMemberRecord member;
            member.id = nodeData.id;
            member.x = nodeData.x;
            member.y = nodeData.y;
            member.neighbors = nodeData.neighbors;
            cellData.members.push_back(member);
            g_cellDataList.push_back(cellData);
        }
    }

    //calculate cell neighbors
    for (auto& cellData : g_cellDataList) {
        for (auto& nodeData : g_nodeDataList) {
            if (nodeData.cellId == cellData.cellId) {
                for (const auto& neighbor : nodeData.neighbors) {
                    // Check if the neighbor is in a different cell
                    if (neighbor.cellId != cellData.cellId) {
                        // Check if the neighbor is already in the cell neighbors
                        bool found = false;
                        for (auto& cellNeighbor : cellData.neighbors) {
                            if (cellNeighbor.cellId == neighbor.cellId) {
                                found = true;
                                break;
                            }
                        }
                        // If not found, add it to the cell neighbors
                        if (!found) {
                            CellNeighborRecord cellNeighbor;
                            cellNeighbor.cellId = neighbor.cellId;
                            cellNeighbor.color = nodeData.color; // Assuming color is same as node's color
                            cellNeighbor.ngw_id = -1; // Initialize NGW ID
                            cellData.neighbors.push_back(cellNeighbor);
                        }
                    }
                }
            }
        }
    }

    // Calculate cell leaders
    for (auto& cellData : g_cellDataList) {
        int bestFitness = -1;
        int bestCLId = -1;
        for (auto& member : cellData.members) {
            int r = round((double)cellData.cellId / grid_offset);
            int q = cellData.cellId - r * grid_offset;

            double centerX = cellRadius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r);
            double centerY = cellRadius * (3.0 / 2.0 * r);

            double distance = sqrt(pow(member.x - centerX, 2) + pow(member.y - centerY, 2));

            int fitnessScore = static_cast<int>(1000 / (1 + distance));
            if (fitnessScore > bestFitness) {
                bestFitness = fitnessScore;
                bestCLId = member.id;
            }
        }
        cellData.clId = bestCLId;
    }

    // calculate gateway candidates for each pair of cell
    for (auto& cellData : g_cellDataList) {
        for (auto& neighbor : cellData.neighbors) {
            if (neighbor.ngw_id == -1) { // If NGW ID is not set
                //choose the best pair of CGW and NGW
                double bestDistance = 9999.0;
                int bestCGWId = -1;
                int bestNGWId = -1;

                for (auto& nodeData : g_nodeDataList) {
                    if (nodeData.cellId == cellData.cellId) {
                        for (auto& neighborNode : nodeData.neighbors) {
                            //if (cellData.cellId >= neighbor.cellId) continue;
                            if (neighborNode.cellId == neighbor.cellId) {
                                double linkDistance = sqrt(pow(nodeData.x - neighborNode.x, 2) + pow(nodeData.y - neighborNode.y, 2));

                                if (linkDistance < bestDistance) {
                                    bestDistance = linkDistance;
                                    bestCGWId = nodeData.id;
                                    bestNGWId = neighborNode.id;
                                }
                                else if (linkDistance == bestDistance) {
                                    // If the distance is equal, prefer the one with lower ID
                                    if (nodeData.id < bestCGWId) {
                                        bestCGWId = nodeData.id;
                                        bestNGWId = neighborNode.id;
                                    }
                                }
                            }
                        }
                    }
                }

                if (bestCGWId != -1 && bestNGWId != -1) {
                    cellData.gateways[neighbor.cellId] = bestCGWId;
                    neighbor.ngw_id = bestNGWId;
                }
            }
            // update next hop ID for the CGW
            for (auto& nodeData : g_nodeDataList) {
                if (nodeData.id == cellData.gateways[neighbor.cellId]) {
                    nodeData.nextHopId = neighbor.ngw_id; // Set next hop to NGW
                }
            }
        }
    }
    //trace next hop IDs
    for (const auto& nodeData : g_nodeDataList) {
        trace() << "Node ID: " << nodeData.id << ", Next Hop ID: " << nodeData.nextHopId;
    }

    // calculate intra-cell routing table for each cell gateways and CL
    for (auto& cellData : g_cellDataList) {
        for (auto& neighbor : cellData.neighbors) {
            if (neighbor.ngw_id != -1) { // If NGW ID is set
                int gatewayId = cellData.gateways[neighbor.cellId];
                for (auto& member : cellData.members) {
                    g_routingTable[member.id][neighbor.cellId] = -1;
                    bool isGatewayInRange = false;
                    if (member.id != gatewayId) {
                        for (const auto& neighborNode : member.neighbors) {
                            if (neighborNode.id == gatewayId) {
                                isGatewayInRange = true;
                                g_routingTable[member.id][neighbor.cellId] = gatewayId;
                            }

                        }
                        if (!isGatewayInRange) {
                            // If the gateway is not in range, find the best next hop
                            double minDistance = 9999.0;
                            int bestNextHopId = -1;

                            for (const auto& neighborNode : member.neighbors) {
                                bool isNeighborInRange = false;
                                // continue if the neighbor is not in range of the gateway
                                for (const auto& neighborNodeData : g_nodeDataList) {
                                    if (neighborNodeData.id == neighborNode.id) {
                                        for (auto& neighborOfNeighbor : neighborNodeData.neighbors) {
                                            if (neighborOfNeighbor.id == gatewayId) {
                                                isNeighborInRange = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (isNeighborInRange) {

                                    // save the distance and choose the best next hop based on total distance from this node to neighbor to gateway
                                    auto it = std::find_if(cellData.members.begin(), cellData.members.end(), [gatewayId](const CellMemberRecord& m) { return m.id == gatewayId; });
                                    double distanceNodeToNeighbor = sqrt(pow(member.x - neighborNode.x, 2) + pow(member.y - neighborNode.y, 2));
                                    double distanceNeighborToGateway = sqrt(pow(neighborNode.x - it->x, 2) + pow(neighborNode.y - it->y, 2));
                                    double totalDistance = distanceNodeToNeighbor + distanceNeighborToGateway;
                                    if (totalDistance < minDistance) {
                                        minDistance = totalDistance;
                                        bestNextHopId = neighborNode.id;
                                    } else if (totalDistance == minDistance) {
                                        // If the distance is equal, prefer the one with lower ID
                                        if (neighborNode.id < bestNextHopId) {
                                            bestNextHopId = neighborNode.id;
                                        }
                                    }
                                }
                            }
                            if (bestNextHopId != -1) {
                                g_routingTable[member.id][neighbor.cellId] = bestNextHopId;
                            }
                        }
                        trace() << "Intra-cell routing: member " << member.id
                                << " → cell " << neighbor.cellId
                                << " via next hop " << g_routingTable[member.id][neighbor.cellId]
                                << " (gateway " << gatewayId << ")";
                    }
                }
            }
        }
        // routing table for CL
        for (const auto& member : cellData.members) {
            if (member.id == cellData.clId) {
                continue;
            }
            g_routingTable[member.id][cellData.cellId] = -1;
            bool isCLInRange = false;
            for (const auto& neighbor : member.neighbors) {
                if (neighbor.id == cellData.clId) {
                    isCLInRange = true;
                    break;
                }
            }
            if (isCLInRange) {
                g_routingTable[member.id][cellData.cellId] = cellData.clId;
            } else {
                double minDistance = 9999.0;
                int bestNextHopId = -1;

                for (const auto& neighbor : member.neighbors) {
                    // Check if the neighbor is in range of the CL
                    bool isNeighborInRange = false;
                    for (const auto& neighborNodeData : g_nodeDataList) {
                        if (neighborNodeData.id == neighbor.id) {
                            for (const auto& neighborOfNeighbor : neighborNodeData.neighbors) {
                                if (neighborOfNeighbor.id == cellData.clId) {
                                    isNeighborInRange = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (isNeighborInRange) {
                        // save the distance and choose the best next hop based on total distance from this node to neighbor to CL
                        auto it = std::find_if(cellData.members.begin(), cellData.members.end(), [cellData](const CellMemberRecord& m) { return m.id == cellData.clId; });
                        double distanceNodeToNeighbor = sqrt(pow(member.x - neighbor.x, 2) + pow(member.y - neighbor.y, 2));
                        double distanceNeighborToCL = sqrt(pow(neighbor.x - it->x, 2) + pow(neighbor.y - it->y, 2));
                        double totalDistance = distanceNodeToNeighbor + distanceNeighborToCL;
                        if (totalDistance < minDistance) {
                            minDistance = totalDistance;
                            bestNextHopId = neighbor.id;
                        } else if (totalDistance == minDistance) {
                            // If the distance is equal, prefer the one with lower ID
                            if (neighbor.id < bestNextHopId) {
                                bestNextHopId = neighbor.id;
                            }
                        }
                    }
                }
                if (bestNextHopId != -1) {
                    g_routingTable[member.id][cellData.cellId] = bestNextHopId;
                }
            }
            trace() << "Intra-cell routing: member " << member.id
                    << " → cell " << cellData.cellId
                    << " via next hop " << g_routingTable[member.id][cellData.cellId];
        }
    }
}

Point CellularRouting::calculateCellCenter(int cell_id) {
    int r = round((double)cell_id / grid_offset);
    int q = cell_id - r * grid_offset;

    Point center;
    center.x = cellRadius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r);
    center.y = cellRadius * (3.0 / 2.0 * r);

    return center;
}

void CellularRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void CellularRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    CellularRoutingPacket* netPacket = dynamic_cast<CellularRoutingPacket*>(pkt);
    if (!netPacket) {
        return;
    }
    netPacket->setTtl(netPacket->getTtl() - 1);
    switch (netPacket->getPacketType()) {
        case HELLO_PACKET: {
            handleHelloPacket(netPacket);
            break;
        }

        case CL_ANNOUNCEMENT: {
            handleCLAnnouncementPacket(netPacket);
            break;
        }

        case CL_CONFIRMATION: {
            handleCLConfirmationPacket(netPacket);
            break;
        }
        
        default: {
            trace() << "WARNING: Received unknown packet type.";
            break;
        }
    }
}

void CellularRouting::calculateCellInfo()
{
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

    const int grid_offset = 100;
    myCellId = q + r * grid_offset;
    myColor = ((q - r) % 3 + 3) % 3;
}

void CellularRouting::sendHelloPacket()
{
        CellularRoutingPacket *netPacket = new CellularRoutingPacket("CellularRouting packet", NETWORK_LAYER_PACKET);
        netPacket->setPacketType(HELLO_PACKET);
        HelloInfo helloData;
        helloData.x = myX;
        helloData.y = myY;
        helloData.cellId = myCellId;
        netPacket->setHelloData(helloData);
        netPacket->setSource(SELF_NETWORK_ADDRESS);
        netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
        toMacLayer(netPacket, resolveNetworkAddress(BROADCAST_NETWORK_ADDRESS));
}

void CellularRouting::handleHelloPacket(CellularRoutingPacket* pkt)
{
    int sourceId = atoi(pkt->getSource());

    for (const auto& neighbor : neighborTable) {
        if (neighbor.id == sourceId) {
            return; // Already a neighbor
        }
    }

    HelloInfo helloData = pkt->getHelloData();
    NeighborRecord newNeighbor;
    newNeighbor.id = sourceId;
    newNeighbor.x = helloData.x;
    newNeighbor.y = helloData.y;
    // if neighbor is far away, consider it for removal
    if (calculateDistance(myX, myY, newNeighbor.x, newNeighbor.y) > cellRadius) {
        return;
    }
    newNeighbor.cellId = helloData.cellId;
    neighborTable.push_back(newNeighbor);

    // rate the neighborTable to the pre-calculated neighbors in g_nodeDataList
    int rightCount = 0;
    int errorCount = 0;
    for (const auto& nodeData : g_nodeDataList) {
        if (nodeData.id == self) {
            for (const auto& neighbor : nodeData.neighbors) {
                bool found = false;
                for (const auto& neighborRecord : neighborTable) {
                    if (neighborRecord.id == neighbor.id) {
                        found = true;
                        rightCount++;
                        break;
                    }
                }
                if (!found) {
                    errorCount++;
                }
            }
            trace() << "Neighbor Table Rating: " << rightCount << "/" << nodeData.neighbors.size() << " right, " << errorCount << " errors";
        }
    }
}

void CellularRouting::calculateFitnessScore()
{
    // Calculate fitness score based on id, energy, distance, etc.
    // Or just let the application layer handle it
    Point cellCenter = calculateCellCenter(myCellId);
    double distanceToCenter = sqrt(pow(myX - cellCenter.x, 2) + pow(myY - cellCenter.y, 2));
    fitnessScore = 1.0 / (1.0 + distanceToCenter);
}

void CellularRouting::sendCLAnnouncement()
{
    CellularRoutingPacket* pkt = new CellularRoutingPacket("CL Announcement", NETWORK_LAYER_PACKET);
    pkt->setPacketType(CL_ANNOUNCEMENT);
    CLAnnouncementInfo newCLAnnouncementInfo;
    newCLAnnouncementInfo.x = myX;
    newCLAnnouncementInfo.y = myY;
    newCLAnnouncementInfo.cellId = myCellId;
    newCLAnnouncementInfo.fitnessScore = fitnessScore;
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

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt)
{

    if (myCellId != pkt->getClAnnouncementData().cellId) {
        return;
    }
    int announcerId = atoi(pkt->getSource());
    // make comparisons with my best fitness score
    // If I have a worse score, cancel my CL_ADVERTISING_TIMER if running
    if (fitnessScore < pkt->getClAnnouncementData().fitnessScore && clFitnessScore < pkt->getClAnnouncementData().fitnessScore) {
        cancelTimer(CL_ANNOUNCEMENT_TIMER);
        myCL_id = announcerId;
        clFitnessScore = pkt->getClAnnouncementData().fitnessScore;
    }

    // Forward the packet to other cell members
    if (pkt->getTtl() > 0) {
        CellularRoutingPacket* fwdPkt = pkt->dup();
        fwdPkt->setTtl(1);
        fwdPkt->setSource(SELF_NETWORK_ADDRESS);
        for (const auto& neighbor : neighborTable) {
            std::stringstream dest_addr;
            dest_addr << neighbor.id;
            fwdPkt->setDestination(dest_addr.str().c_str());
            toMacLayer(fwdPkt, neighbor.id);
        }
    }

    setTimer(CL_CONFIRMATION_TIMER, 50);
}

void CellularRouting::sendCLConfirmationPacket()
{
    // Send confirmation packet to the best candidate CL with my information
    if (myCL_id != -1) {

        CellularRoutingPacket* confirmPkt = new CellularRoutingPacket("CL Confirmation", NETWORK_LAYER_PACKET);
        confirmPkt->setPacketType(CL_CONFIRMATION);

        NodeInfo myInfo;
        myInfo.nodeId = self;
        myInfo.x = myX;
        myInfo.y = myY;

        for (int i = 0; i < 20; ++i) {
            myInfo.neighbors[i].nodeId = -1;
        }

        for (size_t i = 0; i < neighborTable.size(); ++i) {
            myInfo.neighbors[i].nodeId = neighborTable[i].id;
            myInfo.neighbors[i].x = neighborTable[i].x;
            myInfo.neighbors[i].y = neighborTable[i].y;
            myInfo.neighbors[i].cellId = neighborTable[i].cellId;
        }

        confirmPkt->setNodeInfoData(myInfo);
        // if CL is in range
        bool isCLInRange = false;
        for (const auto& neighbor : neighborTable) {
            if (neighbor.id == myCL_id) {
                confirmPkt->setSource(SELF_NETWORK_ADDRESS);
                std::stringstream dest_addr;
                dest_addr << myCL_id;
                confirmPkt->setDestination(dest_addr.str().c_str());
                isCLInRange = true;
                toMacLayer(confirmPkt, myCL_id);
            }
        }
        if (!isCLInRange) {
            for (const auto& neighbor : neighborTable) {
                confirmPkt->setSource(SELF_NETWORK_ADDRESS);
                std::stringstream dest_addr;
                dest_addr << neighbor.id;
                confirmPkt->setDestination(dest_addr.str().c_str());
                confirmPkt->setTtl(1);
                toMacLayer(confirmPkt, neighbor.id);
            }
        }

        toMacLayer(confirmPkt, myCL_id);
    }
}

void CellularRouting::handleCLConfirmationPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save the confirmation and update my cell members
    // If I am not the CL, forward the confirmation to my CL
    if (myRole == CELL_LEADER) {
        NodeInfo senderInfo = pkt->getNodeInfoData();
        int senderId = senderInfo.nodeId;

        if (cellMembers.empty()) {
            setTimer(CL_CONFIRMATION_TIMER, 200);
        }

        for (const auto& member : cellMembers) {
            if (member.id == senderId) {
                return;
            }
        }

        CellMemberRecord newMember;
        newMember.id = senderId;
        newMember.x = senderInfo.x;
        newMember.y = senderInfo.y;
        newMember.energy = senderInfo.energyLevel;

        for (size_t i = 0; i < 20; ++i) {
            NeighborInfo neighbor_of_sender = pkt->getNodeInfoData().neighbors[i];
            if (neighbor_of_sender.nodeId == -1) {
                continue; // Skip empty neighbors
            }
            NeighborRecord neighbor_of_sender_record;
            neighbor_of_sender_record.id = neighbor_of_sender.nodeId;
            neighbor_of_sender_record.x = neighbor_of_sender.x;
            neighbor_of_sender_record.y = neighbor_of_sender.y;
            neighbor_of_sender_record.cellId = neighbor_of_sender.cellId;
            newMember.neighbors.push_back(neighbor_of_sender_record);
        }

        cellMembers.push_back(newMember);
    } else {
        if (pkt->getTtl() > 0) {
            CellularRoutingPacket* fwdPkt = pkt->dup();
            fwdPkt->setTtl(1);
            fwdPkt->setSource(SELF_NETWORK_ADDRESS);
            std::stringstream dest_addr;
            dest_addr << myCL_id;
            fwdPkt->setDestination(dest_addr.str().c_str());
            toMacLayer(fwdPkt, myCL_id);
        }
    }
}

void CellularRouting::gatewaySelection()
{
    // Calculate the best gateway candidates based on distance
    // Or just let the application layer handle it
    // sendGatewaySelectionPacket(); after a random delay
}

void CellularRouting::sendGatewaySelectionPacket()
{

}

void CellularRouting::handleGatewaySelectionPacket(CellularRoutingPacket* pkt)
{

}

void CellularRouting::sendLinkRequestPacket()
{

}

void CellularRouting::handleLinkRequestPacket(CellularRoutingPacket* pkt)
{
    // If I am the NGW, forward the request to my CL
    // If I am the CL, sendLinkConfirmationPacket()
}

void CellularRouting::sendLinkConfirmationPacket(CellularRoutingPacket* pkt)
{
    // Send link confirmation packet to the NGW
}

void CellularRouting::handleLinkConfirmationPacket(CellularRoutingPacket* pkt)
{
    // If I am the NGW, update my state and forward the confirmation to my CGW
    // If I am the CGW, forward the confirmation to my CL
    // If I am the CL, update my routing table
}

void CellularRouting::calculateRoutingTree()
{
    // Calculate the intra-cell routing trees with CGWs and CL are roots
    // all members should have a routing table entry to every CGW and CL
    // Or just let the application layer handle it
//    if (!amI_CL) return;
//
//    if (gatewayTowardsCH == -1) {
//        gatewayTowardsCH = myCH_id;
//    }
//
//    for (const auto& member : cellMembers) {
//        if (member.id == myCH_id) {
//            gatewayTowardsCH = myCH_id;
//            break;
//        }
//    }
//
//    Point gatewayPos = allNodesPositions[gatewayTowardsCH];
//
//    for (const auto& member : cellMembers) {
//        if (member.id == gatewayTowardsCH) {
//            intraCellRoutingTable[member.id] = -1;
//            continue;
//        }
//
//        int bestHop = -1;
//        double minDistanceToGatewaySq = -1.0;
//        for (const auto& neighbor : member.neighbors) {
//            if (neighbor.cellId == myCellId) {
//                double distSq;
//                if (neighbor.id == myCH_id) distSq = 0.0;
//
//                distSq = pow(allNodesPositions[neighbor.id].x - gatewayPos.x, 2) +
//                                pow(allNodesPositions[neighbor.id].y - gatewayPos.y, 2);
//
//                if (bestHop == -1 || distSq < minDistanceToGatewaySq) {
//                    minDistanceToGatewaySq = distSq;
//                    bestHop = neighbor.id;
//                }
//            }
//        }
//
//        intraCellRoutingTable[member.id] = bestHop;
//
//        if (bestHop != -1) {
//            announceRoutingTree(member.id, bestHop);
//        } else {
//            trace() << "  WARNING: Node " << member.id << " has no neighbors in this cell to forward packets.";
//        }
//    }
//
//    int bestHopForCL = -1;
//    double minDistanceForCLSq = -1.0;
//
//    for (const auto& neighbor : neighborTable) {
//         if (neighbor.cellId == myCellId) {
//            double distSq = pow(neighbor.x - gatewayPos.x, 2) +
//                            pow(neighbor.y - gatewayPos.y, 2);
//
//            if (bestHopForCL == -1 || distSq < minDistanceForCLSq) {
//                minDistanceForCLSq = distSq;
//                bestHopForCL = neighbor.id;
//            }
//        }
//    }
//
//    myNextHopId = bestHopForCL;
//
//    if (myNextHopId != -1) {
//         trace() << "[ROUTING] " << self << " to "<< myNextHopId;
//    } else {
//         trace() << "  WARNING: I (CL) have no intra-cell neighbors to forward packets.";
//    }
}

void CellularRouting::announceRoutingTable()
{
    // for each cell member, send a routing table update packet
    // sendRoutingTableAnnouncementPacket();
}

void CellularRouting::sendRoutingTableAnnouncementPacket()
{
    // Create and send a routing table announcement packet
}

void CellularRouting::handleRoutingTableAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If for me: Update my routing table based on the received announcement
    // If not for me: Forward the packet to the destination if in range
}

void CellularRouting::finalizeRouting()
{
    // Node has completed routing setup
    // Send a final confirmation to CL
}

void CellularRouting::sendCHAnnouncement()
{
    // If I am the CH, send a CH announcement packet CL
}

void CellularRouting::handleCHAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save my cell information to packet metadata
        // and forward the CH announcement to my cell members with destination as all neighbor cells

    // If I am a cell member,
        // if the destination is our cell, forward the packet to my CL
        // if the destination is not our cell, forward the packet to CGW

    // If I am a CGW,
        // if the destination is our cell, forward the packet to my CL
        // if the destination is not our cell, forward the packet to my NGW
}

void CellularRouting::selectClusterHead()
{
    // select the closest CH
    // Or just let the application layer handle it
}
