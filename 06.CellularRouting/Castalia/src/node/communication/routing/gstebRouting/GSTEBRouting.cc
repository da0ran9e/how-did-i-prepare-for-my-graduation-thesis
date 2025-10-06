#include "node/communication/routing/gstebRouting/GSTEBRouting.h"

Define_Module(GSTEBRouting);

struct GSTEBNodeNeighborInfo {
    int id;
    vector<int> neighborsNeighborIds;
};
struct GSTEBNodeInfo {
    int id;
    int x;
    int y;
    double el;
    vector<GSTEBNodeNeighborInfo> neighbors;
};
static vector<GSTEBNodeInfo> nodesInfo;

void GSTEBRouting::startup()
{
    // Initialize parameters: coordinates, communication interval, routing table, etc.
    nodeId = self;
    cModule *parentNode = getParentModule();
    xCoor = parentNode->getParentModule()->par("xCoor");
    yCoor = parentNode->getParentModule()->par("yCoor");
    energy = 12000;
    isCH = par("isCH");
    if (isCH) {
        chId = self;
        trace() << "#CH " << self;
    }

    GSTEBNodeInfo nodeInfo;
    nodeInfo.id = nodeId;
    nodeInfo.x = xCoor;
    nodeInfo.y = yCoor;
    nodeInfo.el = energy;
    nodesInfo.push_back(nodeInfo);
    alpha = par("alpha").doubleValue();

    chId = -1;

    trace() << "#NODE " << self << " (" << xCoor << ", " << yCoor << ")";

    setTimer(INITIAL_PHRASE, 20);
}

void GSTEBRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void GSTEBRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    GSTEBRoutingPacket* netPacket = dynamic_cast<GSTEBRoutingPacket*>(pkt);
    if (!netPacket) {
        return;
    }
    netPacket->setTtl(netPacket->getTtl() - 1);
    switch (netPacket->getPacketType()) {
    case BS_BROADCAST_PACKET: {
        handleBSBroadcast(netPacket);
        break;
    }

    case SENSOR_BROADCAST_PACKET: {
        handleSensorBroadcast(netPacket);
        break;
    }

    case NEIGHBOR_BROADCAST_PACKET: {
        handleNeighborsTable(netPacket);
        break;
    }

    case NODE_CONTROL_PACKET: {
        handleRoutingTree(netPacket);
        break;
    }

    case NODE_INFO_PACKET: {
        handleInfoFromNode(netPacket);
        break;
    }

    default:
        break;
    }
}

void GSTEBRouting::timerFiredCallback(int index)
{
    switch (index) {
    case INITIAL_PHRASE: {
        if (isCH) {
            setTimer(BS_BROADCAST, 1);
        }
        break;
    }

    case BS_BROADCAST: {
        setTimer(TREE_CONSTRUCTION_PHASE, 20000); //initialPhraseTimeout); // Broadcast every 10 seconds
        sendBSBroadcast();
        break;
    }

    case PHRASE_I_TIMESLOT: {
        //setTimer(PHRASE_I_TIMESLOT, phaseITimeslot);
        sendSensorBroadcast();
        break;
    }

    case SENSOR_BROADCAST_TIMEOUT: {
        // After all nodes send their information, each
        // node records a table in their memory which contains
        // the information of all its neighbors

        sendNeighborsTable();
        break;
    }

    case TREE_CONSTRUCTION_PHASE: {
        if (isCH) {
            // BS always assigns itself as root
            // BS can collect the initial EL and coordinates
            // information of all the sensor nodes in Initial Phase. For each
            // round, BS builds the routing tree and the schedule of the
            // network by using the EL and coordinates information.
            calculateRoutingTree();
            //broadcastRoutingTree();
        } else {
            // Each node tries to select a parent in its neighbors
            // using EL and coordinates which are recorded in
            // Table I. The selection criteria are:
                // 1. distance between its parent node and the root
                // should be shorter than that between itself and the
                // root

                // 2. only the
                // nodes with the largest EL of all its neighbors
                // and itself can act as relay nodes
                // The relay node which causes
                // minimum consumption will be chosen as the
                // parent node.

            // If the sensor node cannot
            // find a suitable parent node, it will transmit its
            // data directly to BS.
            sendInfoToBS();
            setTimer(DATA_COLLECTING_PHASE, 100);

        }
        // Because every node chooses the parent from its
        // neighbors and every node records its neighborsâ€™
        // information in Table II, each node can
        // know all its neighborsâ€™ parent nodes by computing,
        // and it can also know all its child nodes. If a node
        // has no child node, it defines itself as a leaf node,
        // from which the data transmitting begins.
        break;
    }

    case DATA_COLLECTING_PHASE: {
        // Each node sends its data to its parent node
        // and finally to BS along the tree constructed
        // in Step 4.
        break;
    }

    default:
        break;
    }
}

