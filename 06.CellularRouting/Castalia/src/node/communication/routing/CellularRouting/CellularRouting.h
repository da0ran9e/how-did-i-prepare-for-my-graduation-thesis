#ifndef _CELLULARROUTING_H_
#define _CELLULARROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "CellularRoutingPacket_m.h"
#include <vector>
#include <map>

using namespace std;

enum NodeRole {
    NORMAL_NODE = 0,
    CELL_LEADER,       
    CLUSTER_HEAD,      
    CELL_GATEWAY    
};

// timer
enum CellularRoutingTimers {
    SEND_HELLO_TIMER = 1,           
    LINK_REQUEST_TIMEOUT = 2        
    // ...
};

struct NeighborRecord {
    int id;
    int cellId;
    double x;
    double y;
    simtime_t lastHeard;
};

struct LinkRequestState {
    int source_cgw_id;
    int target_ngw_id;
    int target_cell_id;
    int to_final_ch_id;
    cMessage* timeout_timer; 
};


class CellularRouting : public VirtualRouting {
 private:
    double helloInterval;
    double cellRadius;
    bool amI_CH;
    bool amI_CL;


    int myCellId;
    int myColor;
    NodeRole myRole;
    double myX, myY;

    int myCL_id;
    int myCH_id; 

    vector<NeighborRecord> neighborTable;
    map<int, int> intraCellRoutingTable;      
    map<int, int> interCellRoutingTable;      


    map<int, LinkRequestState> pendingLinkRequests; 
    int nextTimerIndex;


 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;

    void calculateCellInfo();
    void sendHelloPacket();
    void handleHelloPacket(CellularRoutingPacket* pkt);
    void runCLElection();

    void startReconfiguration();
    void findAndEstablishInterCellLinks();
    void handleLinkRequest(CellularRoutingPacket* pkt);
    void handleLinkAck(CellularRoutingPacket* pkt);
    void handleLinkEstablishedConfirmation(CellularRoutingPacket* pkt);
    void calculateAndDistributeIntraCellTree();
    void handleRoutingTableUpdate(CellularRoutingPacket* pkt);
};

#endif //_CELLULARROUTING_H_
