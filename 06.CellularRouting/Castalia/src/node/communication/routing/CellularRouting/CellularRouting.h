#ifndef _CELLULARROUTING_H_
#define _CELLULARROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/cellularRouting/CellularRoutingPacket_m.h"

using namespace std;

class CellularRouting: public VirtualRouting {
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif			
