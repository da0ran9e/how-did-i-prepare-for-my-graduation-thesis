cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum GSTEBRoutingPacketType {
    BS_BROADCAST_PACKET = 0;   
};

struct BSBroadcastInfo {
    double timeStart;
    int numNodes;
    double timeSlot;
};

packet GSTEBRoutingPacket extends RoutingPacket {
    int packetType @enum(GSTEBRoutingPacketType);

    BSBroadcastInfo bSBroadcastData;
}

