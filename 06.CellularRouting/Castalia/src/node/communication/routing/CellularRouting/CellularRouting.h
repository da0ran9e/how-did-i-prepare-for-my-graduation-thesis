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
    PRECALCULATE_TIMERS = 0,
    STATE_0,
    SEND_HELLO_TIMER,
    HELLO_TIMEOUT,
    CL_ELECTION_TIMER,
    CL_CONFIRMATION_TIMER,
    GATEWAY_SELECTION_TIMER,
    CL_CALCULATION_TIMER,
    RECONFIGURATION_TIMER,
    CL_ANNOUNCEMENT_TIMER,
    ROUTING_TABLE_UPDATE_TIMER,
    CONFIRMATION_SENDER_TIMER,
    CL_VOTE_CH,
    LINK_REQUEST_TIMEOUT,
    LINK_ESTABLISHED_CONFIRMATION,
};

struct NeighborRecord {
    int id;
    int cellId;
    double x;
    double y;
    simtime_t lastHeard;
};

struct CellNeighborRecord {
    int cellId;
    int color;
    int ngw_id;
};

struct LinkRequestState {
    int source_cgw_id;
    int target_ngw_id;
    int target_cell_id;
    int to_final_ch_id;
    cMessage* timeout_timer;
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
};
vector<GatewayCandidate> candidates;

struct NodeData {
    int id;
    double x;
    double y;
    NodeRole role;
    int cellId;
    int color;
    int clId;
    int chId;
    int nextHopId;
    vector<NeighborRecord> neighbors;
};

struct CellData {
    int cellId;
    int color;
    int clId;  // Cell Leader ID
    int chId;  // Cluster Head ID
    vector<CellMemberRecord> members;
    vector<CellNeighborRecord> neighbors;
    map<int, int> gateways;
    map<int, int> intraCellRoutingTable;
};

struct NodeRoutingUpdateInfo {
    RoutingUpdateInfo routingInfo[7];
};

class CellularRouting : public VirtualRouting {
 private:
    double helloInterval;
    double clElectionDelayInterval;
    double cellRadius;
    int grid_offset;

    int myCellId;
    int myColor;
    NodeRole myRole;
    double myX, myY;

    int myCL_id;
    int myCH_id;

    double fitnessScore = -1;
    double clFitnessScore = -1;

    vector<NeighborRecord> neighborTable;
    map <int, map<int, int>> intraCellRoutingTable; // <node, <next-cell, next-hop>>
    map<int, int> interCellRoutingTable;
    vector<CellMemberRecord> cellMembers;
    int gatewayTowardsCH = -1;
    int myNextHopId = -1;

    int neighborCells[6] = {-1, -1, -1, -1, -1, -1}; 
    int cellGateways[6] = {-1, -1, -1, -1, -1, -1};
    int neighborCellGateways[6] = {-1, -1, -1, -1, -1, -1};

    vector<NodeRoutingUpdateInfo> routingUpdates;

    map<int, LinkRequestState> pendingLinkRequests;
    int nextTimerIndex;


 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;
    double calculateDistance(double x1, double y1, double x2, double y2);

    void PrecalculateSimulationResults();

    Point calculateCellCenter(int cell_id);
    void calculateCellInfo();
    void forwardPacket(CellularRoutingPacket* pkt, int nextHopId);
    void sendHelloPacket();
    void handleHelloPacket(CellularRoutingPacket* pkt);
    void calculateFitnessScore();
    void sendCLAnnouncement();
    void handleCLAnnouncementPacket(CellularRoutingPacket* pkt);
    void sendCLConfirmationPacket();
    void handleCLConfirmationPacket(CellularRoutingPacket* pkt);
    void gatewaySelection();
    void sendGatewaySelectionPacket();
    void handleGatewaySelectionPacket(CellularRoutingPacket* pkt);
    void sendLinkRequestPacket();
    void handleLinkRequestPacket(CellularRoutingPacket* pkt);
    void sendLinkConfirmationPacket(CellularRoutingPacket* pkt);
    void handleLinkConfirmationPacket(CellularRoutingPacket* pkt);
    void calculateRoutingTree();
    void announceRoutingTable();
    void sendRoutingTableAnnouncementPacket();
    void handleRoutingTableAnnouncementPacket(CellularRoutingPacket* pkt);
    void finalizeRouting();
    void sendCHAnnouncement();
    void handleCHAnnouncementPacket(CellularRoutingPacket* pkt);
    void selectClusterHead();
};

#endif //_CELLULARROUTING_H_
