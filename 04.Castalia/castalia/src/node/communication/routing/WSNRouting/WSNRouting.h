#ifndef _BYPASSROUTING_H_
#define _BYPASSROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

using namespace std;

class WSNRouting: public VirtualRouting {
 protected:
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
};

#endif				