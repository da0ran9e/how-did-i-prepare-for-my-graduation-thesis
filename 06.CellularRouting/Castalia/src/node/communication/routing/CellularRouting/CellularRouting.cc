#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

static std::vector<int> clusterHeadList;

void CellularRouting::startup()
{
    helloInterval = par("helloInterval");
    cellRadius = par("cellRadius");
    myCL_id = -1;
    amI_CL = false;
    myX = par("xCoor");
    myY = par("yCoor");

    bool isSink = par("isSink");
    if (isSink) {
        amI_CH = true;
        myCH_id = self; 
        myRole = CLUSTER_HEAD;
        clusterHeadList.push_back(self);
    }
    myRole = amI_CH ? CLUSTER_HEAD : NORMAL_NODE;
    myCellId = -1; 
    myColor = -1; 
    myNextHopId = -1;
    neighborTable.clear();

    calculateCellInfo();
    grid_offset = par("gridOffset");

    trace() << "[EVENT] Node " << self << " initialized at (" << myX << ", " << myY
            << "). Cell ID: " << myCellId << ", Color: " << myColor
            << ", Is CH: " << (amI_CH ? "Yes" : "No");

    setTimer(SEND_HELLO_TIMER, uniform(helloInterval, helloInterval + 1.0));
    setTimer(RECONFIGURATION_TIMER, 500); //reconfiguration phrase after 500 seconds
}

