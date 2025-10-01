cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum GSTEBRoutingPacketType {
    BS_BROADCAST_PACKET = 0;   
    SENSOR_BROADCAST_PACKET = 1;
    NEIGHBOR_BROADCAST_PACKET = 2;
};

struct BSBroadcastInfo {
    int x;
    int y;
    double timeStart;
    int numNodes;
    double timeSlot;
};

struct SensorBroadcastInfo {
    int nX;
    int nY;
    double nEL;
};

struct BSBroadcastInfo {
    double timeStart;
    int numNodes;
    double timeSlot;
};

packet GSTEBRoutingPacket extends RoutingPacket {
    int packetType @enum(GSTEBRoutingPacketType);

    int ttl;

    BSBroadcastInfo bSBroadcastData;
    SensorBroadcastInfo sensorBroadcastData;

    int nnNumber;
    int nnId[500];
    int nnXCoor[500];
    int nnYCoor[500];
    int nnEL[500];
}

