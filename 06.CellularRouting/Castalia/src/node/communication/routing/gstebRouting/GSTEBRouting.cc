#include "node/communication/routing/gstebRouting/GSTEBRouting.h"

Define_Module(GSTEBRouting);

struct NodeNeighborInfo {
	int id;
	vector<int> neighborsNeighborIds;
};
struct NodeInfo {
	int id;
	int x;
	int y;
	double el;
	vector<NodeNeighborInfo> neighbors;
};
static vector<NodeInfo> nodesInfo;

void GSTEBRouting::startup()
{
	// Initialize parameters: coordinates, communication interval, routing table, etc.
	nodeId = self;
	cModule *parentNode = getParentModule();
	xCoor = parentNode->getParentModule()->par("xCoor");
	yCoor = parentNode->getParentModule()->par("yCoor");
	energy = par("initialEnergy");
	isSink = par("isSink");
	NodeInfo nodeInfo;
	nodeInfo.id = nodeId;
	nodeInfo.x = xCoor;
	nodeInfo.y = yCoor;
	nodeInfo.el = energy;
	nodesInfo.push_back(nodeInfo);
	alpha = par("alpha").doubleValue();

	chId = -1;

	if (isSink) {
		trace() << "#SINK " << self;
	}

	setTimer(INITIAL_PHRASE, 1);
}

void GSTEBRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void GSTEBRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{

}

