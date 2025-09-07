#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

static vector<NodeData> g_nodeDataList;
static vector<CellData> g_cellDataList;
static bool g_isPrecalculated = false;
static map<int, map<int, int>> g_routingTable; // <nodeId, <cellId, nextHopId>>
static int g_newCH[3] = {-1, -1, -1};
static int g_lifetime = 0;
static int g_cellHopAnnouncement[100][100]; // <node> <CH>
static int g_announcementCount = 0;
static int g_sensorData[100]; // < data>
static int g_sensorDataReceived[100]; // < data>


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
        trace() << "#CH " << self;
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

    // trace() << nodeData.id << " (" << nodeData.x << ", " << nodeData.y
    //         << ") - " << nodeData.role << " <" << nodeData.cellId << "> - {" << nodeData.color << "}"
    //         << " - CL: " << nodeData.clId << " - CH: " << nodeData.chId << " - Next Hop: " << nodeData.nextHopId
    //         << " - Neighbors: " << nodeData.neighbors.size();
    trace() << "#NODE " << nodeData.id << " (" << nodeData.x << ", " << nodeData.y << ")";
        if (!g_isPrecalculated){
            g_isPrecalculated = true;
            setTimer(PRECALCULATE_TIMERS, 20);
        }

    setTimer(RECONFIGURATION_TIMER, 0);
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
            setTimer(HELLO_TIMEOUT, helloInterval * 6);
            break;

        case SEND_HELLO_TIMER:
            sendHelloPacket();
            setTimer(SEND_HELLO_TIMER, helloInterval);
            break;

        case HELLO_TIMEOUT:
            cancelTimer(SEND_HELLO_TIMER);
            calculateFitnessScore();

            break;

        case CL_ELECTION_TIMER:
            myRole = CELL_LEADER;
            setTimer(CL_CALCULATION_TIMER, uniform(300, 400));
            trace() << "#CELL_LEADER " << myCellId << ": " << self;
            sendCLAnnouncement();
            break;

        case CL_CONFIRMATION_TIMER:
            sendCLConfirmationPacket();
            break;

        case GATEWAY_SELECTION_TIMER:
            if (myRole == CLUSTER_HEAD) {
                gatewaySelection();
            }
            break;
        case CL_CALCULATION_TIMER:
            if (myRole == CELL_LEADER) {
                calculateRoutingTree();
                setTimer(ROUTING_TABLE_UPDATE_TIMER, uniform(100, 200));
            }
            break;
        case ROUTING_TABLE_UPDATE_TIMER:
            if (myRole == CELL_LEADER) {
                sendRoutingTableAnnouncementPacket();
            }
            break;

        case FINALIZE_TIMER:
            finalizeRouting();
            break;

        case STATE_1:
            if (myCH_id == self) {
                sendCHAnnouncement();
            }
            break;

        case SEND_CELL_PACKET:
            sendCellPacket();
            setTimer(SEND_CELL_PACKET, uniform(1, 10));
            break;

        case SEND_ANNOUNCEMENT_QUEUE:
            sendAnnouncementQueue();
            setTimer(SEND_ANNOUNCEMENT_QUEUE, uniform(1, 10));
            break;

        case ANNOUNCE_CELL_HOP_TIMER:
            sendCellHopAnnouncementPacket();
            break;
        // case COLOR_SCHEDULING_TIMER:
        //     // Handle color scheduling
        //     isInScheduling = !isInScheduling;
        //     setTimer(COLOR_SCHEDULING_TIMER, 1200);
        //     while (isInScheduling && !cellPacketQueue.empty()) {
        //         sendCellPacket();
        //         trace() << "#SCHEDULED_PKT " << self << " to " << cellPacketQueue.front().destId;
        //     }
        //     break;

        case SENSING_STATE:
            sendSensorDataPacket();
            setTimer(SENSING_STATE, 15000); //uniform(9000,10000));
            break;

        case RECONFIGURATION_TIMER:
            if (g_lifetime > 0) {
                if (myCH_id == self) {
                    myCH_id = -1;
                    myRole = NORMAL_NODE;
                }
                if (self == g_newCH[0] || self == g_newCH[1] || self == g_newCH[2]) {
                    myCH_id = self;
                    trace() << "#CH " << self;
                }
            }

            setTimer(STATE_0, uniform(0, 10));
            setTimer(STATE_1, 1000);
            setTimer(SENSING_STATE, uniform(3000, 3100));
            setTimer(RECONFIGURATION_TIMER, 15000);
            break;
    }
}

