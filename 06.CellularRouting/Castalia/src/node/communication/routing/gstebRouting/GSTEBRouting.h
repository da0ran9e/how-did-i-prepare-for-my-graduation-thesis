#ifndef _GSTEBROUTING_H_
#define _GSTEBROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/gstebRouting/GSTEBRoutingPacket_m.h"

using namespace std;

enum GSTEBRoutingTimers {
	BS_BROADCAST = 0,
	PHRASE_A,
	NEIGHBOR_DISCOVERY,
	NEIGHBOR_EXCHANGE,
	SCHEDULE_RX,
	ACK_BS,
	PHRASE_B,
	BS_SEND_CTRL,
	PHRASE_C,
};

class GSTEBRouting: public VirtualRouting {
	private:
	int nodeId;
	int xCoor;
	int yCoor;
	double energy;
	bool isCH;
	bool isSink;
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif				//BYPASSROUTINGMODULE
