#include "node/communication/routing/ssCellularRouting/SSCellularRouting.h"

Define_Module(SSCellularRouting);

void SSCellularRouting::startup()
{
    myCLId = -1;
    myCHId = -1;
    isCH = false;
    isCL = false;
    myCellId = -1;
    myColor = -1;

    cModule *parentNode = getParentModule();
    numberOfNodes = parentNode->getParentModule()->getParentModule()->par("numNodes");
    myX = parentNode->getParentModule()->par("xCoor");
    myY = parentNode->getParentModule()->par("yCoor");
    isCH = par("isCH");
    maxHopCount = numberOfNodes; 

    if (isCH) {
        myCHId = self;
        if (traceMode == 0) trace() << "#CH " << self;
    }

    SSCRNodeData nodeData;
    nodeData.id = self;
    nodeData.x = myX;
    nodeData.y = myY;
    nodeData.isCH = isCH;
    nodeData.isCL = isCL;
    if (traceMode == 0) trace() << "#NODE " << nodeData.id << " (" << nodeData.x << ", " << nodeData.y << ")";
    calculateCellInfo();
    nodeData.cellId = myCellId;
    nodeData.color = myColor;
    nodeData.clId = myCLId;
    nodeData.chId = myCHId;
    if (traceMode == 0) trace() << "#CELL_COLOR " << self << ": " << myColor;
    g_ssNodesDataList.push_back(nodeData);
        if (!g_isPrecalculated){
            g_isPrecalculated = true;
            setTimer(PRECALCULATE_TIMERS, 100);
        }

    setTimer(RECONFIGURATION_TIMER, 0);
}

void SSCellularRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
    SSCellularRoutingPacket *netPacket = new SSCellularRoutingPacket("SSCellularRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(destination);
    encapsulatePacket(netPacket, pkt);
    toMacLayer(netPacket, resolveNetworkAddress(destination));
}

void SSCellularRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    SSCellularRoutingPacket *netPacket = dynamic_cast<SSCellularRoutingPacket*>(pkt);
    if (!netPacket) {
        trace() << "SSCellularRouting::fromMacLayer() - Error: not a SSCellularRouting packet!";
        delete pkt;
        return;
    }

    switch (netPacket->getPacketType()) {
        case CH_ANNOUNCEMENT_PACKET:
            handleCHAnnouncementPacket(netPacket);
            break;

        case SENSOR_DATA:
            //handleSensorDataPacket(netPacket, srcMacAddress);
            break;

        default:
            trace() << "SSCellularRouting::fromMacLayer() - Error: unknown packet type!";
            delete netPacket;
            break;
    }
}

void SSCellularRouting::timerFiredCallback(int index)
{
    switch (index) {
        case PRECALCULATE_TIMERS:
            PrecalculateSimulationResults();
            break;

        case CH_ADVERTISMENT_TIMER:
            sendCHAnnouncement();
            break;

        case SEND_CELL_PACKET:
            //sendCellPacket();
            //setTimer(SEND_CELL_PACKET, uniform(0, 1));
            break;

        case SENSING_STATE:
            //sendSensorDataPacket();
            //setTimer(SENSING_STATE, 3000); //uniform(9000,10000));
            break;

        case RECONFIGURATION_TIMER:
            // cancelTimer(SENSING_STATE);
            // rotationCH();
            // if (g_lifetime > 0) {
            //     if (myCHId == self) {
            //         myRole = NORMAL_NODE;
            //     }
            //     myCHId = -1;
            //     if (self == g_newCH[g_lifetime][0] || self == g_newCH[g_lifetime][1] || self == g_newCH[g_lifetime][2]) {
            //         myCHId = self;
            //         if (traceMode == 0) trace() << "#CH " << self;
            //     }
            // }
            if (getNodeData(self).isCL) {
                isCL = true;
                myCLId = self;
                trace() << "#CL " << self;
            } 
            myCLId = getCellData(myCLId).clId;
            // setTimer(STATE_1, state1Time);
            setTimer(CH_ADVERTISMENT_TIMER, 200);
            //setTimer(SENSING_STATE, uniform(1000, 1000 + 100));
            // setTimer(RECONFIGURATION_TIMER, reconfigurationTime);
            break;

    }
}

