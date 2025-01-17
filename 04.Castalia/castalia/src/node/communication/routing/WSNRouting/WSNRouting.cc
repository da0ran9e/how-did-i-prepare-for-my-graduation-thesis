#include "node/communication/routing/WSNRouting/WSNRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

Define_Module(WSNRouting);

void WSNRouting::startup(){


}

void WSNRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
    trace() << "Startup " << SELF_NETWORK_ADDRESS << " node ";
        cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
        if (appModule->hasPar("isSink"))
            isSink = appModule->par("isSink");

        else
            throw cRuntimeError("\nMultiPathRings routing has to be used with an application that defines the parameter isSink");
        if (isSink){
            // broadcast
            WSNRoutingPacket *setupPkt = new WSNRoutingPacket("BCAST pkt", NETWORK_LAYER_PACKET);
            trace() << "Node " << SELF_NETWORK_ADDRESS << " is sink ";
            char* clusters[10] = {"12", "56", "71", "90"};
            for (int i=0; i<4; i++){
                setupPkt->setClusterAdd(i, clusters[i]);
            }
            setupPkt->setMessageID(101);
            setupPkt->setWSNRoutingMessage(BCAST);
            setupPkt->setSource(SELF_NETWORK_ADDRESS);
            setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
            toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
        }
}

void WSNRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    trace() << "WSN pkt Reached";
	// Cast the packet
    WSNRoutingPacket *netPacket = dynamic_cast <WSNRoutingPacket*>(pkt);
	if (!netPacket) {
		return;
	}
	int pkt_kind = netPacket->getWSNRoutingMessage();
	trace() << "WSN Received pkt kind: " << pkt_kind;
	switch (pkt_kind) {

	case WSNRoutingPacket_Type::BCAST:
		// If broadcast packet, check message id
		int msgID = netPacket->getMessageID();
		if (lastPkt == msgID){
			break;
		} else {
			//save data
			WSNRoutingPacket* p = netPacket->dup();
			lastPkt = msgID;
			for (int i=1; i<4; i++){
			    cluster[i] = p->getClusterAdd(i);
			}

			trace() << "WSN Received " << msgID << " pkt ";

			toMacLayer(p, BROADCAST_MAC_ADDRESS);
		}
		break;
	}
}
