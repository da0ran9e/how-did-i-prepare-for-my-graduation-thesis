#ifndef _BYPASSROUTING_H_
#define _BYPASSROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

using namespace std;

class WSNRouting: public VirtualRouting {
 private:
    bool isSink;
	std::string cluster[10];
	int lastPkt = -1;
 protected:
 	void startup();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	//void finish();
};

#endif				
