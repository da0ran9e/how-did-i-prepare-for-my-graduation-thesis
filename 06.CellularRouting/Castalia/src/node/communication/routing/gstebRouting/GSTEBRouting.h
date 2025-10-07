#ifndef _GSTEBROUTING_H_
#define _GSTEBROUTING_H_

#include <map>
#include <set>
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>
#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/gstebRouting/GSTEBRouting_m.h"

using namespace std;

enum GSTEBRoutingTimers {
    INITIAL_PHRASE = 0,
    BS_BROADCAST,
    PHRASE_I_TIMESLOT,
    SENSOR_BROADCAST_TIMEOUT,

    TREE_CONSTRUCTION_PHASE,

    DATA_COLLECTING_PHASE,
};

struct GSTEBNeighborsOfNeighbors {
    int neighborId;
    int nnId;
    int nnX;
    int nnY;
    int nnEL;
};

struct GSTEBNeighbors {
    int nodeId; // neighbor of
    int nNumber;
    int nId; // this node ID
    int nX;
    int nY;
    int nEL;
    int consumption;
    double distanceToCH;
};

struct SortByNumber { //number of neighbor
        bool operator() (const GSTEBNeighbors& a, const GSTEBNeighbors& b) const {
            if (a.nNumber != b.nNumber) return a.nNumber < b.nNumber;
            return a.nId < b.nId;
        }
    };

struct SortByDistance { //number of neighbor
        bool operator() (const GSTEBNeighbors& a, const GSTEBNeighbors& b) const {
            if (a.distanceToCH != b.distanceToCH) return a.distanceToCH < b.distanceToCH;
            if (a.nId != b.nId) return a.nId < b.nId;
        }
    };

class GSTEBRouting: public VirtualRouting {
    private:
    int nodeId;
    double xCoor;
    double yCoor;
    double energy;
    bool isCH;
    bool isSink;
    double alpha = 0.1;
    double communicationRadius = 80.0;

    int chId = -1;
    int chX;
    int chY;
    int numNodes;
    int timeStart;
    int timeSlot = 60;

    vector<GSTEBNeighbors> tableI;
    vector<GSTEBNeighborsOfNeighbors> tableII;
    double myEL;
    int phase;

    vector<GSTEBNeighbors> networkTableI;
    map<int, int> networkParentTable;

    vector<GSTEBNeighbors> relayCandidates;
    int parentId = -1;
    vector<int> myChild;

    int phaseITimeslot;
    int sensorBroadcastTimeout;
    int initialPhraseTimeout; //The length of time slots in Steps 2 and 3 is predefined
 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;


    void sendBSBroadcast();
    void handleBSBroadcast(GSTEBRoutingPacket* pkt);
    void computeEL();
    void sendSensorBroadcast();
    void handleSensorBroadcast(GSTEBRoutingPacket* pkt);
    double calculateDistance(int x1, int y1, int x2, int y2);
    void sendNeighborsTable();
    void handleNeighborsTable(GSTEBRoutingPacket* pkt);
    double calcTxEnergy(int kBits, double distance);
    void chooseRelayNode();
    void chooseParentNode();
    void sendInfoToBS();
    void handleInfoFromNode(GSTEBRoutingPacket* pkt);
    void calculateRoutingTree();
    void broadcastRoutingTree();
    void handleRoutingTree(GSTEBRoutingPacket* pkt);
};

#endif              //BYPASSROUTINGMODULE