void GSTEBRouting::sendBSBroadcast()
{
    // BS broadcasts a packet to all the nodes to inform them of
    // beginning time, the length of time slot and the number of nodes N
    GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(BS_BROADCAST_PACKET);
    numNodes = getParentModule()->getParentModule()->getParentModule()->par("numNodes");
    GSTEBBSBroadcastInfo bSBroadcastData;
    bSBroadcastData.x = xCoor;
    bSBroadcastData.y = yCoor;
    bSBroadcastData.numNodes = numNodes;
    bSBroadcastData.timeSlot = timeSlot;
    bSBroadcastData.timeStart = timeStart;
    bSBroadcastData.senderX = xCoor;
    bSBroadcastData.senderY = yCoor;
    bSBroadcastData.sinkId = self;
    netPacket->setBSBroadcastData(bSBroadcastData);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
    netPacket->setTtl(numNodes);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
    //trace() << "BS broadcast " << numNodes;
}
void GSTEBRouting::handleBSBroadcast(GSTEBRoutingPacket* pkt)
{
    // they will compute their own energy-level (EL) using function
    // EL(i) = [residual energy(i) / alpha]

        // EL is a parameter for load balance,
        // and it is an estimated energy value
        // rather than a true one and only used in Case2

        // i is the ID of each node

        // is a constant which reflects the minimum energy unit
        // and can be changed depending on our demands
    //trace() << "recevice bc " << pkt->getSource();
    computeEL();
    if (chId != -1 || isCH) return;

    GSTEBBSBroadcastInfo bSBroadcastData = pkt->getBSBroadcastData();
    if (calculateDistance(bSBroadcastData.senderX, bSBroadcastData.senderY, xCoor, yCoor) > communicationRadius) return;
    chX = bSBroadcastData.x;
    chY = bSBroadcastData.y;
    chId = bSBroadcastData.sinkId;
    numNodes = bSBroadcastData.numNodes;
    timeSlot = bSBroadcastData.timeSlot;
    bSBroadcastData.senderX = xCoor;
    bSBroadcastData.senderY = yCoor;

    trace() << "#BS_BROADCAST " << pkt->getSource() << " -> " << self;

    setTimer(PHRASE_I_TIMESLOT, nodeId*timeSlot+10);
    setTimer(SENSOR_BROADCAST_TIMEOUT, (nodeId+numNodes)*timeSlot+10);
    setTimer(TREE_CONSTRUCTION_PHASE, (nodeId+2*numNodes)*timeSlot+10);

    GSTEBRoutingPacket *dupPkt = pkt->dup();
    dupPkt->setTtl(pkt->getTtl());
    dupPkt->setBSBroadcastData(bSBroadcastData);
    dupPkt->setSource(SELF_NETWORK_ADDRESS);
    if (dupPkt->getTtl() > 0) {
        toMacLayer(dupPkt, BROADCAST_MAC_ADDRESS);
        //trace() << "BS broadcast " << self;
    }

    //delete pkt;
}

void GSTEBRouting::computeEL()
{
    // EL(i) = [residual energy(i) / alpha]
    myEL = energy / alpha;
}

void GSTEBRouting::sendSensorBroadcast()
{
    // Each node sends its packet in a circle with a certain
    // radius during its own time slot
        // This packet contains a preamble
        // and the information such as coordinates and
        // EL of node i
    GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(SENSOR_BROADCAST_PACKET);
    GSTEBSensorBroadcastInfo sensorBroadcastData;
    sensorBroadcastData.nX = xCoor;
    sensorBroadcastData.nY = yCoor;
    sensorBroadcastData.nEL = myEL;
    netPacket->setSensorBroadcastData(sensorBroadcastData);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
    netPacket->setTtl(1);
    toMacLayer(netPacket, BROADCAST_MAC_ADDRESS);
}