void GSTEBRouting::timerFiredCallback(int index)
{
	switch (index) {
	case INITIAL_PHRASE: {
		if (isSink) {
			setTimer(BS_BROADCAST, 1);
		}
		break;
	}

	case BS_BROADCAST: {
		setTimer(TREE_CONSTRUCTION_PHASE, initialPhraseTimeout); // Broadcast every 10 seconds
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
		if (isSink) {
			// BS always assigns itself as root
			// BS can collect the initial EL and coordinates
			// information of all the sensor nodes in Initial Phase. For each
			// round, BS builds the routing tree and the schedule of the
			// network by using the EL and coordinates information. 
			calculateRoutingTree();
			broadcastRoutingTree();
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
 		// neighbors and every node records its neighbors’
		// information in Table II, each node can
		// know all its neighbors’ parent nodes by computing,
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
    BSBroadcastInfo bSBroadcastData;
	bSBroadcastData.x = xCoor;
	bSBroadcastData.y = yCoor;
    bSBroadcastData.numNodes = numNodes;
    bSBroadcastData.timeSlot = timeSlot;
    bSBroadcastData.timeStart = timeStart;
    netPacket->setBSBroadcastData(bSBroadcastData);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
	netPacket->setTtl(numNodes);
    toMacLayer(netPacket, BROADCAST_NETWORK_ADDRESS);
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
	computeEL();
	if (chId != -1) return;
	int chId = atoi(pkt->getSource());

    BSBroadcastInfo bSBroadcastData = pkt->getBSBroadcastData();
    chX = bSBroadcastData.x;
	chY = bSBroadcastData.y;
	numNodes = bSBroadcastData.numNodes;
	timeSlot = bSBroadcastData.timeSlot;

	setTimer(PHRASE_I_TIMESLOT, nodeId*timeSlot+10);
	setTimer(SENSOR_BROADCAST_TIMEOUT, (nodeId+numNodes)*timeSlot+10);
	setTimer(TREE_CONSTRUCTION_PHASE, (nodeId+2*numNodes)*timeSlot+10);

	GSTEBRoutingPacket *dupPkt = pkt->dup();
	dupPkt->setTtl(pkt->getTtl()-1);
	if (dupPkt->getTtl() > 0) toMacLayer(dupPkt, BROADCAST_NETWORK_ADDRESS);
	delete pkt;
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
    SensorBroadcastInfo sensorBroadcastData;
    sensorBroadcastData.x = nX;
    sensorBroadcastData.y = nY;
    sensorBroadcastData.EL = nEL;
    netPacket->setSensorBroadcastData(sensorBroadcastData);
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
	netPacket->setTtl(1);
    toMacLayer(netPacket, BROADCAST_NETWORK_ADDRESS);
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
	
    SensorBroadcastInfo sensorBroadcastData = pkt->getSensorBroadcastData();
	GSTEBNeighbors newNeighbor;
    newNeighbor.nId = sourceId;
    newNeighbor.nX = sensorBroadcastData.x;
    newNeighbor.nY = sensorBroadcastData.y;
    newNeighbor.nEL = sensorBroadcastData.EL;
    // if neighbor is far away, consider it for removal
    if (calculateDistance(xCoor, yCoor, newNeighbor.nX, newNeighbor.nY) > communicationRadius) {
        return;
    }

    tableI.push_back(newNeighbor);
}

double GSTEBRouting::calculateDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void GSTEBRouting::sendNeighborsTable()
{
	// Each node sends a packet which contains all its
	// neighbors’ information during its own time slot
	// when Step 2 is over.
	GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
    netPacket->setPacketType(NEIGHBOR_BROADCAST_PACKET);

    netPacket->setNnNumber(tableI.size());
	for (size_t i = 0; i < tableI.size(); ++i) {
		netPacket->setNnXCoor(i, tableI[i].nX);
		netPacket->setNnYCoor(i, tableI[i].nY);
		netPacket->setNnEL(i, tableI[i].nEL);
	}
    netPacket->setSource(SELF_NETWORK_ADDRESS);
    netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
	netPacket->setTtl(1);
    toMacLayer(netPacket, BROADCAST_NETWORK_ADDRESS);
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

		tableII.push_back(nnInfo);
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
    netPacket->setDestination(dest_addr.str());
	netPacket->setTtl(numNodes);
    toMacLayer(netPacket, chId);
}

void GSTEBRouting::handleInfoFromNode(GSTEBRoutingPacket* pkt)
{
	// BS receives the packets from all the nodes
	// and records all the information in memory.
	int senderId = atoi(pkt->getSource());
	int numberOfNeighbors = pkt->getNNumber();
	
	for (int i = 0; i < numberOfNeighbors; ++i) {
		GSTEBNeighbors newNode;
		newNode.nId = pkt->getNId(i);
		newNode.nX = pkt->getNXCoor(i);
		newNode.nY = pkt->getNYCoor(i);
		newNode.nEL = pkt->getNEL(i);
		networkTableI.push_back(make_pair(senderId, newNode));
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

	std::map<int, NodeInfo> nodeInfoMap;
    std::map<int, std::set<int>> adjacency; 
    
    for (auto &entry : networkTableI) {
        int sender = entry.first;
        GSTEBNeighbors nb = entry.second;
        if (nodeInfoMap.find(nb.nId) == nodeInfoMap.end()) {
            NodeInfo ni;
            ni.id = nb.nId;
            ni.x = nb.nX;
            ni.y = nb.nY;
            ni.EL = nb.nEL;
            ni.dataSize = 2000; //bits
            nodeInfoMap[nb.nId] = ni;
        }
        adjacency[sender].insert(nb.nId);
        adjacency[nb.nId].insert(sender);
    }
    
    double bsX = xCoor; 
    double bsY = yCoor;
    int bsId = self;  

    for (auto &kv : adjacency) {
        int id = kv.first;
        if (nodeInfoMap.find(id) == nodeInfoMap.end()) {
            ni.id = id;
            ni.x = 0.0;
            ni.y = 0.0;
            ni.EL = 0.0;
            ni.dataSize = 2000;
            nodeInfoMap[id] = ni;
        }
        for (int nb : kv.second) nodeInfoMap[id].neighbors.insert(nb);
    }

    if (nodeInfoMap.empty()) {
        return;
    }

    // --- 1) Data structures for tree building ---
    std::map<int,int> parentMap;          // node -> parent
    std::map<int, std::vector<int>> childrenMap;
    std::set<int> unassigned;             // nodes not yet in tree
    
    for (auto &kv : nodeInfoMap) {
        int id = kv.first;
        unassigned.insert(id);
    }
    // Remove BS from unassigned if it exists as a node
    if (unassigned.count(bsId)) unassigned.erase(bsId);
    parentMap[bsId] = bsId; // root points to itself

    // --- 2) Top-down BFS-like construction ---
    std::queue<int> q;
    q.push(bsId);

    // convenience lambda: compute cost for i via candidate r
    auto cost_via = [&](int i, int r) -> double {
        NodeInfo &ni = nodeInfoMap[i];
        NodeInfo &nr = nodeInfoMap[r];
        // For simplicity we use direct r->BS cost (one hop).
        double dist_i_r = euclidDist(ni.x, ni.y, nr.x, nr.y);
        double dist_r_bs = euclidDist(nr.x, nr.y, bsX, bsY);
        double cost1 = calcTxEnergy(ni.dataSize, dist_i_r); // i -> r
        // r must forward i's data plus its own data (non-fuse)
        double totalBitsFromR = ni.dataSize + nr.dataSize;
        double cost2 = calcTxEnergy((int)totalBitsFromR, dist_r_bs); // r -> BS (approx)
        return cost1 + cost2;
    };

    while (!q.empty()) {
        int parent = q.front(); q.pop();

        // candidate nodes for this parent:
        //  - if parent == bsId, consider all unassigned nodes
        //  - else consider neighbors of parent that are still unassigned
        std::vector<int> candidates;
        if (parent == bsId) {
            for (int nid : unassigned) candidates.push_back(nid);
        } else {
            for (int nid : nodeInfoMap[parent].neighbors) {
                if (unassigned.count(nid)) candidates.push_back(nid);
            }
        }

        // For each candidate node i, test whether this parent is the preferred relay
        // according to the GSTEB rules:
        //  1) dist(parent,BS) < dist(i,BS)
        //  2) parent must be among i's relayCandidates (max EL among neighbors∪self)
        //  3) parent must yield minimal consumption among i's relayCandidates

        std::vector<int> nodesAssignedThisParent;

        for (int i : candidates) {
            NodeInfo &ni = nodeInfoMap[i];
            double distParentToBS = (parent == bsId) ? 0.0 : euclidDist(nodeInfoMap[parent].x, nodeInfoMap[parent].y, bsX, bsY);
            double distIToBS = euclidDist(ni.x, ni.y, bsX, bsY);
            if (!(distParentToBS < distIToBS)) {
                // criterion 1 fail
                continue;
            }

            // find EL_max among i ∪ neighbors(i)
            double elMax = ni.EL;
            for (int nb : ni.neighbors) {
                elMax = std::max(elMax, nodeInfoMap[nb].EL);
            }

            // build relayCandidates: those with EL == elMax among (i ∪ neighbors)
            std::vector<int> relayCandidates;
            if (ni.EL >= elMax - 1e-12) relayCandidates.push_back(i);
            for (int nb : ni.neighbors) {
                if (fabs(nodeInfoMap[nb].EL - elMax) < 1e-12) relayCandidates.push_back(nb);
            }

            // if parent is not one of relay candidates, skip
            bool parentIsRelayCandidate = false;
            for (int r : relayCandidates) if (r == parent) parentIsRelayCandidate = true;
            if (!parentIsRelayCandidate) continue;

            // compute minimal cost among relayCandidates
            double bestCost = std::numeric_limits<double>::infinity();
            int bestR = -1;
            for (int r : relayCandidates) {
                // ensure r exists in nodeInfoMap
                if (nodeInfoMap.find(r) == nodeInfoMap.end()) continue;
                double c = cost_via(i, r);
                if (c < bestCost) { bestCost = c; bestR = r; }
            }

            // If this parent is the best relay for i, assign
            if (bestR == parent) {
                parentMap[i] = parent;
                nodesAssignedThisParent.push_back(i);
            }
        } // end for candidates

        // Mark assigned nodes and push them to queue
        for (int n : nodesAssignedThisParent) {
            unassigned.erase(n);
            childrenMap[parent].push_back(n);
            q.push(n);
            trace() << "assigned parent of node " << n << " = " << parent;
        }
    } // end while q

    // Any remaining unassigned nodes -> attach directly to BS (fallback)
    for (int nid : std::vector<int>(unassigned.begin(), unassigned.end())) {
        parentMap[nid] = bsId;
        childrenMap[bsId].push_back(nid);
        unassigned.erase(nid);
        trace() << "fallback: attach node " << nid << " directly to BS";
    }

    // Store parentMap/childrenMap in class members for later use (scheduling, etc.)
    this->parentTable.clear();
    this->childrenTable.clear();
    for (auto &kv : parentMap) {
        this->parentTable[kv.first] = kv.second;
    }
    for (auto &kv : childrenMap) {
        this->childrenTable[kv.first] = kv.second;
    }

    // --- 3) Create simple TDMA schedule: BFS order -> slot numbers ---
    std::map<int,int> slotMap;
    int slotCounter = 1;
    // BFS from BS over parentMap to assign increasing slots.
    std::queue<int> q2;
    q2.push(bsId);
    std::set<int> visited;
    visited.insert(bsId);
    while (!q2.empty()) {
        int p = q2.front(); q2.pop();
        for (int c : childrenMap[p]) {
            if (!visited.count(c)) {
                slotMap[c] = slotCounter++;
                visited.insert(c);
                q2.push(c);
            }
        }
    }
    // store schedule in class member
    this->tdmaSchedule.clear();
    for (auto &kv : slotMap) this->tdmaSchedule[kv.first] = kv.second;

    // Debug print final tree
    trace() << "Final routing tree (parent -> [children]):";
    for (auto &kv : childrenMap) {
        int p = kv.first;
        std::ostringstream oss;
        oss << p << " -> [";
        for (size_t i=0;i<kv.second.size();++i) {
            if (i) oss << ",";
            oss << kv.second[i];
        }
        oss << "]";
        trace() << oss.str();
    }
    trace() << "TDMA slots assigned to nodes (node:slot):";
    for (auto &kv : this->tdmaSchedule) trace() << kv.first << ":" << kv.second;
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
	toMacLayer(netPacket, BROADCAST_NETWORK_ADDRESS);
}

void GSTEBRouting::handleRoutingTree(GSTEBRoutingPacket* pkt)
{
	// each node receives the routing tree
	// and records its parent node and child nodes
	// in memory.
	setTimer(DATA_COLLECTING_PHASE, 100);
}