double SSCellularRouting::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

SSCRNodeData* SSCellularRouting::getNodeData(int nodeId) {
    for (auto& nodeData : g_ssNodesDataList) {
        if (nodeData.id == nodeId) {
            return &nodeData;
        }
    }
    SSCRNodeData* emptyData = new SSCRNodeData();
    return emptyData;
}

SSCRCellData SSCellularRouting::getCellData(int cellId) {
    for (const auto& cellData : g_ssCellDataList) {
        if (cellData.cellId == cellId) {
            return cellData;
        }
    }
    SSCRCellData emptyData;
    return emptyData;
}

bool SSCellularRouting::isNodeInList(int nodeId, const vector<int>& nodeList) {
    for (int id : nodeList) {
            if (id == nodeId) {
                return true;
            }
        }
        return false;
}

void SSCellularRouting::PrecalculateSimulationResults()
{
    // for (int i=0; i<numberOfNodes; i++) {
    //     g_sensorData[i] = -1;
    //     g_sensorDataArr[i] = -1;
    //     g_networkConsumption[i][0] = 0;
    //     g_networkConsumption[i][1] = 0;
    // }
    for (auto &node : g_ssNodesDataList) {
    node.neighbors.clear();
    node.clId = -1; 
    }
    g_ssCellDataList.clear();

    // Calculate node neighbors
    for (auto& nodeData : g_ssNodesDataList) {
        for (auto& otherNodeData : g_ssNodesDataList) {
            if (nodeData.id == otherNodeData.id) continue;
            double distance = calculateDistance(nodeData.x, nodeData.y, otherNodeData.x, otherNodeData.y);
            if (distance <= cellRadius) {
                nodeData.neighbors.push_back(otherNodeData.id);
            }
        }
    }

    // Calculate cell members
    for (auto& nodeData : g_ssNodesDataList) {
        // check if the cell already exists, add the node to the cell members
        bool cellExists = false;
        for (auto& cellData : g_ssCellDataList) {
            if (cellData.cellId == nodeData.cellId) {
                cellExists = true;
                cellData.members.push_back(nodeData.id);
                break;
            }
        }
        // else create a new cell and add the node as a member
        if (!cellExists) {
            SSCRCellData cellData;
            cellData.cellId = nodeData.cellId;
            cellData.color = nodeData.color;
            cellData.clId = nodeData.clId;
            cellData.members.push_back(nodeData.id);
            g_ssCellDataList.push_back(cellData);
        }
    }
    //calculate cell neighbors
    for (auto& cellData : g_ssCellDataList) { // for each cell
        int thisCellId = cellData.cellId;
        for (auto& nodeData : g_ssNodesDataList) { // for each node 
            if (nodeData.cellId == thisCellId) { // in the cell
                for (const int neighborId : nodeData.neighbors) { // for each neighbor
                    SSCRNodeData neighbor = getNodeData(neighborId);
                    int neighborCellId = neighbor.cellId;
                    if (neighborCellId != thisCellId) { // if different cell
                        if (!isNodeInList(neighborCellId, cellData.neighbors)) {
                            cellData.neighbors.push_back(neighborCellId);
                            cellData.gateways[neighborCellId] = -1; // Initialize NGW ID
                        }
                    }
                }
            }
        }
    }

    // Calculate cell leaders
    for (auto& cellData : g_ssCellDataList) {
        int bestFitness = -1;
        int bestCLId = -1;
        for (int memberId : cellData.members) {
            SSCRNodeData member = getNodeData(memberId);
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
        for (auto& nodeData : g_ssNodesDataList) {
            if (isNodeInList(nodeData.id, cellData.members)) {
                nodeData.clId = bestCLId;
            }
        }
    }

    // calculate gateway candidates for each pair of cell
    for (auto& cellData : g_ssCellDataList) {
        for (int neighborId : cellData.neighbors) {
            //choose the best pair of CGW and NGW
            double bestDistance = 9999.0;
            int bestCGWId = -1;
            int bestNGWId = -1;
            if (cellData.gateways[neighborId] == -1) { // If NGW ID is not set
                for (auto& nodeData : g_ssNodesDataList) {
                    int nodeCellId = nodeData.cellId;
                    if (nodeCellId == cellData.cellId) {
                        for (int neighborNodeId : nodeData.neighbors) {
                            SSCRNodeData neighborNode = getNodeData(neighborNodeId);
                            //if (cellData.cellId >= neighbor.cellId) continue;
                            if (neighborNode.cellId == neighborId) {
                                double linkDistance = sqrt(pow(nodeData.x - neighborNode.x, 2) + pow(nodeData.y - neighborNode.y, 2));
                                const double EPS = 1e-6;
                                if (linkDistance < bestDistance) {
                                    bestDistance = linkDistance;
                                    bestCGWId = nodeData.id;
                                    bestNGWId = neighborNode.id;
                                }
                                else if (fabs(linkDistance - bestDistance) < EPS) {
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
                    cellData.gateways[neighborId] = bestCGWId;
                    getCellData(neighborId).gateways[cellData.cellId] = bestNGWId;
                }
            }
            // update next hop ID for the CGW
            for (auto& nodeData : g_ssNodesDataList) {
                int nodeDataId = nodeData.id;
                if (nodeDataId == cellData.gateways[neighborId]) {
                    g_ssRoutingTable[nodeDataId][neighborId] = bestNGWId; // Set next hop to NGW
                    if (traceMode == 0) trace() << "#ROUTING_TABLE " << nodeDataId << " (" << cellData.cellId << ") -> " << g_ssRoutingTable[nodeDataId][neighborId] << " (" << neighborId << ")";
                }
            }
        }
    }

    // calculate intra-cell routing table for each cell gateways and CL
    for (auto& cellData : g_ssCellDataList) {
        for (int neighborId : cellData.neighbors) {
            int gatewayId = cellData.gateways[neighborId];
            SSCRNodeData neighbor = getNodeData(neighborId);
            if (gatewayId != -1) {
                SSCRNodeData cellGateway = getNodeData(gatewayId);
                for (int memberId : cellData.members) {
                    SSCRNodeData member = getNodeData(memberId);
                    g_ssRoutingTable[memberId][cellData.cellId] = -1;
                    bool isGatewayInRange = false;
                    if (memberId != gatewayId) {
                        for (const int neighborNodeId : member.neighbors) {
                            SSCRNodeData neighborNode = getNodeData(neighborNodeId);
                            if (neighborNodeId == gatewayId) {
                                isGatewayInRange = true;
                                g_ssRoutingTable[memberId][cellData.cellId] = gatewayId;
                                if (traceMode == 0) trace() << "#ROUTING_TABLE " << memberId << " (" << member.cellId << ") -> " << g_ssRoutingTable[memberId][cellData.cellId] << " (" << cellData.cellId << ")";
                            }

                        }
                        if (!isGatewayInRange) {
                            // If the gateway is not in range, find the best next hop
                            double minDistance = 9999.0;
                            int bestNextHopId = -1;

                            for (const int neighborNodeId : member.neighbors) {
                                bool isNeighborInRange = false;
                                // continue if the neighbor is not in range of the gateway
                                for (const auto& neighborNodeData : g_ssNodesDataList) {
                                    if (neighborNodeData.id == neighborNodeId) {
                                        for (const int neighborOfNeighborId : neighborNodeData.neighbors) {
                                            if (neighborOfNeighborId == gatewayId) {
                                                isNeighborInRange = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (isNeighborInRange) {
                                    // save the distance and choose the best next hop based on total distance from this node to neighbor to gateway
                                    SSCRNodeData neighborNode = getNodeData(neighborNodeId);
                                    double distanceNodeToNeighbor = sqrt(pow(member.x - neighborNode.x, 2) + pow(member.y - neighborNode.y, 2));
                                    double distanceNeighborToGateway = sqrt(pow(neighborNode.x - cellGateway.x, 2) + pow(neighborNode.y - cellGateway.y, 2));
                                    double totalDistance = distanceNodeToNeighbor + distanceNeighborToGateway;
                                    if (totalDistance < minDistance) {
                                        minDistance = totalDistance;
                                        bestNextHopId = neighborNodeId;
                                    } else if (totalDistance == minDistance) {
                                        // If the distance is equal, prefer the one with lower ID
                                        if (neighborNodeId < bestNextHopId) {
                                            bestNextHopId = neighborNodeId;
                                        }
                                    }
                                }
                            }
                            if (bestNextHopId != -1) {
                                g_ssRoutingTable[member.id][cellData.cellId] = bestNextHopId;
                                if (traceMode == 0) trace() << "#ROUTING_TABLE " << member.id << " (" << member.cellId << ") -> " << g_ssRoutingTable[member.id][cellData.cellId] << " (" << cellData.cellId << ")";
                            }
                        }
                    }
                }
            }
        }
        ///////////////
        // routing table for CL
        for (const int memberId : cellData.members) {
            if (memberId == cellData.clId) {
                continue;
            }
            g_ssRoutingTable[memberId][cellData.cellId] = -1;
            SSCRNodeData member = getNodeData(memberId);
            if (isNodeInList(cellData.clId, member.neighbors)) { // if CL is in range
                g_ssRoutingTable[memberId][cellData.cellId] = cellData.clId;
            } else {
                double minDistance = 9999.0;
                int bestNextHopId = -1;

                for (const int neighborId : member.neighbors) {
                    // Check if the neighbor is in range of the CL
                    bool isNeighborInRange = false;
                    for (const auto& neighborNodeData : g_ssNodesDataList) {
                        if (neighborNodeData.id == neighborId) {
                            for (const int neighborOfNeighborId : neighborNodeData.neighbors) {
                                if (neighborOfNeighborId == cellData.clId) {
                                    isNeighborInRange = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (isNeighborInRange) {
                        SSCRNodeData clNode = getNodeData(cellData.clId);
                        SSCRNodeData neighbor = getNodeData(neighborId);
                        double distanceNodeToNeighbor = sqrt(pow(member.x - neighbor.x, 2) + pow(member.y - neighbor.y, 2));
                        double distanceNeighborToCL = sqrt(pow(neighbor.x - clNode.x, 2) + pow(neighbor.y - clNode.y, 2));
                        double totalDistance = distanceNodeToNeighbor + distanceNeighborToCL;
                        if (totalDistance < minDistance) {
                            minDistance = totalDistance;
                            bestNextHopId = neighborId;
                        } else if (totalDistance == minDistance) {
                            // If the distance is equal, prefer the one with lower ID
                            if (neighborId < bestNextHopId) {
                                bestNextHopId = neighborId;
                            }
                        }
                    }
                }
                if (bestNextHopId != -1) {
                    g_ssRoutingTable[member.id][cellData.cellId] = bestNextHopId;
                }
            }
        }
    }
}

Point SSCellularRouting::calculateCellCenter(int cell_id) {
    int r = round((double)cell_id / grid_offset);
    int q = cell_id - r * grid_offset;

    Point center;
    center.x = cellRadius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r);
    center.y = cellRadius * (3.0 / 2.0 * r);

    return center;
}

void SSCellularRouting::calculateCellInfo()
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

    myCellId = q + r * grid_offset;
    myColor = ((q - r) % 3 + 3) % 3;
}

void SSCellularRouting::sendCHAnnouncement()
{
    SSCHAnnouncementInfo chInfo;
    chInfo.chId = self;
    for (const int neighborCellId : getCellData(myCellId).neighbors) {
        SSCellularRoutingPacket* dupPkt = new SSCellularRoutingPacket("CH Announcement", NETWORK_LAYER_PACKET);
        dupPkt->setPacketType(CH_ANNOUNCEMENT_PACKET);
        dupPkt->setCellSource(myCellId);
        dupPkt->setCellHopCount(1);
        dupPkt->setCellDestination(-1);
        dupPkt->setCellPath(0, myCellId);
        dupPkt->setCellPath(1, -1);
        dupPkt->setTtl(maxHopCount);
        dupPkt->setCellSent(myCellId);
        dupPkt->setCHAnnouncementData(chInfo);
        dupPkt->setSource(SELF_NETWORK_ADDRESS);

        int nextHopId = g_ssRoutingTable[self][neighborCellId];
        // check if node in range
        if (isNodeInList(nextHopId, getNodeData(self).neighbors)) {
            //g_networkConsumption[self][0] += calculateConsumption(nextHopId);
            toMacLayer(dupPkt, nextHopId);
        } else {
            delete dupPkt;
        }
    }

    if (isCL) {
        myCHId = self;
        if (traceMode == 0) trace() << "#CH_SELECTION " << self << ": " << myCHId;
        for (int i = 0; i < maxHopCount; ++i) {
            myCellPathToCH[i] = -1;
        }
        myCellPathToCH[0] = myCellId;
        selectClusterHead();
    }
}


void SSCellularRouting::handleCHAnnouncementPacket(SSCellularRoutingPacket* pkt)
{
    // If I am the CL, save my cell information to packet metadata
        // and forward the CH announcement to my cell members with destination as all neighbor cells
    int sourceId = atoi(pkt->getSource());
    if (traceMode == 0) trace() << "#CELL_MESSAGE " << sourceId << " -> " << self;
    if (isCL) {
        if (myCHId != -1) return;
        SSCHAnnouncementInfo chInfo = pkt->getCHAnnouncementData();
        int chId = chInfo.chId;
        myCHId = chId;

        selectClusterHead();
        int hopCount = pkt->getCellHopCount();
        for (int i = 0; i < maxHopCount; ++i) {
            myCellPathToCH[i] = pkt->getCellPath(i);
            if (i >= hopCount) {
                myCellPathToCH[i] = -1;
            }
        }
        
        if (myCellPathToCH[hopCount-1] != myCellId) {
            myCellPathToCH[hopCount] = myCellId;
            pkt->setCellHopCount(hopCount + 1);
            pkt->setCellPath(hopCount, myCellId);
        }

        int cellDestination = pkt->getCellDestination();
        int cellSent = pkt->getCellSent();
        
        pkt->setCellSent(myCellId);
        pkt->setSource(SELF_NETWORK_ADDRESS);

        if (cellDestination == -1) {
            for (int i=0; i<6; i++) {
                if (neighborCells[i] == -1 || neighborCells[i] == cellSent) {
                    continue;
                }
                SSCellularRoutingPacket* dupPkt = pkt->dup();
                dupPkt->setCellNext(neighborCells[i]);
                toMacLayer(dupPkt, g_ssRoutingTable[self][neighborCells[i]]);
            }
        }
        selectClusterHead();
    }

    if (!isCL) {
        int cellDestination = pkt->getCellDestination();
        int cellNext = pkt->getCellNext();
        pkt->setSource(SELF_NETWORK_ADDRESS);
        SSCellularRoutingPacket* dupPkt = pkt->dup();
        toMacLayer(dupPkt, g_ssRoutingTable[self][cellNext]);
    }
}

void SSCellularRouting::selectClusterHead()
{
    // select the closest CH
    // Or just let the application layer handle it
    if (traceMode == 0) trace() << "#CH_SELECTION " << self << ": " << myCHId;
    // Announce members about next cell hop
    //setTimer(ANNOUNCE_CELL_HOP_TIMER, uniform(1000, 1500));
}

