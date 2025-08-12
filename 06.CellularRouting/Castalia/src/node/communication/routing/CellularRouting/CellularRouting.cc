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
    // Set timer for STATE_1
}

void CellularRouting::timerFiredCallback(int index)
{
	switch (index) {
		case PRECALCULATE_TIMERS:
			// Precalculate the simulation results
			PrecalculateSimulationResults();
			break;
	}		
    // case STATE_0:
        // Start the SEND_HELLO_TIMER
        // Start the CL_ELECTION_TIMER
        // Start the CL_CONFIRMATION_TIMER
        // Start the GATEWAY_SELECTION_TIMER
        // Start the ROUTING_TREE_CALCULATION_TIMER
        // Start the ROUTING_TABLE_ANNOUNCEMENT_TIMER
        // Start the FINALIZE_ROUTING_TIMER

    // case SEND_HELLO_TIMER:
        // sendHelloPacket();

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
			int row = cellData.cellId / grid_offset;
            int col = cellData.cellId % grid_offset;

            double centerX = cellRadius * 1.5 * col;
            double centerY = sqrt(3.0) * cellRadius * (row + 0.5 * (col % 2));

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
    }
}

void CellularRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void CellularRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    // case HELLO_PACKET:
        // handleHelloPacket();

    // case CL_ANNOUNCEMENT:
        // handleCLAnnouncementPacket();

    // case CL_CONFIRMATION:
        // handleCLConfirmationPacket();

    // case GATEWAY_SELECTION:
        // handleGatewaySelectionPacket();

    // case LINK_REQUEST:
        // handleLinkRequestPacket();

    // case LINK_CONFIRMATION:
        // handleLinkConfirmationPacket();

    // case ROUTING_TABLE_ANNOUNCEMENT:
        // handleRoutingTableAnnouncementPacket();

    // case FINALIZE_ROUTING:
        //
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
    // Broadcast hello packet with my coordinates and cell ID
}

void CellularRouting::handleHelloPacket(CellularRoutingPacket* pkt)
{
    // Save the information from the hello packet to neighbor table
}

void CellularRouting::calculateFitnessScore()
{
    // Calculate fitness score based on id, energy, distance, etc.
    // Or just let the application layer handle it
}

void CellularRouting::sendCLAnnouncement()
{
    // send CL announcement packet to neighbors with my fitness score
}

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // make comparisons with my best fitness score
    // If I have a worse score, cancel my CL_ADVERTISING_TIMER if running
    // Forward the packet to other cell members
}

void CellularRouting::sendCLConfirmationPacket()
{
    // Send confirmation packet to the best candidate CL with my information
}

void CellularRouting::handleCLConfirmationPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save the confirmation and update my cell members
    // If I am not the CL, forward the confirmation to my CL
}

void CellularRouting::gatewaySelection()
{
    // Calculate the best gateway candidates based on distance
    // Or just let the application layer handle it
    // sendGatewaySelectionPacket(); after a random delay
}

void CellularRouting::sendGatewaySelectionPacket()
{
    // Send gateway selection packet to the CGW
}

void CellularRouting::handleGatewaySelectionPacket(CellularRoutingPacket* pkt)
{
    // sendLinkRequestPacket()
}

void CellularRouting::sendLinkRequestPacket()
{
    // Send link request packet to the best candidate NGW
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