void CellularRouting::timerFiredCallback(int index)
{
    switch (index) {
        case SEND_HELLO_TIMER: {
            sendHelloPacket();
            setTimer(SEND_HELLO_TIMER, helloInterval);
            break;
        }

        case RECONFIGURATION_TIMER: {
            cancelTimer(SEND_HELLO_TIMER);
            startCLElectionContention();
            break;
        }

        case CL_ANNOUNCEMENT_TIMER: {
            myRole = CELL_LEADER;
            amI_CL = true;
            myCL_id = self;
            sendCLAnnouncement();
            break;
        }

        case CONFIRMATION_SENDER_TIMER: {
            trace() << "Confirmation sender timer fired. Sending CL confirmation.";
            sendCLConfirmationPacket();
            break;
        }

        case CL_CONFIRMATION_TIMER: {
            trace() << "CL confirmation timer fired. I am the CL, waiting for confirmations from cell members.";
            trace() << "Total cell members: " << cellMembers.size();
            trace() << " --> Nodes in cell: ";
            for (const auto& member : cellMembers) {
                trace() << member.id << " at (" << member.x << ", " << member.y
                        << ") With neighbors: ";
                for (const auto& neighbor : member.neighbors) {
                    trace() << "  - Neighbor ID: " << neighbor.id
                            << " at (" << neighbor.x << ", " << neighbor.y
                            << ") Last heard: " << neighbor.lastHeard;
                }
            }
            setTimer(CL_VOTE_CH, 10);
                break;
        }

        case CL_VOTE_CH: {
            trace() << "CL_VOTE_CH timer fired. Starting CL election process.";
            voteCH();
            startReconfiguration();
            break;
        }

        case LINK_REQUEST_TIMEOUT: {
            trace() << "WARNING: Link Request timed out." << index;
            if (!candidates.empty()) {
                candidates.erase(candidates.begin());
                trace() << "Removed the first candidate due to timeout. Remaining candidates: " << candidates.size();
                sendLinkRequest();
            } else {
                trace() << "No more candidates left to process.";
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
        case CL_CONFIRMATION: {
            handleCLConfirmationPacket(netPacket);
            trace() << "Received CL_CONFIRMATION packet from " << netPacket->getSource();
            break;
        }
        case BS_UPDATE_PACKET: {
            // TODO
            trace() << "Received BS_UPDATE_PACKET from " << netPacket->getSource();
            break;
        }
        case LINK_REQUEST: {
            handleLinkRequest(netPacket);
            trace() << "Received LINK_REQUEST from " << netPacket->getSource();
            break;
        }
        case NCL_CONFIRM_PACKET: {
            handleLinkEstablishedConfirmation(netPacket);
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
        case CL_COMMAND_PACKET: {
            handleCLCommandPacket(netPacket);
            break;
        }
        case ROUTING_TREE_UPDATE_PACKET: {
            handleRoutingTableUpdate(netPacket);
            trace() << "Received ROUTING_TREE_UPDATE_PACKET from " << netPacket->getSource();
            break;
        }
        case DATA_PACKET: {
            handleDataPacket(netPacket);
            trace() << "Received DATA_PACKET from " << netPacket->getSource();
            break;
        }
        default: {
            trace() << "WARNING: Received unknown packet type.";
            break;
        }
    }
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

    const int grid_offset = 100;
    myCellId = q + r * grid_offset;
    myColor = ((q - r) % 3 + 3) % 3; //TODO: check here ...
}

void CellularRouting::startCLElectionContention() {
    if (amI_CL || myCL_id != -1) {
        return;
    }

    Point cellCenter = calculateCellCenter(myCellId);
    double myDistanceToCenter = sqrt(pow(myX - cellCenter.x, 2) + pow(myY - cellCenter.y, 2));

    double contentionScalingFactor = 0.005; 
    simtime_t contentionDelay = (contentionScalingFactor * myDistanceToCenter) + uniform(0, 0.001);

    trace() << "[EVENT] My distance to center is " << myDistanceToCenter
            << "m. Will announce myself as CL in " << contentionDelay << "s.";

    setTimer(CL_ANNOUNCEMENT_TIMER, myDistanceToCenter + 50);
}

Point CellularRouting::calculateCellCenter(int cell_id) {
    int r = round((double)cell_id / grid_offset);
    int q = cell_id - r * grid_offset;

    Point center;
    center.x = cellRadius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r);
    center.y = cellRadius * (3.0 / 2.0 * r);

    return center;
}

void CellularRouting::sendHelloPacket() {
    trace() << "[EVENT]Function sendHelloPacket() called.";
        CellularRoutingPacket *netPacket = new CellularRoutingPacket("CellularRouting packet", NETWORK_LAYER_PACKET);
        netPacket->setPacketType(HELLO_PACKET);
        HelloPacketData helloData;
        helloData.x = myX;
        helloData.y = myY;
        helloData.cellId = myCellId;
        netPacket->setHelloData(helloData);
        netPacket->setSource(SELF_NETWORK_ADDRESS);
        netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
        toMacLayer(netPacket, resolveNetworkAddress(BROADCAST_NETWORK_ADDRESS));
}

void CellularRouting::handleHelloPacket(CellularRoutingPacket* pkt) {
    int sourceId = atoi(pkt->getSource());
    
    for (const auto& neighbor : neighborTable) {
        if (neighbor.id == sourceId) {
            return; // Already a neighbor
        }
    }

    HelloPacketData helloData = pkt->getHelloData();
    NeighborRecord newNeighbor;
    newNeighbor.id = sourceId;
    newNeighbor.x = helloData.x;
    newNeighbor.y = helloData.y;
    newNeighbor.cellId = helloData.cellId;
    neighborTable.push_back(newNeighbor);
    trace() << "[EVENT]Received HELLO from " << sourceId << ". Neighbor table updated."
            << " New neighbor: " << newNeighbor.id
            << " at (" << newNeighbor.x << ", " << newNeighbor.y
            << ") in cell " << newNeighbor.cellId;
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
           trace() << "[EVENT]Sending CL announcement to neighbor " << neighborId
                   << " at (" << neighbor.x << ", " << neighbor.y << ")";
       }
}

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt) {
    if (myCellId != pkt->getClAnnouncementData().cellId) {
        return;
    }
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
    setTimer(CONFIRMATION_SENDER_TIMER, myDistanceToCenter+50);
}

