cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum GSTEBRoutingPacketType {
    BS_BROADCAST_PACKET = 0;   
    SENSOR_BROADCAST_PACKET = 1;
    NEIGHBOR_BROADCAST_PACKET = 2;
    NODE_INFO_PACKET = 3;
    NODE_CONTROL_PACKET = 4;
};

struct GSTEBBSBroadcastInfo {
    int x;
    int y;
    double timeStart;
    int numNodes;
    int sinkId;
    double timeSlot;
};

struct GSTEBSensorBroadcastInfo {
    int nX;
    int nY;
    double nEL;
};

packet GSTEBRoutingPacket extends RoutingPacket {
    int packetType @enum(GSTEBRoutingPacketType);

    int ttl;

    GSTEBBSBroadcastInfo bSBroadcastData;
    GSTEBSensorBroadcastInfo sensorBroadcastData;

    int nNumber;
    int nId[500];
    int nXCoor[500];
    int nYCoor[500];
    int nEL[500];

    int nnNumber;
    int nNeighbor[500];
    int nnId[500];
    int nnXCoor[500];
    int nnYCoor[500];
    int nnEL[500];
}