void GSTEBRouting::handleSensorBroadcast(GSTEBRoutingPacket* pkt)
{
    // neighbors of node i, they can receive this packet
    // and record the information of node i in memory
    int sourceId = atoi(pkt->getSource());
    for (const auto& neighbor : tableI) {
        if (neighbor.nId == sourceId) {
            return; // already recorded
        }
    }

    GSTEBSensorBroadcastInfo sensorBroadcastData = pkt->getSensorBroadcastData();
    GSTEBNeighbors newNeighbor;
    newNeighbor.nId = sourceId;
    newNeighbor.nX = sensorBroadcastData.nX;
    newNeighbor.nY = sensorBroadcastData.nY;
    newNeighbor.nEL = sensorBroadcastData.nEL;
    // if neighbor is far away, consider it for removal
    if (calculateDistance(xCoor, yCoor, newNeighbor.nX, newNeighbor.nY) > communicationRadius) {
        return;
    }

    tableI.push_back(newNeighbor);
    //trace() << "neighbor " << sourceId;
}

double GSTEBRouting::calculateDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void GSTEBRouting::sendNeighborsTable()
{
    // Each node sends a packet which contains all its
    // neighborsâ€™ information during its own time slot
    // when Step 2 is over.
    GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(NEIGHBOR_BROADCAST_PACKET);

    netPacket->setNnNumber(tableI.size());
    for (size_t i = 0; i < tableI.size(); ++i) {
        netPacket->setNnId(i, tableI[i].nId);
        netPacket->setNnXCoor(i, tableI[i].nX);
        netPacket->setNnYCoor(i, tableI[i].nY);
        netPacket->setNnEL(i, tableI[i].nEL);
    }
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
    netPacket->setTtl(1);
    for (const auto& neighb : tableI){
        GSTEBRoutingPacket *dupPkt = netPacket->dup();
        toMacLayer(dupPkt, neighb.nId);
    }
//    toMacLayer(netPacket, -1);
}

void GSTEBRouting::handleNeighborsTable(GSTEBRoutingPacket* pkt)
{
    // Then its neighbors can receive
    // this packet and record the information in memory.
    int senderId = atoi(pkt->getSource());

    int nnNumber = pkt->getNnNumber();
    for (int i = 0; i < nnNumber; ++i) {
        GSTEBNeighborsOfNeighbors nnInfo;
        nnInfo.neighborId = senderId;
        nnInfo.nnId = pkt->getNnId(i);
        nnInfo.nnX = pkt->getNnXCoor(i);
        nnInfo.nnY = pkt->getNnYCoor(i);
        nnInfo.nnEL = pkt->getNnEL(i);
//        int distance = calculateDistance(xCoor, yCoor, nnInfo.nnX, nnInfo.nnY);
//        if (distance > communicationRadius) return;
        tableII.push_back(nnInfo);
        //trace() << "nn " << nnInfo.nnId;
    }
}

double GSTEBRouting::calcTxEnergy(int kBits, double distance) {
    // These params should exist as members or be read from par()
    // e.g., E_elec = par("E_elec"); eps_fs = par("eps_fs"); ...
    double E_elec = 50e-9;       // example: 50 nJ/bit
    double eps_fs  = 10e-12;     // example: 10 pJ/bit/m^2
    double eps_mp  = 0.0013e-12; // example for mp (you should set real values)
    double d0 = std::sqrt(eps_fs/eps_mp);

    if (distance < d0) {
        return kBits * (E_elec + eps_fs * distance * distance);
    } else {
        return kBits * (E_elec + eps_mp * pow(distance, 4));
    }
}

void GSTEBRouting::chooseRelayNode() {
    // For each neighbor, check if it satisfies the conditions
    // If yes, calculate the energy consumption to send data via this neighbor
    // Choose the neighbor with minimum energy consumption as relay node

    for (const auto& neighbor : tableI) {
        double distToSink = calculateDistance(xCoor, yCoor, chX, chY);
        double myDistToSink = calculateDistance(xCoor, yCoor, chX, chY);
        if (distToSink >= myDistToSink) {
            continue; // Condition 1 not satisfied
        }

        bool isRelayCandidate = true;
        double highestEL = neighbor.nEL;

        for (const auto& nnRecord : tableII) {
            if (nnRecord.neighborId == neighbor.nId) {
                if (nnRecord.nnEL > highestEL) {
                    isRelayCandidate = false;
                    highestEL = nnRecord.nnEL;
                    break;
                }
            }
        }

        if (!isRelayCandidate) {
            continue; // Condition 2 not satisfied
        }

        relayCandidates.push_back(neighbor);
    }

    if (relayCandidates.empty()) {
        // if BS in range, send directly to BS
        for (const auto& neighbor : tableI) {
            if (neighbor.nId == chId) {
                parentId = chId;
                return;
            }
        }
        // else, choose the closest neighbor to BS
        double minDistToSink = 9999.0;
        int bestNeighborId = -1;
        for (const auto& neighbor : tableI) {
            double distToSink = calculateDistance(neighbor.nX, neighbor.nY, chX, chY);
            if (distToSink < minDistToSink) {
                minDistToSink = distToSink;
                bestNeighborId = neighbor.nId;
            }
        }
        if (bestNeighborId != -1) {
            parentId = bestNeighborId;
        }
    }
}