void CellularRouting::sendCLConfirmationPacket() {
    if (myCL_id != -1) {
        trace() << "Sending confirmation to " << myCL_id;

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

        confirmPkt->setSource(SELF_NETWORK_ADDRESS);
        std::stringstream dest_addr;
        dest_addr << myCL_id;
        confirmPkt->setDestination(dest_addr.str().c_str());

        toMacLayer(confirmPkt, myCL_id);
    }
}

void CellularRouting::handleCLConfirmationPacket(CellularRoutingPacket* pkt) {
    if (!amI_CL) return;

    NodeInfo senderInfo = pkt->getNodeInfoData();
    int senderId = senderInfo.nodeId;

    if (cellMembers.empty()) {
        trace() << "Received first confirmation, starting CL confirmation timer.";
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
    trace() << "Received confirmation from node " << senderId
            << " at (" << newMember.x << ", " << newMember.y << ") with energy: "
            << newMember.energy;

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
        trace() << "  -> Neighbor: " << neighbor_of_sender_record.id
                << " at (" << neighbor_of_sender_record.x << ", "
                << neighbor_of_sender_record.y << ") with cell ID: "
                << neighbor_of_sender_record.cellId;
    }

    cellMembers.push_back(newMember);

    trace() << "Received confirmation from node " << senderId
            << ". Total confirmed members: " << cellMembers.size();

}

void CellularRouting::voteCH() {
    if (!amI_CL) {
        return;
    }

    trace() << "I am the CL. Starting the process to select the closest CH.";

    if (clusterHeadList.empty()) {
        trace() << "WARNING: Cluster Head list is empty. Cannot select a target CH.";
        myCH_id = -1;
        return;
    }

    int best_ch_id = -1;
    double min_distance_sq = -1.0;

    for (int ch_id : clusterHeadList) {
        if (allNodesPositions.count(ch_id)) {
            Point ch_position = allNodesPositions[ch_id];

            double distance_sq = pow(myX - ch_position.x, 2) + pow(myY - ch_position.y, 2);

            if (best_ch_id == -1 || distance_sq < min_distance_sq) {
                min_distance_sq = distance_sq;
                best_ch_id = ch_id;
            }
        }
    }

    if (best_ch_id != -1) {
        myCH_id = best_ch_id;
        trace() << "Election complete. My cell will route data towards CH " << myCH_id
                << " at distance " << sqrt(min_distance_sq) << "m.";
    } else {
        trace() << "WARNING: Could not find a valid CH from the provided list.";
        myCH_id = -1;
    }
}

void CellularRouting::startReconfiguration() {
    if (!amI_CL) return;

    trace() << "I am CL, starting reconfiguration process...";

    if (myCH_id == -1) {
        trace() << "No target CH found. Reconfiguration cannot proceed.";
        return;
    }

    findAndEstablishInterCellLinks();
}

void CellularRouting::findAndEstablishInterCellLinks() {
    if (!amI_CL) {
        return;
    }
    gatewayTowardsCH = -1;
    trace() << "I am CL, discovering, evaluating, and ranking all potential gateway pairs...";

    if (myCH_id == -1) {
        trace() << "  ERROR: Target CH is not set. Cannot proceed.";
        return;
    }
    candidates.clear();

    Point targetCH_pos = allNodesPositions[myCH_id];
    for (const auto& member : cellMembers) {
        for (const auto& neighbor : member.neighbors) {
            if (neighbor.cellId != myCellId) {
                GatewayCandidate candidate;
                candidate.cgw_id = member.id;
                candidate.ngw_id = neighbor.id;
                candidate.target_cell_id = neighbor.cellId;

                Point cgw_pos = allNodesPositions[member.id];
                Point ngw_pos = allNodesPositions[neighbor.id];

                candidate.link_distance = sqrt(pow(cgw_pos.x - ngw_pos.x, 2) + pow(cgw_pos.y - ngw_pos.y, 2));
                candidate.ngw_to_ch_distance = sqrt(pow(ngw_pos.x - targetCH_pos.x, 2) + pow(ngw_pos.y - targetCH_pos.y, 2));

                candidates.push_back(candidate);
            }
        }
    }

    if (candidates.empty()) {
        trace() << "  No potential inter-cell links found.";
        return;
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const GatewayCandidate& a, const GatewayCandidate& b) {
            if (a.ngw_to_ch_distance < b.ngw_to_ch_distance) {
                return true;
            }
            if (a.ngw_to_ch_distance > b.ngw_to_ch_distance) {
                return false;
            }
            return a.link_distance < b.link_distance;
        });

    trace() << "--- Ranked Gateway Candidates (Best to Worst) ---";
    for (const auto& cand : candidates) {
        trace() << "  Pair: (CGW:" << cand.cgw_id << " -> NGW:" << cand.ngw_id
                << ") | NGW to CH Dist: " << cand.ngw_to_ch_distance << "m"
                << " | Link Dist: " << cand.link_distance << "m";
    }
    trace() << "---------------------------------------------";

    sendLinkRequest();
}

