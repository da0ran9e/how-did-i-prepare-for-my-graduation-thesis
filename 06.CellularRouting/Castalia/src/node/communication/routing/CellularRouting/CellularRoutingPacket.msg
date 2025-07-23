cplusplus {{
#include "node/communication/routing/RoutingPacket_m.h"
}}

class RoutingPacket;

enum CellularRoutingPacketType {
    HELLO_PACKET = 0;         
    CL_ANNOUNCEMENT = 1;    
    CL_CONFIRMATION = 2;
    BS_UPDATE_PACKET = 3;       
    LINK_REQUEST = 4;        
    LINK_ACK = 5;             
    LINK_ESTABLISHED = 6;      

    INTRA_CELL_ROUTING_UPDATE = 7;
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

struct NeighborInfo {
    int nodeId;
    double x;
    double y;
    int cellId;
}

struct NodeInfo {
    int nodeId;
    double x;
    double y;
    double energyLevel;
    NeighborInfo neighbors[20];
};

struct LinkRequestInfo {
    int sourceCellId;
    int targetCH_id; 
};


packet CellularRoutingPacket extends RoutingPacket {
    int packetType @enum(CellularRoutingPacketType);

    HelloInfo helloData;
    CLAnnouncementInfo clAnnouncementData;
    NodeInfo nodeInfoData;
    
    LinkRequestInfo linkRequestData;
    int nextHopId; 
};