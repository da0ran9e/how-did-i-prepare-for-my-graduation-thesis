#include "node/communication/routing/WSNRouting/WSNRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

Define_Module(WSNRouting);

void WSNRouting::startup(){
    trace() << "Startup " << SELF_NETWORK_ADDRESS << " node ";
        cModule *appModule = getParentModule()->getParentModule()->getSubmodule("Application");
            if (appModule->hasPar("isSink"))
                isSink = appModule->par("isSink");

            else
                throw cRuntimeError("\nMultiPathRings routing has to be used with an application that defines the parameter isSink");
            if (isSink){
                // broadcast
                WSNRoutingPacket *setupPkt = new WSNRoutingPacket("BCAST pkt", NETWORK_LAYER_PACKET);
                trace() << "This node is sink: " << SELF_NETWORK_ADDRESS;
                char* clusters[10] = {"12", "56", "71", "90"};
                for (int i=0; i<4; i++){
                    setupPkt->setClusterAdd(i, clusters[i]);
                }
                int msgID = 101;
                lastPkt = msgID;
                setupPkt->setMessageID(msgID);
                setupPkt->setWSNRoutingMessage(BCAST);
                setupPkt->setSource(SELF_NETWORK_ADDRESS);
                setupPkt->setOrigin(SELF_NETWORK_ADDRESS);
                setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
                toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
            }
}

void WSNRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void WSNRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    //trace() << "WSN pkt Reached";
	// Cast the packet
    WSNRoutingPacket *netPacket = dynamic_cast <WSNRoutingPacket*>(pkt);
	if (!netPacket) {
		return;
	}
	int pkt_kind = netPacket->getWSNRoutingMessage();
	const char * org = netPacket->getOrigin();
	const char * from = netPacket->getSource();
	trace() << "Msg type " << pkt_kind << " org: " << org << " from: " << from << " to: " << SELF_NETWORK_ADDRESS;
	switch (pkt_kind) {

	case WSNRoutingPacket_Type::BCAST:
	{
		// If broadcast packet, check message id
		int msgID = netPacket->getMessageID();
		if (lastPkt == msgID){
			break;
		} else {
			//save data
			WSNRoutingPacket* p = netPacket->dup();
			lastPkt = msgID;
			origin = p->getOrigin();
			for (int i=0; i<4; i++){
			    cluster[i] = p->getClusterAdd(i);
			}

			// Forward the message to neighbors 
			p->setSource(SELF_NETWORK_ADDRESS);
            p->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(p, BROADCAST_MAC_ADDRESS);

			for (int i=0; i<4; i++){
				if (!strcmp(cluster[i].c_str(), SELF_NETWORK_ADDRESS)){
					//Sent recruit
					WSNRoutingPacket *setupPkt = new WSNRoutingPacket("RECRUIT pkt", NETWORK_LAYER_PACKET);
            		trace() << "Node " << SELF_NETWORK_ADDRESS << " is CH ";
					ch = SELF_NETWORK_ADDRESS;
					setupPkt->setWSNRoutingMessage(RECRUIT);
					setupPkt->setSource(SELF_NETWORK_ADDRESS);
					setupPkt->setOrigin(SELF_NETWORK_ADDRESS);
					setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
					toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
				}
			}
		}
		break;
	}

	case WSNRoutingPacket_Type::RECRUIT:
	{
		std::string recruitId = netPacket->getOrigin();
		std::string senderId = netPacket->getSource();
		WSNRoutingPacket* p = netPacket->dup();
		if (ch != "-1"){
			break;
		} else {
			//Save CH
			trace() << "Node " << SELF_NETWORK_ADDRESS << " joint cluster: " << recruitId;
			ch = recruitId;
			bHop = senderId;

			// send a join message back
			WSNRoutingPacket *setupPkt = new WSNRoutingPacket("JOIN pkt", NETWORK_LAYER_PACKET);
					setupPkt->setWSNRoutingMessage(JOIN);
					setupPkt->setSource(SELF_NETWORK_ADDRESS);
					setupPkt->setOrigin(SELF_NETWORK_ADDRESS);
					setupPkt->setDestination(ch.c_str());
					toMacLayer(setupPkt, atoi(bHop.c_str()));
			//Forward
			p->setSource(SELF_NETWORK_ADDRESS);
            p->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(p, BROADCAST_MAC_ADDRESS);
		}
		break;
	}

	case WSNRoutingPacket_Type::JOIN:
	{
		if(ch == SELF_NETWORK_ADDRESS){
			//save data
			clusterUnits.push_back(netPacket->getOrigin());
		} else {
			//forward to bHop
		    WSNRoutingPacket* p = netPacket->dup();
			p->setSource(SELF_NETWORK_ADDRESS);
            p->setDestination(bHop.c_str());
			toMacLayer(netPacket, atoi(bHop.c_str()));
		}
		
		break;
	}
	}
}
