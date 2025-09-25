#include "node/communication/routing/gstebRouting/GSTEBRouting.h"

Define_Module(GSTEBRouting);


void GSTEBRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination(destination);
	encapsulatePacket(netPacket, pkt);
	toMacLayer(netPacket, resolveNetworkAddress(destination));
}

void GSTEBRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	RoutingPacket *netPacket = dynamic_cast <RoutingPacket*>(pkt);
	if (netPacket) {
		string destination(netPacket->getDestination());
		if (destination.compare(SELF_NETWORK_ADDRESS) == 0 ||
		    destination.compare(BROADCAST_NETWORK_ADDRESS) == 0)
			toApplicationLayer(decapsulatePacket(pkt));
	}
}

void GSTEBRouting::startup()
{
	nodeId = par("nodeId");
	xCoor = par("xCoor");
	yCoor = par("yCoor");
	energy = par("energy");

	// 
}

void GSTEBRouting::timerFiredCallback(int index)
{
	// ch chọn node ngẫu nhiên làm root,  thông báo. ưu tiên node có EL cao

	// mỗi node (i) tự chọn cha ưu tiên node có El cao (r)

	// 

}

void GSTEBRouting::beacon()
{
	// 
}

void 