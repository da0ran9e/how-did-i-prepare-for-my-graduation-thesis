cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

struct SensorData{
    int destinationCH;
    int dataId;
    int sensorId;
    int hopCount;
};

packet LEACHCMFPacket extends RoutingPacket {
    int packetType @enum(LEACHCMFPacketType);
};