void CellularRouting::sendLinkRequest() {
    if (!amI_CL) {
        return;
    }

    if (!candidates.empty()) {
        GatewayCandidate best_candidate = candidates.front();
        trace() << "Attempting to establish link with the best candidate: CGW "
                << best_candidate.cgw_id << " -> NGW " << best_candidate.ngw_id;
        int best_cgw = best_candidate.cgw_id;
        int best_ngw = best_candidate.ngw_id;
        int best_next_hop_cell = best_candidate.target_cell_id;
        int best_final_ch_id = myCH_id;

        if (best_cgw != -1 && best_ngw != -1) {
            trace() << "  BEST GATEWAY PAIR FOUND: CGW " << best_cgw << " -> NGW " << best_ngw;

            CellularRoutingPacket* cmdPkt = new CellularRoutingPacket("CL Command to CGW", NETWORK_LAYER_PACKET);
            cmdPkt->setPacketType(CL_COMMAND_PACKET);

            CLCommandInfo cmdInfo;
            cmdInfo.source_cgw_id = best_cgw;
            cmdInfo.target_ngw_id = best_ngw;
            cmdInfo.target_cell_id = best_next_hop_cell;
            cmdInfo.final_ch_id = myCH_id;
            cmdPkt->setClCommandData(cmdInfo);
            cmdPkt->setSource(SELF_NETWORK_ADDRESS);
            std::stringstream dest_addr;
            dest_addr << best_cgw;
            cmdPkt->setDestination(dest_addr.str().c_str());

            trace() << "  Sending command to my CGW " << best_cgw << " to link with NGW " << best_ngw;
            toMacLayer(cmdPkt, best_cgw);

            setTimer(LINK_REQUEST_TIMEOUT, 100);
        }
    }
}

void CellularRouting::handleCLCommandPacket(CellularRoutingPacket* pkt) {
    CLCommandInfo cmdData = pkt->getClCommandData();

    CellularRoutingPacket* reqPkt = new CellularRoutingPacket("Link Request", NETWORK_LAYER_PACKET);
    reqPkt->setPacketType(LINK_REQUEST);

    LinkRequestInfo reqInfo;
    reqInfo.source_cell_id = myCellId;
    reqInfo.final_ch_id = cmdData.final_ch_id;
    reqInfo.target_cell_id = cmdData.target_cell_id;
    reqInfo.source_cgw_id = cmdData.source_cgw_id;
    reqInfo.target_ngw_id = cmdData.target_ngw_id;
    reqPkt->setLinkRequestData(reqInfo);

    reqPkt->setSource(SELF_NETWORK_ADDRESS);
    std::stringstream dest_addr;
    dest_addr << cmdData.target_ngw_id;
    reqPkt->setDestination(dest_addr.str().c_str());
    trace() << "Sending LINK_REQUEST to NGW " << cmdData.target_ngw_id << " cell " << cmdData.target_cell_id
            << " with target CH " << cmdData.final_ch_id;

    toMacLayer(reqPkt, cmdData.target_ngw_id);

    setTimer(LINK_ESTABLISHED_CONFIRMATION, 100);
}

