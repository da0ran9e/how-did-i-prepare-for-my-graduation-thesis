cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum WSNRoutingPacket_Type
{
	BCAST  = 1;			
	ACK = 2;
};

packet WSNRoutingPacket extends RoutingPacket {
	int WSNRoutingMessage enum (WSNRoutingPacket_Type); 
	int messageID;
	string clusterAdd[10];
	string source;
	string destination;
}

