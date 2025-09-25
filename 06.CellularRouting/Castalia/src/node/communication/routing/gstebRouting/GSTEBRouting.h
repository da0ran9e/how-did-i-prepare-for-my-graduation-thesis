#ifndef _GSTEBROUTING_H_
#define _GSTEBROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/gstebRouting/GSTEBRoutingPacket_m.h"

using namespace std;

class GSTEBRouting: public VirtualRouting {
	private:
	int nodeId;
	int xCoor;
	int yCoor;
	double energy;
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif				//BYPASSROUTINGMODULE