void CellularRouting::handleLinkRequest(CellularRoutingPacket* pkt) {
    if (myCL_id == -1) {
        return;
    }

    if (amI_CL) {
        handleLinkAck(pkt);
        return;
    }

    int source_cgw_id = atoi(pkt->getSource());
    trace() << "I am an NGW. Received LINK_REQUEST from CGW " << source_cgw_id
            << ". Forwarding to my CL " << myCL_id;

    CellularRoutingPacket* fwdPkt = pkt->dup();
    fwdPkt->setSource(SELF_NETWORK_ADDRESS);
    std::stringstream dest_addr;
    dest_addr << myCL_id;
    fwdPkt->setDestination(dest_addr.str().c_str());

    toMacLayer(fwdPkt, myCL_id);
}

void CellularRouting::handleLinkAck(CellularRoutingPacket* pkt) {
    trace() << "I am an NCL. Received LINK_REQUEST from NGW " << pkt->getSource();

    LinkRequestInfo linkRq = pkt->getLinkRequestData();

    CellularRoutingPacket* confPkt = new CellularRoutingPacket("Link Request", NETWORK_LAYER_PACKET);
    confPkt->setPacketType(NCL_CONFIRM_PACKET);

    NCLConfirmInfo confInfo;
    confInfo.cgw_id = linkRq.source_cgw_id;
    confInfo.ngw_id = linkRq.target_ngw_id;
    confPkt->setNclCommandData(confInfo);

    confPkt->setSource(SELF_NETWORK_ADDRESS);
    std::stringstream dest_addr;
    dest_addr << linkRq.target_ngw_id;
    confPkt->setDestination(dest_addr.str().c_str());

    trace() << "Confirming LINK_REQUEST to NGW " << pkt->getSource();
    toMacLayer(confPkt, linkRq.target_ngw_id);
}

void CellularRouting::handleLinkEstablishedConfirmation(CellularRoutingPacket* pkt) {
    if (amI_CL) {
        trace() << "Received LINK_ACK from CGW " << pkt->getSource()
                << ". Save the link information and update routing table.";
        int cellDestination;
        NCLConfirmInfo confInfo = pkt->getNclCommandData();
        for (const auto& member : cellMembers) {
            if (member.id == confInfo.cgw_id) {
                for (const auto& neighbor : member.neighbors) {
                    if (neighbor.id == confInfo.ngw_id) {
                        cellDestination = neighbor.cellId;
                    }
                }
            }
        }
        interCellRoutingTable[cellDestination] = confInfo.cgw_id;
        gatewayTowardsCH = confInfo.cgw_id;
        cancelTimer(LINK_REQUEST_TIMEOUT);
        trace() << "Link established with CGW " << confInfo.cgw_id
                << " to NGW " << confInfo.ngw_id;
        announceRoutingTree(gatewayTowardsCH, confInfo.ngw_id);

        calculateAndDistributeIntraCellTree();

        return;
    } else {
        NCLConfirmInfo confInfo = pkt->getNclCommandData();
        if (confInfo.cgw_id == self) {
            trace() << "I am the CGW, forwarding to CL.";
            CellularRoutingPacket* fwdPkt = pkt->dup();
            fwdPkt->setSource(SELF_NETWORK_ADDRESS);
            std::stringstream dest_addr;
            dest_addr << myCL_id;
            fwdPkt->setDestination(dest_addr.str().c_str());
            toMacLayer(fwdPkt, myCL_id);
            trace() << "Forwarded LINK_ACK to CL " << myCL_id;
        } else {
            trace() << "I am an NGW, received LINK_ACK from NCL. Forwarding to my CGW " << confInfo.cgw_id;
            CellularRoutingPacket* fwdPkt = pkt->dup();
            fwdPkt->setSource(SELF_NETWORK_ADDRESS);
            std::stringstream dest_addr;
            dest_addr << confInfo.cgw_id;
            fwdPkt->setDestination(dest_addr.str().c_str());
            toMacLayer(fwdPkt, confInfo.cgw_id);
            trace() << "Forwarded LINK_ACK to CGW " << confInfo.cgw_id;
        }
        return;
    }
}

