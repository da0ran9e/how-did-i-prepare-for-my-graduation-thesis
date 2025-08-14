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
    SEND_HELLO_TIMER = 1,
    RECONFIGURATION_TIMER = 2,
    CL_ANNOUNCEMENT_TIMER = 3,
    CL_CONFIRMATION_TIMER = 4,
    CONFIRMATION_SENDER_TIMER = 5,
    CL_VOTE_CH = 6,
    LINK_REQUEST_TIMEOUT = 7,
    LINK_ESTABLISHED_CONFIRMATION = 8,
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
    double ngw_to_ch_distance;  // NGW -> CH
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


class CellularRouting : public VirtualRouting {
 private:
    double helloInterval;
    double cellRadius;
    int grid_offset;

    int myCellId;
    int myColor;
    NodeRole myRole;
    double myX, myY;

    int myCL_id;
    int myCH_id;

    vector<NeighborRecord> neighborTable;
    map<int, int> intraCellRoutingTable;
    map<int, int> interCellRoutingTable;
    vector<CellMemberRecord> cellMembers;
    int gatewayTowardsCH = -1;
    int myNextHopId = -1;

    map<int, LinkRequestState> pendingLinkRequests;
    int nextTimerIndex;


 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;

    void PrecalculateSimulationResults();

    Point calculateCellCenter(int cell_id);
    void calculateCellInfo();
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
