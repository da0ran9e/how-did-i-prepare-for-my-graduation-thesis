cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum WSNRoutingPacket_Type
{
	BC_TOPO  = 1;			
	ACK_TOPO = 2;
};

packet WSNRoutingPacket extends RoutingPacket {
	int WSNRoutingMessage enum (WSNRoutingPacket_Type); 
	int messageAdd;
	int clusterHeadAdd;
	int senderAdd;
	int originAdd;
	int hopCount = 0;
}

