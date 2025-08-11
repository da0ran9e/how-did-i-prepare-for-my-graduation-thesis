#ifndef _CELLULARROUTING_H_
#define _CELLULARROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "CellularRoutingPacket_m.h"
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    double x;
    double y;
};

enum NodeRole {
    NORMAL_NODE = 0,
    CELL_LEADER,
    CLUSTER_HEAD,
    CELL_GATEWAY
};

// timer
enum CellularRoutingTimers {
    STAGE_0 = 0,
    STAGE_1 = 1,
    SEND_HELLO_TIMER = 2,
};

struct NeighborRecord {
    int id;
    int cellId;
    double x;
    double y;
};

struct LinkRequestState {
    int source_cgw_id;
    int target_ngw_id;
    int target_cell_id;
    int to_final_ch_id;
};

struct CellMemberRecord {
    int id;
    double x;
    double y;
    double energy;
    vector<NeighborRecord> neighbors;
};

struct GatewayCandidate {
    int cgw_id;
    int ngw_id;
    int target_cell_id;
    double link_distance;       // CGW <-> NGW
    double ngw_to_ch_distance;  // NGW -> CH
};
vector<GatewayCandidate> candidates;


class CellularRouting : public VirtualRouting {
 private:
    double helloInterval;
    double cellRadius;
    double communicationRange;
    double energyThreshold;

    int myCL_id;
    int myCH_id;
    NodeRole myRole;
    double myX, myY;
    int myCellId;
    int myColor;

    double fitnessScore = -1.0; // for CL election

    vector<NeighborRecord> neighborTable;
    vector<CellMemberRecord> cellMembers;
    int gatewayTowardsCH = -1; // for CL only
    int myNextHopId = -1;

    map<int, LinkRequestState> pendingLinkRequests;
    int nextTimerIndex;
    
    map<int, int> intraCellRoutingTable; 
    map<int, int> interCellRoutingTable;

 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;

};

#endif //_CELLULARROUTING_H_
