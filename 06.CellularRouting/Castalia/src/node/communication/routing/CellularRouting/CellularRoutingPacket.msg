cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum CellularRoutingPacketType {
    HELLO_PACKET = 0;         
    CL_ANNOUNCEMENT = 1;    

    BS_UPDATE_PACKET = 2;       
    LINK_REQUEST = 3;        
    LINK_ACK = 4;             
    LINK_ESTABLISHED = 5;      

    INTRA_CELL_ROUTING_UPDATE = 6;
};

struct HelloInfo {
    int cellId;
    int color;
    double energyLevel;
    double x;
    double y;
};

struct CLAnnouncementInfo {
    int cellId;
    int color;
    double energyLevel;
    double x;
    double y;
};

struct LinkRequestInfo {
    int sourceCellId;
    int targetCH_id; 
};


packet CellularRoutingPacket extends RoutingPacket {
    int packetType @enum(CellularRoutingPacketType);

    HelloInfo helloData;
    CLAnnouncementInfo clAnnouncementData;
    LinkRequestInfo linkRequestData;
    int nextHopId; 
};