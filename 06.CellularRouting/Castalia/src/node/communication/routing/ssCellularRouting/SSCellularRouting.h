#ifndef _SSCELLULARROUTING_H_
#define _SSCELLULARROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/ssCellularRouting/SSCellularRouting_m.h"

using namespace std;

struct Point {
    double x;
    double y;
};

enum SSCellularRoutingTimers {
    PRECALCULATE_TIMERS = 0,
    CH_ADVERTISMENT_TIMER,
    SEND_ANNOUNCEMENT_QUEUE,
    SENSING_STATE,
    SEND_CELL_PACKET,
	RECONFIGURATION_TIMER,
    CLEANUP,
};

struct SSCRCellData {
    int cellId;
    int color;
    int clId;  
    int chId;  
    vector<int> members;
    vector<int> neighbors;
    map<int, int> gateways;
};

struct SSCRNodeData {
    int id;
    double x;
    double y;
	bool isCH;
	bool isCL;
    int cellId;
    int color;
    int clId;
    int chId;
    vector<int> neighbors;
};
static bool g_isPrecalculated = false;
static vector<SSCRNodeData> g_ssNodesDataList;
static vector<SSCRCellData> g_ssCellDataList;
static map <int, map<int, int>> g_ssRoutingTable; // <node, <next-cell, next-hop>>


class SSCellularRouting: public VirtualRouting {
private:
    int traceMode = 0;
	int grid_offset = 100;
	double cellRadius = 80.0;
	int sensingDuration = 100;
	int reconfigurationTime = 100;
	double colorTimeSlot = 100.0;
	int sensorDataDub = 100;
	int numberOfNodes;
    int maxHopCount;

	bool isCH;
	bool isCL;
	int myCellId = -1;
	int myColor = -1;
	double myX = 0.0, myY = 0.0;
	int myCLId = -1;
	int myCHId = -1;

	map <int, map<int, int>> intraCellRoutingTable; // <node, <next-cell, next-hop>>

	//vector<CellMemberRecord> cellMembers;
	int neighborCells[7] = {-1, -1, -1, -1, -1, -1};
	int cellGateways[6] = {-1, -1, -1, -1, -1, -1};
	int neighborCellGateways[6] = {-1, -1, -1, -1, -1, -1};

	struct ComparePacketsPriority {
		bool operator()(const pair<SSCellularRoutingPacket*, int>& a, const pair<SSCellularRoutingPacket*, int>& b) {
			return a.first->getSensorData().dataId < b.first->getSensorData().dataId;
		}
	};
	priority_queue<pair<SSCellularRoutingPacket*, int>, vector<pair<SSCellularRoutingPacket*, int>>, ComparePacketsPriority> cellPacketQueue;
	
	int myCellPathToCH[100] = {-1};

	int levelInCell = -1;

protected:
	void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;
    double calculateDistance(double x1, double y1, double x2, double y2);

    void PrecalculateSimulationResults();
	SSCRNodeData getNodeData(int nodeId);
    SSCRCellData getCellData(int cellId);
    bool isNodeInList(int nodeId, const vector<int>& nodeList);

    Point calculateCellCenter(int cell_id);
    void calculateCellInfo();
    void forwardPacket(SSCellularRoutingPacket* pkt, int nextHopId);
    void sendHelloPacket();
    void handleHelloPacket(SSCellularRoutingPacket* pkt);
    void calculateFitnessScore();
    void sendCLAnnouncement();
    void handleCLAnnouncementPacket(SSCellularRoutingPacket* pkt);
    void sendCLConfirmationPacket();
    void handleCLConfirmationPacket(SSCellularRoutingPacket* pkt);
    void gatewaySelection();
    void sendGatewaySelectionPacket();
    void handleGatewaySelectionPacket(SSCellularRoutingPacket* pkt);
    void sendLinkRequestPacket();
    void handleLinkRequestPacket(SSCellularRoutingPacket* pkt);
    void sendLinkConfirmationPacket(SSCellularRoutingPacket* pkt);
    void handleLinkConfirmationPacket(SSCellularRoutingPacket* pkt);
    void calculateRoutingTree();
    void announceRoutingTable();
    void sendRoutingTableAnnouncementPacket();
    void handleRoutingTableAnnouncementPacket(SSCellularRoutingPacket* pkt);
    void finalizeRouting();
    void sendCHAnnouncement();
    void sendAnnouncementQueue();
    void sendCellPacket();
    void handleCHAnnouncementPacket(SSCellularRoutingPacket* pkt);
    void selectClusterHead();
    void sendCellHopAnnouncementPacket();
    void handleCellHopAnnouncementPacket(SSCellularRoutingPacket* pkt);
    void sendSensorDataPacket();
    void handleSensorDataPacket(SSCellularRoutingPacket* pkt);
    double calculateConsumption(int desNode);
};

#endif		
