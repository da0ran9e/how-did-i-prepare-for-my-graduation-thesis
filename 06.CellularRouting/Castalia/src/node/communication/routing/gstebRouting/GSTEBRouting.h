#ifndef _GSTEBROUTING_H_
#define _GSTEBROUTING_H_

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

class GSTEBRouting: public VirtualRouting {
	private:
	int nodeId;
	int xCoor;
	int yCoor;
	double energy;
	bool isCH;
	bool isSink;

	int phaseITimeslot;
	int sensorBroadcastTimeout;
	int initialPhraseTimeout; //The length of time slots in Steps 2 and 3 is predefined
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif				//BYPASSROUTINGMODULE