void GSTEBRouting::chooseParentNode() {
    // For each relay candidate, calculate the energy consumption
    // Choose the one with minimum energy consumption as parent node
    int minConsumption = 9999;
    int bestParentId = -1;

    for (const auto& candidate : relayCandidates) {
        double distanceToCandidate = calculateDistance(xCoor, yCoor, candidate.nX, candidate.nY);
        double distanceCandidateToSink = calculateDistance(candidate.nX, candidate.nY, chX, chY);
        double consumption = calcTxEnergy(2000, distanceToCandidate) + calcTxEnergy(2000, distanceCandidateToSink); // assuming 2000 bits

        if (consumption < minConsumption) {
            minConsumption = consumption;
            bestParentId = candidate.nId;
        }
    }

    if (bestParentId != -1) {
        parentId = bestParentId;
    }
}

void GSTEBRouting::sendInfoToBS()
{
    GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(NODE_INFO_PACKET);

    netPacket->setNNumber(tableI.size());
    netPacket->setNnNumber(tableII.size());
    for (size_t i = 0; i < tableI.size(); ++i) {
        netPacket->setNId(i, tableI[i].nId);
        netPacket->setNXCoor(i, tableI[i].nX);
        netPacket->setNYCoor(i, tableI[i].nY);
        netPacket->setNEL(i, tableI[i].nEL);
    }
    for (size_t i = 0; i < tableII.size(); ++i) {
        netPacket->setNnId(i, tableII[i].nnId);
        netPacket->setNNeighbor(i, tableII[i].neighborId);
        netPacket->setNnXCoor(i, tableII[i].nnX);
        netPacket->setNnYCoor(i, tableII[i].nnY);
        netPacket->setNnEL(i, tableII[i].nnEL);
    }
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    double maxDistance = -1;
    for (const auto& node : tableI) {
        double distance = calculateDistance(xCoor, yCoor, node.nX, node.nY);
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }
    std::stringstream dest_addr;
    dest_addr << chId;
    netPacket->setDestination(dest_addr.str().c_str());
    netPacket->setTtl(numNodes);
    toMacLayer(netPacket, chId);
    //trace() << "send info to " << chId;
}

void GSTEBRouting::handleInfoFromNode(GSTEBRoutingPacket* pkt)
{
    // BS receives the packets from all the nodes
    // and records all the information in memory.
    int senderId = atoi(pkt->getSource());
    int nNumber = pkt->getNNumber();

    for (int i = 0; i < nNumber; ++i) {
        GSTEBNeighbors newNode;
        newNode.nodeId = senderId;
        newNode.nNumber = nNumber;
        newNode.nId = pkt->getNId(i);
        newNode.nX = pkt->getNXCoor(i);
        newNode.nY = pkt->getNYCoor(i);
        newNode.nEL = pkt->getNEL(i);
        networkTableI.push_back(newNode);
    }
    trace() << "Set of neighbor " << networkTableI.size();
}

