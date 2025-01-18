cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum WSNRoutingPacket_Type
{
	BCAST  = 1;			
	ACK = 2;
	RECRUIT = 3;
	JOIN = 4;
};

packet WSNRoutingPacket extends RoutingPacket {
	int WSNRoutingMessage enum (WSNRoutingPacket_Type); 
	int messageID;
	string clusterAdd[10];
	string origin;
	string source;
	string destination;
}