double CellularRouting::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void CellularRouting::PrecalculateSimulationResults()
{
    g_newCH[0] = uniform(0, 100);
    g_newCH[1] = uniform(0, 100);
    g_newCH[2] = uniform(0, 100);

    for (int i=0; i<100; i++) {
        g_sensorData[i] = -1;
        g_sensorDataReceived[i] = -1;
    }

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
    // for (const auto& nodeData : g_nodeDataList) {
    //     trace() << "Node ID: " << nodeData.id << ", Next Hop ID: " << nodeData.nextHopId;
    // }

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
                        // trace() << "Intra-cell routing: member " << member.id
                        //         << " → cell " << neighbor.cellId
                        //         << " via next hop " << g_routingTable[member.id][neighbor.cellId]
                        //         << " (gateway " << gatewayId << ")";
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
            // trace() << "Intra-cell routing: member " << member.id
            //         << " → cell " << cellData.cellId
            //         << " via next hop " << g_routingTable[member.id][cellData.cellId];
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
            //trace() << "Received CL Announcement from " << netPacket->getSource();
            handleCLAnnouncementPacket(netPacket);
            break;
        }

        case CL_CONFIRMATION: {
            //trace() << "Received CL Confirmation from " << netPacket->getSource();
            handleCLConfirmationPacket(netPacket);
            break;
        }

        case ROUTING_TREE_UPDATE_PACKET: {
            handleRoutingTableAnnouncementPacket(netPacket);
            break;
        }

        case CH_ANNOUNCEMENT_PACKET: {
            //trace() << "Received CH Announcement from " << netPacket->getSource();
            handleCHAnnouncementPacket(netPacket);
            break;
        }

        case ANNOUNCE_CELL_HOP: {
            handleCellHopAnnouncementPacket(netPacket);
            break;
        }

        case SENSOR_DATA: {
            //trace() << "Received sensor data from " << netPacket->getSource();
            handleSensorDataPacket(netPacket);
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
    trace() << "#CELL_COLOR " << self << ": " << myColor;
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

    // trace() << "New neighbor discovered: " << newNeighbor.id
    //         << " at (" << newNeighbor.x << ", " << newNeighbor.y
    //         << ") with cell ID: " << newNeighbor.cellId;
}

void CellularRouting::calculateFitnessScore()
{
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
    // Calculate fitness score based on id, energy, distance, etc.
    // Or just let the application layer handle it
    Point cellCenter = calculateCellCenter(myCellId);
    double distanceToCenter = calculateDistance(myX, myY, cellCenter.x, cellCenter.y);
    fitnessScore = 1.0 / (1.0 + distanceToCenter);

    int bestNeighborId = -1;
    double bestNeighborScore = -1;
    for (const auto& neighbor : neighborTable) {
        if (neighbor.cellId != myCellId) {
            continue;
        }
        double neighborDistance = calculateDistance(cellCenter.x, cellCenter.y, neighbor.x, neighbor.y);
        double neighborFitness = 1.0 / (1.0 + neighborDistance);
        if (neighborFitness > bestNeighborScore) {
            bestNeighborScore = neighborFitness;
            bestNeighborId = neighbor.id;
        }
    }
    if (bestNeighborScore < fitnessScore) {
        // If the best neighbor's score is better than mine, I should consider it
        setTimer(CL_ELECTION_TIMER, clElectionDelayInterval + fitnessScore*100);

    }
}