void GSTEBRouting::calculateRoutingTree()
{
    // BS builds the routing tree and the schedule of the
    // network by using the EL and coordinates information
    // collected in Initial Phase.
    // The routing tree is built in a top-down manner,
    // starting from BS. BS first selects its child nodes
    // from all the nodes in the network, then each child
    // node selects its own child nodes from its neighbors,
    // and so on and so forth until all the nodes are
    // included in the tree.

    // The selection criteria are:
        // 1. distance between its parent node and the root
        // should be shorter than that between itself and the
        // root

        // 2. only the
        // nodes with the largest EL of all its neighbors
        // and itself can act as relay nodes
        // The relay node which causes
        // minimum consumption will be chosen as the
        // parent node.

    // Because every node chooses the parent from its
        // neighbors and every node records its neighbors’
        // information in Table II, each node can
        // know all its neighbors’ parent nodes by computing,
        // and it can also know all its child nodes. If a node
        // has no child node, it defines itself as a leaf node,
        // from which the data transmitting begins.
    trace() << "*****Start calc routing tree";
    SortByNumber sortByNumber;
    std::sort(networkTableI.begin(), networkTableI.end(), sortByNumber);
    vector<int> calculatedNodeIds;
    for (const auto& node : networkTableI) {
        //trace() << networkTableI.size();
        int calculatingNode = node.nodeId;
        //trace() << "Calc " << calculatingNode;
        if (std::find_if(calculatedNodeIds.begin(), calculatedNodeIds.end(), [calculatingNode](const int m) { return m == calculatingNode; }) != calculatedNodeIds.end()) continue;
        //trace() << "Calc " << calculatingNode;
        calculatedNodeIds.push_back(calculatingNode);
        vector<GSTEBNeighbors> nRelayCandidates;
        int nParentId = -1;
        // find relay candidates
        for (const auto& curNode : networkTableI){
            if (curNode.nodeId == calculatingNode){
                double distToSink = calculateDistance(xCoor, yCoor, curNode.nX, curNode.nY);
                double nDistToSink = calculateDistance(xCoor, yCoor, node.nX, node.nY);
                if (distToSink >= nDistToSink) {
                    continue; // Condition 1 not satisfied
                }

                bool isRelayCandidate = true;
                double highestEL = curNode.nEL;

                for (const auto& curNNode : networkTableI) {
                    if (curNNode.nodeId == curNode.nId) {
                        if (curNNode.nEL > highestEL) {
                            isRelayCandidate = false;
                            highestEL = curNNode.nEL;
                            break;
                        }
                    }
                }

                if (!isRelayCandidate) {
                    continue; // Condition 2 not satisfied
                }

                nRelayCandidates.push_back(curNode);
            }
        }

        if (nRelayCandidates.empty()) {
            // if BS in range, send directly to BS
            if (calculateDistance(xCoor, yCoor, node.nX, node.nY) <= communicationRadius)
            {
                nParentId = chId;
                continue;
            }

            // else, choose the closest neighbor to BS
            double minDistToSink = 9999.0;
            int bestNeighborId = -1;
            for (const auto& curNode : networkTableI) {
                if (curNode.nodeId != calculatingNode) continue;
                double distToSink = calculateDistance(curNode.nX, curNode.nY, xCoor, yCoor);
                if (distToSink < minDistToSink) {
                    minDistToSink = distToSink;
                    bestNeighborId = curNode.nId;
                }
            }
            if (bestNeighborId != -1) {
                nParentId = bestNeighborId;
            }
        } else {
            // For each relay candidate, calculate the energy consumption
            // Choose the one with minimum energy consumption as parent node
            int minConsumption = 9999;
            int bestParentId = -1;

            for (const auto& curNode : networkTableI) {
                if (curNode.nodeId != calculatingNode) continue;
                double distanceToCandidate = calculateDistance(node.nX, node.nY, curNode.nX, curNode.nY);
                double distanceCandidateToSink = calculateDistance(xCoor, yCoor, curNode.nX, curNode.nY);
                double consumption = calcTxEnergy(2000, distanceToCandidate) + calcTxEnergy(2000, distanceCandidateToSink); // assuming 2000 bits

                if (consumption < minConsumption) {
                    minConsumption = consumption;
                    bestParentId = curNode.nId;
                }
            }

            if (bestParentId != -1) {
                nParentId = bestParentId;
            }
        }
        networkParentTable[calculatingNode] = nParentId;
        trace() << "parent of " << calculatingNode << ": " << nParentId;
    }
}

void GSTEBRouting::broadcastRoutingTree()
{
    // BS broadcasts the routing tree to all the nodes
    // in the network.
    GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(NODE_CONTROL_PACKET);
    // set routing tree data
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
    netPacket->setTtl(numNodes);
    toMacLayer(netPacket, -1);
}

void GSTEBRouting::handleRoutingTree(GSTEBRoutingPacket* pkt)
{
    // each node receives the routing tree
    // and records its parent node and child nodes
    // in memory.
    setTimer(DATA_COLLECTING_PHASE, 100);
}
