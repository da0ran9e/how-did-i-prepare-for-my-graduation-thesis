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

			setTimer(DATA_COLLECTING_PHASE, 1);
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

void GSTEBRouting::handleSensorBroadcast()
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

void GSTEBRouting::handleNeighborsTable()
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