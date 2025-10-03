#ifndef _GSTEBROUTING_H_
#define _GSTEBROUTING_H_

#include <map>
#include <set>
#include <queue>
#include <limits>
#include <cmath>
#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/gstebRouting/GSTEBRoutingPacket_m.h"

using namespace std;

enum GSTEBRoutingTimers {
	INITIAL_PHRASE = 0,
	BS_BROADCAST,
	PHRASE_I_TIMESLOT,
	SENSOR_BROADCAST_TIMEOUT,

	TREE_CONSTRUCTION_PHASE,

	DATA_COLLECTING_PHASE,
};

struct NodeInfo {
    int id;
    double x;
    double y;
    double EL;         // estimated energy-level
    int dataSize;      // expected data size in bits (e.g. 2000)
    std::set<int> neighbors; // neighbor IDs
    bool assigned = false;   // whether this node is attached to the tree
};

struct GSTEBNeighborsOfNeighbors {
	int neighborId;
	int nnId;
	int nnX;
	int nnY;
	int nnEL;
};

struct GSTEBNeighbors {
	int nId;
	int nX;
	int nY;
	int nEL;
};

class GSTEBRouting: public VirtualRouting {
	private:
	int nodeId;
	int xCoor;
	int yCoor;
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
	int timeSlot;

	vector<GSTEBNeighbors> tableI;
	vector<GSTEBNeighborsOfNeighbors> tableII;
	double myEL;
	int phase;

	vector<pair<int, GSTEBNeighbors>> networkTableI;
	map<int, int> networkParentTable;

	int phaseITimeslot;
	int sensorBroadcastTimeout;
	int initialPhraseTimeout; //The length of time slots in Steps 2 and 3 is predefined
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif				//BYPASSROUTINGMODULE
