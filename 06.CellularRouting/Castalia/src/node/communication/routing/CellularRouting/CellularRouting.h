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
    SEND_HELLO_TIMER = 1,
    RECONFIGURATION_TIMER = 2,
    CL_ANNOUNCEMENT_TIMER = 3,
    CL_CONFIRMATION_TIMER = 4,
    CONFIRMATION_SENDER_TIMER = 5,
    CL_VOTE_CH = 6,
    LINK_REQUEST_TIMEOUT = 7,
    LINK_ESTABLISHED_CONFIRMATION = 8,
    ROUTING_TABLE_UPDATE = 9,
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

    void parseNetworkLayout();
    void calculateCellInfo();
    Point calculateCellCenter(int cell_id);

    void sendHelloPacket();
    void handleHelloPacket(CellularRoutingPacket* pkt);

    void runCLElection();
    void startCLElectionContention();
    void sendCLAnnouncement();
    void handleCLAnnouncementPacket(CellularRoutingPacket* pkt);

    void sendCLConfirmationPacket();
    void handleCLConfirmationPacket(CellularRoutingPacket* pkt);

    void startReconfiguration();
    void voteCH();
    void findAndEstablishInterCellLinks();
    void handleCLCommandPacket(CellularRoutingPacket* pkt);
    void sendLinkRequest();
    void handleLinkRequest(CellularRoutingPacket* pkt);
    void handleLinkAck(CellularRoutingPacket* pkt);
    void handleLinkEstablishedConfirmation(CellularRoutingPacket* pkt);
    void calculateAndDistributeIntraCellTree();
    double calculateDistance(double x1, double y1, double x2, double y2);
    void announceRoutingTree(int from, int to);
    void handleRoutingTableUpdate(CellularRoutingPacket* pkt);
};

#endif //_CELLULARROUTING_H_
