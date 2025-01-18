#ifndef _BYPASSROUTING_H_
#define _BYPASSROUTING_H_

#include "node/communication/routing/VirtualRouting.h"
#include "node/communication/routing/WSNRouting/WSNRoutingPacket_m.h"

using namespace std;

class WSNRouting: public VirtualRouting {
 private:
    bool isSink;
	std::string cluster[10];
	std::string origin;
	std::string ch = "-1";
	int lastPkt = -1;
	std::string bHop;
	std::vector<std::string> clusterUnits;
	std::map<std::string, std::string> routingTable;
	
 protected:
 	void startup();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);
	//void finish();
};

#endif				
