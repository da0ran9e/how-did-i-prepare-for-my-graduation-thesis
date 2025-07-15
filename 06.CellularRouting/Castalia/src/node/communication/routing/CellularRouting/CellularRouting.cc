#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

void CellularRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{
	CellularRoutingPacket *netPacket = new CellularRoutingPacket("CellularRouting packet", NETWORK_LAYER_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination(destination);
	encapsulatePacket(netPacket, pkt);
	toMacLayer(netPacket, resolveNetworkAddress(destination));
}

void CellularRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	RoutingPacket *netPacket = dynamic_cast <RoutingPacket*>(pkt);
	if (netPacket) {
		string destination(netPacket->getDestination());
		if (destination.compare(SELF_NETWORK_ADDRESS) == 0 ||
		    destination.compare(BROADCAST_NETWORK_ADDRESS) == 0)
			toApplicationLayer(decapsulatePacket(pkt));
	}
}

