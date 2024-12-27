#ifndef _NETFORMATION_H_
#define _NETFORMATION_H_

#include  "node/application/VirtualApplication.h"
#include <map>

using namespace std;

enum FloodAppTimers {
	SEND_PACKET = 1
};

class Netformation: public VirtualApplication {

private:
 
	double startupDelay;
	double delayLimit;
	double packet_spacing;
	int dataSN;
	int recipientId;
	std::string recipientAddress;
	
	std::FILE* log;												/**< @brief Logging file pointer */
	
	//variables below are used to determine the packet delivery rates.	
	int numNodes;
	std::map<long,int> packetsReceived;
	std::map<long,int> bytesReceived;
	std::map<long,int> packetsSent;

protected:
	void startup();
	void fromNetworkLayer(ApplicationPacket*, const char*, double, double);
	void handleRadioControlMessage(RadioControlMessage*);
	void timerFiredCallback(int);
	void finishSpecific();

public:
	int getPacketsSent(int addr) { return packetsSent[addr]; }
	int getPacketsReceived(int addr) { return packetsReceived[addr]; }
	int getBytesReceived(int addr) { return bytesReceived[addr]; }
	
};

#endif				// _FLOODAPP_APPLICATIONMODULE_H_
