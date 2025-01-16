#include "node/communication/routing/WSNRouting/WSNRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

Define_Module(WSNRouting);

void WSNRouting::startup(){
	if (appModule->hasPar("isSink"))
		isSink = appModule->par("isSink");
	else
		throw cRuntimeError("\nMultiPathRings routing has to be used with an application that defines the parameter isSink");
	if (isSink){
		// broadcast
		WSNRoutingPacket *setupPkt = new WSNRoutingPacket("BCAST pkt", NETWORK_LAYER_PACKET);
		
		std::string clusters["12", "56", "71", "90"];
		setupPkt->setClusterAdd(clusters);
		setupPkt->setId(0);
		setupPkt->setWSNRoutingPacketKind(BCAST);
		setupPkt->setSource(SELF_NETWORK_ADDRESS);
		setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
		toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
	}

}

void WSNRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void WSNRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	// Cast the packet
	FloodRoutingPacket *netPacket = dynamic_cast <FloodRoutingPacket*>(pkt);
	if (!netPacket) {
		std::fprintf(log, "Unrecognized packet, discarding\n\n");
		return;
	}

	switch (netPacket->getType()) {

	case PacketType::BCAST:
		// If broadcast packet, check message id
		int msgID = netPacket->getMessageID();
		if (count(pktHistory.first(), pktHistory.last(), msgID)>0){
			break;
		} else {
			//save data
			FloodRoutingPacket* p = netPacket->dup();
			pktHistory.push_back(msgID);
			cluster = p->getClusterAdd();

			toMacLayer(p, BROADCAST_MAC_ADDRESS);
		}
		break;
	}
}
