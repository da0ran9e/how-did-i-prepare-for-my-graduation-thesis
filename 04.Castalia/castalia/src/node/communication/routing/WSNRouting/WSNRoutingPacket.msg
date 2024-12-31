cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum WSNRoutingPacket_Type
{
	BC_SINK  = 1;			
	ACK_SINK = 2;
};

packet WSNRoutingPacket extends RoutingPacket {
	int WSNRoutingMessage enum (WSNRoutingPacket_Type); 
	char * messageID;
	char * clusterHeadAdd;
	char * originAdd = -1;
	int hopCount = 0;
}