void CellularRouting::calculateAndDistributeIntraCellTree() {
    if (!amI_CL) return;

    if (gatewayTowardsCH == -1) {
        gatewayTowardsCH = myCH_id;
    }

    for (const auto& member : cellMembers) {
        if (member.id == myCH_id) {
            gatewayTowardsCH = myCH_id;
            break;
        }
    }

    Point gatewayPos = allNodesPositions[gatewayTowardsCH];

    for (const auto& member : cellMembers) {
        if (member.id == gatewayTowardsCH) {
            intraCellRoutingTable[member.id] = -1;
            continue;
        }

        int bestHop = -1;
        double minDistanceToGatewaySq = -1.0;
        for (const auto& neighbor : member.neighbors) {
            if (neighbor.cellId == myCellId) {
                double distSq;
                if (neighbor.id == myCH_id) distSq = 0.0;
                
                distSq = pow(allNodesPositions[neighbor.id].x - gatewayPos.x, 2) +
                                pow(allNodesPositions[neighbor.id].y - gatewayPos.y, 2);

                if (bestHop == -1 || distSq < minDistanceToGatewaySq) {
                    minDistanceToGatewaySq = distSq;
                    bestHop = neighbor.id;
                }
            }
        }

        intraCellRoutingTable[member.id] = bestHop;

        if (bestHop != -1) {
            announceRoutingTree(member.id, bestHop);
        } else {
            trace() << "  WARNING: Node " << member.id << " has no neighbors in this cell to forward packets.";
        }
    }

    int bestHopForCL = -1;
    double minDistanceForCLSq = -1.0;

    for (const auto& neighbor : neighborTable) {
         if (neighbor.cellId == myCellId) {
            double distSq = pow(neighbor.x - gatewayPos.x, 2) +
                            pow(neighbor.y - gatewayPos.y, 2);

            if (bestHopForCL == -1 || distSq < minDistanceForCLSq) {
                minDistanceForCLSq = distSq;
                bestHopForCL = neighbor.id;
            }
        }
    }
    
    myNextHopId = bestHopForCL;
    
    if (myNextHopId != -1) {
         trace() << "[ROUTING] " << self << " to "<< myNextHopId;
    } else {
         trace() << "  WARNING: I (CL) have no intra-cell neighbors to forward packets.";
    }
}

double CellularRouting::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void CellularRouting::announceRoutingTree(int from, int to) {
    CellularRoutingPacket* pkt = new CellularRoutingPacket("Routing Update", NETWORK_LAYER_PACKET);
    pkt->setPacketType(ROUTING_TREE_UPDATE_PACKET);

    RoutingUpdateInfo updateInfo;
    updateInfo.from = from;
    updateInfo.to = to;
    pkt->setRoutingUpdateData(updateInfo);


    pkt->setSource(SELF_NETWORK_ADDRESS);
    std::stringstream dest_addr;
    dest_addr << to;
    pkt->setDestination(dest_addr.str().c_str());


    toMacLayer(pkt, from);
    trace() << "Announcing routing tree from " << from << " to " << to;
}

void CellularRouting::handleRoutingTableUpdate(CellularRoutingPacket* pkt) {
    RoutingUpdateInfo updateInfo = pkt->getRoutingUpdateData();
    int from = updateInfo.from;
    int to = updateInfo.to;
    trace() << "[ROUTING] " << from << " to " << to;
    myNextHopId = to;
}

void CellularRouting::handleDataPacket(CellularRoutingPacket* pkt) {
    if (myRole == CELL_LEADER) {
        trace() << "I am a CL, forwarding data packet to my next hop " << myNextHopId;
        pkt->setSource(SELF_NETWORK_ADDRESS);
        std::stringstream dest_addr;
        dest_addr << myNextHopId;
        pkt->setDestination(dest_addr.str().c_str());
        toMacLayer(pkt, myNextHopId);
    } else {
        trace() << "I am a normal node, processing data packet.";
        // Normal node processing logic here
    }
}
