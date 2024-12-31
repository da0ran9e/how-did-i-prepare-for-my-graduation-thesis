#ifndef _BYPASSROUTING_H_
#define _BYPASSROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

using namespace std;

class WSNRouting: public VirtualRouting {
 private:
 	char * sinkAdd;
	int level = -1;
	char * prevHop;
	char * nextHop;
 protected:
 	void startup();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);

	/*
	* Process incoming package after 
	*/
	void processIncoming(WSNRoutingPacket *);
	void processOutgoing(WSNRoutingPacket *)
	void acceptHop(WSNRoutingPacket *);
	void onAck(WSNRoutingPacket *);
	void onBC(WSNRoutingPacket *);
};

#endif				
