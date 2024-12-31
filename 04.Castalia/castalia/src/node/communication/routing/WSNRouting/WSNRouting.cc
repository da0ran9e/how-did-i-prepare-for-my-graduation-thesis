#include "node/communication/routing/WSNRouting/WSNRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

Define_Module(WSNRouting);

void WSNRouting::startup(){
	
}

/* Application layer sends a packet together with a dest network layer address.
 * Network layer is responsible to route that packet by selecting an appropriate
 * MAC address. With WSNRouting we do not perform any routing function. We
 * just encapsulate the app packet and translate the net address to a MAC address
 * (e.g. "3" becomes 3, or a BROADCAST_NETWORK_ADDRESS becomes BROADCAST_MAC_ADDRESS)
 * If the destination is a 1-hop neighbor it will receive the packet.
 */
void WSNRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	if (netPacket->getOriginAdd() == -1) {
		netPacket->setOriginAdd(SELF_NETWORK_ADDRESS);
		netPacket->setMessageID(SELF_NETWORK_ADDRESS);
		netPacket->setWSNRoutingMessage(BC_SINK);
	}
	WSNRoutingPacket *netPacket = new WSNRoutingPacket("WSNRouting packet", NETWORK_LAYER_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination(destination);
	encapsulatePacket(netPacket, pkt);
	toMacLayer(netPacket, resolveNetworkAddress(destination));
}

/* MAC layer sends a packet together with the source MAC address and other info.
 * With BypassMAC we just filter the packet by the NET address. If this
 * node is the right destination decapsulatePacket will extract the APP packet.
 * If not, there is no need to do anything. The whole net packet (including
 * the encapsulated apppacket will be deleted by the virtual routing code
 */
void WSNRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	WSNRoutingPacket *netPacket = dynamic_cast <WSNRoutingPacket*>(pkt); 	
	processIncoming(netPacket, srcMacAddress);
}

void WSNRouting::acceptHop(WSNRoutingPacket * netPacket){
	//update variable
	prevHop = netPacket->getSenderAdd();
	netPacket->setHopCount(netPacket->getHopCount()+1);
	level = netPacket->getHopCount();

	//send Ack
	netPacket->setWSNRoutingMessage(ACK_SINK);
	netPacket->setDestination(netPacket->getSource());
	toApplicationLayer(decapsulatePacket(netPacket));
	//Broadcast to others
	netPacket->setWSNRoutingMessage(BC_SINK);
	netPacket->setDestination(BROADCAST_NETWORK_ADDRESS);
	toApplicationLayer(decapsulatePacket(netPacket));
}

void WSNRouting::onBC(WSNRoutingPacket * netPacket){
	if (level != -1){
		if (netPacket->getHopCount() < level) {
			acceptHop(netPacket);
		}
	} else {
		acceptHop(netPacket);
	}
}

void WSNRouting::onAck(WSNRoutingPacket * netPacket){
	nextHop = netPacket->getSenderAdd();
}

void WSNRouting::processIncoming(WSNRoutingPacket * netPacket){
	if (netPacket) {
		sinkAdd = netPacket->getOriginAdd();
		if (netPacket->getWSNRoutingMessage() == BC_SINK) {
			onBC(netPacket);
		}
		if (netPacket->getWSNRoutingMessage() == ACK_SINK){
			onAck(netPacket);
		}
	} 
}