void CellularRouting::sendCLAnnouncement()
{
    // trace() << "Node " << self
    //        << " sending CL Announcement: " << fitnessScore;
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
            pkt_for_neighbor->setTtl(1);
           pkt_for_neighbor->setSource(SELF_NETWORK_ADDRESS);
           std::stringstream dest_addr;
           dest_addr << neighborId;
           pkt_for_neighbor->setDestination(dest_addr.str().c_str());
           toMacLayer(pkt_for_neighbor, neighborId);
        //    trace() << "Sent CL Announcement to neighbor " << neighborId
        //             << " with fitness score: " << fitnessScore;
    }
}

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt)
{
    if (myCellId != pkt->getClAnnouncementData().cellId) {
        return;
    }
    trace() << "#CL_ANNOUNCEMENT " << pkt->getSource() << ": " << self;

    int announcerId = atoi(pkt->getSource());
    // make comparisons with my best fitness score
    // If I have a worse score, cancel my CL_ADVERTISING_TIMER if running
    if (fitnessScore < pkt->getClAnnouncementData().fitnessScore && clFitnessScore < pkt->getClAnnouncementData().fitnessScore) {
        cancelTimer(CL_ANNOUNCEMENT_TIMER);
        myCL_id = announcerId;
        clFitnessScore = pkt->getClAnnouncementData().fitnessScore;
    }

    // Forward the packet to other cell members
    // if (pkt->getTtl() >= 0) {

    //     for (const auto& neighbor : neighborTable) {
    //         if (neighbor.id == announcerId) {
    //             continue;
    //         }
    //         CellularRoutingPacket* fwdPkt = pkt->dup();
    //         fwdPkt->setSource(SELF_NETWORK_ADDRESS);
    //         std::stringstream dest_addr;
    //         dest_addr << neighbor.id;
    //         fwdPkt->setDestination(dest_addr.str().c_str());
    //         toMacLayer(fwdPkt, neighbor.id);
    //     }
    // }

    setTimer(CL_CONFIRMATION_TIMER, uniform(100, 200));
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
                CellularRoutingPacket* fwdPkt = confirmPkt->dup();
                fwdPkt->setSource(SELF_NETWORK_ADDRESS);
                std::stringstream dest_addr;
                dest_addr << neighbor.id;
                fwdPkt->setDestination(dest_addr.str().c_str());
                fwdPkt->setTtl(1);
                toMacLayer(fwdPkt, neighbor.id);
            }
        }
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
            CellMemberRecord myself;
            myself.id = self;
            myself.x = myX;
            myself.y = myY;
            myself.neighbors = neighborTable;
            cellMembers.push_back(myself);
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
    //trace() << "Received CL Confirmation from " << pkt->getSource();
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
    // rate cell members
    int rightCount = 0;
    int errorCount = 0;
    for (const auto& gCell : g_cellDataList){
        if (gCell.cellId == myCellId) {
            for (const auto& member : cellMembers) {
                bool found = false;
                for (const auto& gmember : gCell.members) {
                    if (gmember.id == member.id) {
                        found = true;
                        rightCount++;
                    }
                }
                if (!found) {
                    errorCount++;
                }
            }
            trace() << "Cell member Rating for cell " << myCellId << ": " << rightCount << "/" << gCell.members.size() << " right, " << errorCount << " errors";
        }
    }

    // Calculate the intra-cell routing trees with CGWs and CL are roots
    // all members should have a routing table entry to every CGW and CL
    // Or just let the application layer handle it

    // calculate gateway candidates for each pair of cell
    int neighborCell[6] = {-1, -1, -1, -1, -1, -1};
    double bestDistance[6] = {9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 9999.0};
    int bestCGWId[6] = {-1, -1, -1, -1, -1, -1};
    int bestNGWId[6] = {-1, -1, -1, -1, -1, -1};


    for (auto& member : cellMembers) {
        for (auto& memberNeighbor : member.neighbors) {
            if (memberNeighbor.cellId != myCellId){
                int i=0;
                for (i = 0; i < 6; ++i) {
                    if (neighborCell[i] == -1 || neighborCell[i] == memberNeighbor.cellId) {
                        neighborCell[i] = memberNeighbor.cellId;
                        break;
                    }
                }
                double distance = calculateDistance(member.x, member.y, memberNeighbor.x, memberNeighbor.y);
                if (distance < bestDistance[i]) {
                    bestDistance[i] = distance;
                    bestCGWId[i] = member.id;
                    bestNGWId[i] = memberNeighbor.id;
                } else if (distance == bestDistance[i]) {
                    if (member.id < bestCGWId[i]) {
                        bestCGWId[i] = member.id;
                        bestNGWId[i] = memberNeighbor.id;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 6; ++i) {
        if (neighborCell[i] == -1) {
            continue;
        }
        trace() << "#GATEWAY_SELECTION " << myCellId << ": " << bestCGWId[i] << " -> " << bestNGWId[i];
    }

    // calculate intra-cell routing table for each cell gateways and CL
    for (int i = 0; i < 6; ++i) {
        if (neighborCell[i] != -1 && bestCGWId[i] != -1 && bestNGWId[i] != -1) {
            int gatewayId = bestCGWId[i];
            for (auto& member : cellMembers) {
                intraCellRoutingTable[member.id][neighborCell[i]] = -1;
                bool isGatewayInRange = false;
                if (member.id != gatewayId) {
                    for (const auto& neighborNode : member.neighbors) {
                        if (neighborNode.id == gatewayId) {
                            isGatewayInRange = true;
                            intraCellRoutingTable[member.id][neighborCell[i]] = gatewayId;
                        }
                    }
                    if (!isGatewayInRange) { // member -> neighborNode -> neighborOfNeighbor = gateway
                        double minDistance = 9999.0;
                        int bestNextHopId = -1;

                        for (const auto& neighborNode : member.neighbors) {
                            bool isNeighborInRange = false;
                            for (const auto& neighborInCell : cellMembers) {
                                if (neighborInCell.id == neighborNode.id) {
                                    for (const auto& neighborOfNeighbor : neighborInCell.neighbors) {
                                        if (neighborOfNeighbor.id == gatewayId) {
                                            isNeighborInRange = true;
                                            break;
                                        }
                                    }
                                }
                            }

                            if (isNeighborInRange) {
                                // save the distance and choose the best next hop based on total distance from this node to neighbor to gateway
                                auto it = std::find_if(cellMembers.begin(), cellMembers.end(), [gatewayId](const CellMemberRecord& m) { return m.id == gatewayId; });
                                if (it != cellMembers.end()) {
                                    double totalDistance = calculateDistance(member.x, member.y, neighborNode.x, neighborNode.y) + calculateDistance(neighborNode.x, neighborNode.y, it->x, it->y);
                                    if (totalDistance < minDistance) {
                                        minDistance = totalDistance;
                                        bestNextHopId = neighborNode.id;
                                    }
                                    else if (totalDistance == minDistance) {
                                        if (neighborNode.id < bestNextHopId) {
                                            bestNextHopId = neighborNode.id;
                                        }
                                    }
                                }
                            }
                        }
                        if (bestNextHopId != -1) {
                            intraCellRoutingTable[member.id][neighborCell[i]] = bestNextHopId;
                        }
                    }
                    // trace() << "Intra-cell routing: member " << member.id
                    //     << " → cell " << neighborCell[i]
                    //     << " via next hop " << intraCellRoutingTable[member.id][neighborCell[i]]
                    //     << " (gateway " << gatewayId << ")";
                }
            }
        }
    }

    for (int i = 0; i < 6; ++i) {
        if (neighborCell[i] == -1 || bestCGWId[i] == -1 || bestNGWId[i] == -1) {
            continue; // skip if no neighbor cell or no gateway candidates
        }
        neighborCells[i] = neighborCell[i];
        cellGateways[i] = bestCGWId[i];
        neighborCellGateways[i] = bestNGWId[i];
        intraCellRoutingTable[bestCGWId[i]][neighborCell[i]] = bestNGWId[i];
        // trace() << "From " << myCellId << " to " << neighborCell[i]
        //         << " with CGW " << bestCGWId[i]
        //         << " and NGW " << bestNGWId[i];
    }

    // routing table for CL
    for (const auto& member : cellMembers){
        int currentCellId = myCellId;
        if (member.id == self) {
            continue;
        }
        intraCellRoutingTable[member.id][myCellId] = -1;
        bool isCLInRange = false;
        for (const auto& neighbor : member.neighbors) {
            if (neighbor.id == self) {
                isCLInRange = true;
                break;
            }
        }
        if (isCLInRange) {
            intraCellRoutingTable[member.id][myCellId] = self;
        } else {
            double minDistance = 9999.0;
            int bestNextHopId = -1;

            for (const auto& neighbor : member.neighbors) {
                // Check if the neighbor is in range of the CL
                bool isNeighborInRange = false;
                for (const auto& neighborNodeData : cellMembers) {
                    if (neighborNodeData.id == neighbor.id) {
                        for (const auto& neighborOfNeighbor : neighborNodeData.neighbors) {
                            if (neighborOfNeighbor.id == self) {
                                isNeighborInRange = true;
                                break;
                            }
                        }
                    }
                }
                if (isNeighborInRange) {
                    // save the distance and choose the best next hop based on total distance from this node to neighbor to CL

                    auto it = std::find_if(cellMembers.begin(), cellMembers.end(), [currentCellId](const CellMemberRecord& m) { return m.id == currentCellId; });
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
                intraCellRoutingTable[member.id][myCellId] = bestNextHopId;
            }
        }
        // trace() << "Intra-cell routing: member " << member.id
        //         << " → cell " << myCellId
        //         << " via next hop " << intraCellRoutingTable[member.id][myCellId];
    }

    // announce the routing table
    announceRoutingTable();
}

void CellularRouting::announceRoutingTable()
{
    NodeRoutingUpdateInfo nodeRoutingUpdateInfo;
    for (const auto& member : cellMembers) {
        // for (int i = 0; i < 6; ++i) {
        //     trace() << "Routing node " << member.id
        //             << " to cell " << neighborCells[i]
        //             << ": next hop " << intraCellRoutingTable[member.id][neighborCells[i]];
        // }
        if (member.id == self) {
            continue; // Skip myself
        }
        // With CL
        int nId = member.id;
        int nFromCell = myCellId;
        int nToCell = myCellId;
        int nNextHop = intraCellRoutingTable[nId][nToCell];
        RoutingUpdateInfo routingUpdateInfo;
        routingUpdateInfo.fromCell = nFromCell;
        routingUpdateInfo.nodeId = nId;
        routingUpdateInfo.toCell = nToCell;
        routingUpdateInfo.nextHop = nNextHop;
        nodeRoutingUpdateInfo.routingInfo[0] = routingUpdateInfo;
        // With other CGW
        for (int i = 0; i < 6; ++i) {
            if (neighborCells[i] != -1) {
                nToCell = neighborCells[i];
                nNextHop = intraCellRoutingTable[nId][nToCell];
                routingUpdateInfo.fromCell = nFromCell;
                routingUpdateInfo.nodeId = nId;
                routingUpdateInfo.toCell = nToCell;
                routingUpdateInfo.nextHop = nNextHop;
                nodeRoutingUpdateInfo.routingInfo[i+1] = routingUpdateInfo;
            }
        }
        routingUpdates.push_back(nodeRoutingUpdateInfo);

        // for (int i = 0; i < 7; ++i) {
        //     trace() << "Routing update for node " << nId
        //              << ": from cell " << nFromCell
        //              << " to cell " << nToCell
        //              << " via next hop " << nNextHop;
        // }
    }
}

void CellularRouting::sendRoutingTableAnnouncementPacket()
{
    // Create and send a routing table announcement packet
    //check if routingUpdates is not empty
    bool isEmpty = true;
    for (const auto& routingUpdate : routingUpdates) {
        if (routingUpdate.routingInfo[0].fromCell == myCellId) {
            isEmpty = false;
        }
    }
    if (!isEmpty) {
        // send the routingUpdateInfo at top of routingUpdates vector
        RoutingUpdateInfo routingUpdateInfo[7];
        for (int i = 0; i < 7; i++) {
            routingUpdateInfo[i] = routingUpdates.front().routingInfo[i];
        }
        CellularRoutingPacket* pkt = new CellularRoutingPacket("Routing Table Announcement", NETWORK_LAYER_PACKET);
        pkt->setPacketType(ROUTING_TREE_UPDATE_PACKET);
        for (int i = 0; i < 7; i++) {
            pkt->setRoutingUpdateData(i, routingUpdateInfo[i]);
        }
        pkt->setTtl(1);
        pkt->setSource(SELF_NETWORK_ADDRESS);
        std::stringstream dest_addr;
        dest_addr << routingUpdateInfo[0].nodeId;
        pkt->setDestination(dest_addr.str().c_str());
        routingUpdates.erase(routingUpdates.begin());
        toMacLayer(pkt, routingUpdateInfo[0].nodeId);
        setTimer(ROUTING_TABLE_UPDATE_TIMER, uniform(1, 10));
        //trace() << "Sent routing table announcement to " << routingUpdateInfo[0].nodeId;
    }
}

void CellularRouting::handleRoutingTableAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If for me: Update my routing table based on the received announcement
    // If not for me: Forward the packet to the destination if in range
    // trace() << "Received routing table announcement from " << pkt->getSource();
    RoutingUpdateInfo routingUpdateInfo[7];
    for (int i=0; i<7; i++) {
        routingUpdateInfo[i] = pkt->getRoutingUpdateData(i);
        // trace() << "NEW update for node " << routingUpdateInfo[i].nodeId
        //          << ": from cell " << routingUpdateInfo[i].fromCell
        //          << " to cell " << routingUpdateInfo[i].toCell
        //          << " via next hop " << routingUpdateInfo[i].nextHop;
        neighborCells[i] = routingUpdateInfo[i].toCell;
        if (routingUpdateInfo[i].fromCell == myCellId) {
            intraCellRoutingTable[routingUpdateInfo[i].nodeId][routingUpdateInfo[i].toCell] = routingUpdateInfo[i].nextHop;
            trace() << "#ROUTING_TABLE " << self << " (" << routingUpdateInfo[i].fromCell << ") -> " << routingUpdateInfo[i].nextHop << " (" << routingUpdateInfo[i].toCell << ")";
        }
    }
    setTimer(FINALIZE_TIMER, uniform(1, 10));
}

void CellularRouting::finalizeRouting()
{
    // Rate routing table
    int count = 0;
    int rightCount = 0;
    int wrongCount = 0;

    for (int i=0; i<7; i++) {
        if (neighborCells[i] == -1 || (neighborCells[i] == 0 &&  self == 0 && intraCellRoutingTable[self][neighborCells[i]] == 0)) {
            continue;
        }
        count++;
        if (intraCellRoutingTable[self][neighborCells[i]] == g_routingTable[self][neighborCells[i]]) {
            rightCount++;
        } else {
            wrongCount++;
        }
    }

    trace() << "Routing table finalized: correct" << rightCount << "/" << count << " wrong " << wrongCount;
}

void CellularRouting::sendCHAnnouncement()
{
    //trace() << "Sending CH Announcement from " << myCellId;
    // If I am the CH, send a CH announcement packet CL
    CellularRoutingPacket* pkt = new CellularRoutingPacket("CH Announcement", NETWORK_LAYER_PACKET);
    pkt->setPacketType(CH_ANNOUNCEMENT_PACKET);
    pkt->setCellSource(myCellId);
    pkt->setCellHopCount(1);
    pkt->setCellDestination(-1);
    pkt->setCellPath(0, myCellId);
    pkt->setTtl(100);
    pkt->setCellSent(myCellId);
    CHAnnouncementInfo chInfo;
    chInfo.chId = self;
    pkt->setChAnnouncementData(chInfo);
    pkt->setSource(SELF_NETWORK_ADDRESS);

    for (int i=0; i<6; i++){
        if (neighborCells[i] == -1) {
            continue;
        }
        CellularRoutingPacket* dupPkt = pkt->dup();
        dupPkt->setCellNext(neighborCells[i]);
        announcementQueue.push({dupPkt, neighborCells[i]});
        // trace() << "Queued CH Announcement for cell " << neighborCells[i]
        //          << " with CH ID " << chInfo.chId;
    }
    setTimer(SEND_ANNOUNCEMENT_QUEUE, uniform(1, 10));
    if (myRole == CELL_LEADER) {
        myCH_id = self;
        for (int i = 0; i < 100; ++i) {
            myCellPathToCH[i] = -1;
        }
        myCellPathToCH[0] = myCellId;
        selectClusterHead();
    }
}

void CellularRouting::sendAnnouncementQueue()
{
    if (!announcementQueue.empty()) {
        auto [pkt, nextCellId] = announcementQueue.front();
        //trace() << "Sending cell packet to " << nextCellId;
        announcementQueue.pop();

        pkt->setTtl(pkt->getTtl() - 1);
        if (pkt->getTtl() <= 0) {
            delete pkt;
            return;
        }

        //trace() << "sending for cell " << nextCellId << " via " << intraCellRoutingTable[self][nextCellId];
        toMacLayer(pkt, intraCellRoutingTable[self][nextCellId]);
    }
}

void CellularRouting::sendCellPacket()
{
    if (!cellPacketQueue.empty()) {
        auto [pkt, nextCellId] = cellPacketQueue.front();


        if (pkt->getPacketType() == SENSOR_DATA) {
            CellularRoutingPacket* dupPkt = pkt->dup();
            double timeNow = simTime().dbl();
            double timeSlot = 600.0;
            double subTimeSlot = 60.0;
            double cycle = timeSlot * 3;
            double subCycle = subTimeSlot * 2;
            double phase = fmod(timeNow, cycle);
            double timeWithinSlot = phase - (myColor * timeSlot);
            double subPhase = fmod(timeWithinSlot, subCycle);

            bool isInTimeSlot = (phase >= myColor * timeSlot) && (phase < (myColor+1) * timeSlot);
            
            bool isInSubTimeSlot;
            switch (levelInCell) {
            case 0:
            case 2:
                if (subPhase < subTimeSlot) {
                    isInSubTimeSlot = true;
                }
                break;

            case 1:
                if (subPhase >= subTimeSlot) {
                    isInSubTimeSlot = true;
                }
                break;
        }

            if (!isInTimeSlot) {
                double nextStart = myColor * timeSlot;
                if (phase >= nextStart) {
                    nextStart += cycle;
                }
                double delay = nextStart - phase;

                setTimer(SEND_CELL_PACKET, delay);
                return;
            }

            if (!isInSubTimeSlot) {
                double delay = fmod(timeWithinSlot, subCycle);

                setTimer(SEND_CELL_PACKET, delay);
                return;
            }
            
            if (nextCellId == -1 || nextCellId == myCellId) {
                cellPacketQueue.pop();

                if (myCH_id != -1) {
                    bool chInRange = false;
                    for (auto &neighborNode : neighborTable) {
                        if (neighborNode.id == myCH_id) {
                            chInRange = true;
                            break;
                        }
                    }
                    
                    if (chInRange) {
                        toMacLayer(dupPkt, myCH_id);
                        trace() << "#SENSOR_DATA: " << self << " -> " << myCH_id;
                    } else {
                        
                        CellularRoutingPacket* dupPkt1 = pkt->dup();
                        toMacLayer(dupPkt1, myCL_id);
                        trace() << "#SENSOR_DATA: " << self << " -> " << myCL_id;
                    }
                } 
                return;
            }
            
            cellPacketQueue.pop();

            pkt->setTtl(pkt->getTtl() - 1);
            pkt->setSource(SELF_NETWORK_ADDRESS);
            if (pkt->getTtl() <= 0) {
                delete pkt;
                return;
            }
            int nextHopId = intraCellRoutingTable[self][nextCellId];
            // check if node in range
            bool isInRange = false;
            bool isInMyCell = true;
            for (auto &neighborNode : neighborTable) {
                if (neighborNode.id == nextHopId) {
                    isInRange = true;
                    isInMyCell = (neighborNode.cellId == myCellId);
                    break;
                }
            }
            
            if (isInRange) {
                trace() << "#SENSOR_DATA: " << self << " -> " << nextHopId;
                toMacLayer(pkt, nextHopId);
            }
        }
    }
}


void CellularRouting::handleCHAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save my cell information to packet metadata
        // and forward the CH announcement to my cell members with destination as all neighbor cells
    int sourceId = atoi(pkt->getSource());
    trace() << "#CELL_MESSAGE " << sourceId << " -> " << self;
    if (myRole == CELL_LEADER) {
        int cellSource = pkt->getCellSource();
        if (myCH_id != -1) return;
        CHAnnouncementInfo chInfo = pkt->getChAnnouncementData();
        int chId = chInfo.chId;
        myCH_id = chId;
        selectClusterHead();
        int hopCount = pkt->getCellHopCount();
        for (int i = 0; i < 100; ++i) {
            myCellPathToCH[i] = pkt->getCellPath(i);
            if (i >= hopCount) {
                myCellPathToCH[i] = -1;
            }
        }
        myCellPathToCH[hopCount] = -1;
        if (myCellPathToCH[hopCount-1] != myCellId) {
            myCellPathToCH[hopCount] = myCellId;
            myCellPathToCH[hopCount+1] = -1;
            pkt->setCellHopCount(hopCount + 1);
            pkt->setCellPath(hopCount, myCellId);
        }

        int cellDestination = pkt->getCellDestination();
        int cellSent = pkt->getCellSent();
        myNextCellHop = myCellPathToCH[hopCount - 1];
        if (hopCount > 1) {
            myNextNextCellHop = myCellPathToCH[hopCount - 2];
        }
        pkt->setCellSent(myCellId);
        pkt->setSource(SELF_NETWORK_ADDRESS);

        if (cellDestination == -1) {
            for (int i=0; i<6; i++) {
                if (neighborCells[i] == -1 || neighborCells[i] == cellSent) {
                    continue;
                }
                CellularRoutingPacket* dupPkt = pkt->dup();
                dupPkt->setCellNext(neighborCells[i]);
                announcementQueue.push({dupPkt, neighborCells[i]});
            }
        }
        setTimer(SEND_ANNOUNCEMENT_QUEUE, uniform(1, 10));
    }

    if (myRole != CELL_LEADER) {
        int cellDestination = pkt->getCellDestination();
        int cellNext = pkt->getCellNext();
        pkt->setSource(SELF_NETWORK_ADDRESS);
        CellularRoutingPacket* dupPkt = pkt->dup();
        announcementQueue.push({dupPkt, cellNext});
        setTimer(SEND_ANNOUNCEMENT_QUEUE, uniform(1, 10));
    }

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

    // Announce members about next cell hop
    setTimer(ANNOUNCE_CELL_HOP_TIMER, uniform(1000, 1500));
}

void CellularRouting::sendCellHopAnnouncementPacket()
{
    // trace() << "**********Cell hop to CH";
    // for (int i = 0; myCellPathToCH[i] != -1 && i < 100; ++i) {
    //     trace() << "  Hop " << i << ": " << myCellPathToCH[i];
    // }

    if (myRole == CELL_LEADER) {
        CellularRoutingPacket* pkt = new CellularRoutingPacket("CH Announcement", NETWORK_LAYER_PACKET);
        pkt->setPacketType(ANNOUNCE_CELL_HOP);
        pkt->setCellSource(myCellId);
        pkt->setCellDestination(myCellId);
        pkt->setClusterHead(myCH_id);
        CellHopAnnouncementInfo pktInfo;
        pktInfo.nextCell = myNextCellHop;
        for (int i = 0; i < 100; ++i) {
            pkt->setCellPath(i, myCellPathToCH[i]);
            //trace() << "Announced cell path: " << i << ": " << myCellPathToCH[i] << " -> " << pkt->getCellPath(i);
        }
        pkt->setCellNextNext(myNextNextCellHop);
        pkt->setCellHopAnnouncementData(pktInfo);
        pkt->setSource(SELF_NETWORK_ADDRESS);
        for (auto& member : cellMembers) {
            if (member.id == self) {
                continue; // Skip myself
            }
            CellularRoutingPacket* dupPkt = pkt->dup();
            dupPkt->setDestination(std::to_string(member.id).c_str());
            toMacLayer(dupPkt, member.id);
            //trace() << "Sent cell hop announcement to member " << member.id;
        }
    }
}

void CellularRouting::handleCellHopAnnouncementPacket(CellularRoutingPacket* pkt)
{
    CellHopAnnouncementInfo pktInfo = pkt->getCellHopAnnouncementData();
    int nextCell = pktInfo.nextCell;
    if (nextCell != -1) {
        myNextCellHop = nextCell;
        // for (int i = 0; i < 100; ++i) {
        //     if (pktInfo.cellPath[i] < 0) break;
        //     myCellPathToCH[i] = pktInfo.cellPath[i];
        // }

        for (int i = 0; i < 100; ++i) {
            myCellPathToCH[i] = pkt->getCellPath(i);
            //trace() << "******received cell path " << i << ": " << myCellPathToCH[i];
        }

        myCH_id = pkt->getClusterHead();
        //trace() << "my CH id: " << myCH_id;

        setTimer(COLOR_SCHEDULING_TIMER, 600*myColor);
    }
}

void CellularRouting::sendSensorDataPacket(){
    CellularRoutingPacket* pkt = new CellularRoutingPacket("Sensing State", NETWORK_LAYER_PACKET);
    pkt->setPacketType(SENSOR_DATA);
    pkt->setCellSource(myCellId);
    pkt->setCellSent(myCellId);
    pkt->setCellNext(myNextCellHop);
    pkt->setCellDestination(myCellPathToCH[0]);
    for (int i = 0; i < 100; ++i) {
        pkt->setCellPath(i, myCellPathToCH[i]);
        //if (pkt->getCellPath(i) != -1) trace() << "Sensor Cell path " << i << ": " << myCellPathToCH[i] << " -> " <<pkt->getCellPath(i);
    }
    pkt->setCellNextNext(myNextNextCellHop);

    SensorData sensorData;
    sensorData.dataId = simTime().dbl();
    g_sensorData[self] = sensorData.dataId;
    sensorData.sensorId = self;
    sensorData.hopCount = 0;
    sensorData.destinationCH = myCH_id;
    pkt->setTtl(100);
    pkt->setSensorData(sensorData);
    pkt->setSource(SELF_NETWORK_ADDRESS);
    for (int i=0; i<2; i++) {
        CellularRoutingPacket* dupPkt = pkt->dup();
        cellPacketQueue.push({dupPkt, myNextCellHop});
    }
    setTimer(SEND_CELL_PACKET, uniform(1, 10));
}

void CellularRouting::handleSensorDataPacket(CellularRoutingPacket* pkt){
    // for (int i=0; i<100; i++){
    //     if (g_sensorDataReceived[i] == -1){
    //         trace() << "receive failed from" << i;
    //     }
    // }

    SensorData sensorData = pkt->getSensorData();
    if (myCH_id == self && myCellId == pkt->getCellDestination()) {
        trace() << "Processing sensor data from  " << sensorData.sensorId << " hop count " << sensorData.hopCount;
        if (g_sensorData[sensorData.sensorId] == sensorData.dataId) {
            g_sensorDataReceived[sensorData.sensorId] = sensorData.dataId;
        }
        return;
    }

    sensorData.hopCount++;
    
    CellularRoutingPacket* dupPkt = pkt->dup();
    int prevCell = dupPkt->getCellSent();

    dupPkt->setCellSent(myCellId);

    if (sensorData.destinationCH == -1) {
        for (int i = 0; i < 100; ++i) {
            dupPkt->setCellPath(i, myCellPathToCH[i]);
            //if (pkt->getCellPath(i) != -1) trace() << "Sensor Cell path " << i << ": " << myCellPathToCH[i] << " -> " <<pkt->getCellPath(i);
        }
        sensorData.destinationCH = myCH_id;
    }

    dupPkt->setSensorData(sensorData);

    int nextCell = -1;
    for (int i = 0; i < 100; ++i) {
        int T = dupPkt->getCellPath(i);
        //trace() << "Cell path " << i << ": " << T;
        if (T == myCellId && i > 0) {
            nextCell = dupPkt->getCellPath(i-1);
            break;
        }
    }
    
    trace() << "***sensor " << sensorData.sensorId << " hc: " << sensorData.hopCount << " to: " << nextCell;
    if (myCellId == nextCell || nextCell == -1) {
        if (prevCell == myCellId) {
            levelInCell = 1;
        } else {
            levelInCell = 2;
        }
    } else {
        levelInCell = 0;
    }
    cellPacketQueue.push({dupPkt, nextCell});
    setTimer(SEND_CELL_PACKET, uniform(1, 10));
}
