#ifndef NODE_COMMUNICATION_ROUTING_NETFORMATION_NETFORMATION_H_
#define NODE_COMMUNICATION_ROUTING_NETFORMATION_NETFORMATION_H_

#include "node/communication/routing/VirtualRouting.h"

class NetFormation: public VirtualRouting {

protected:
    void fromMacLayer(cPacket*, int, double, double);
    void fromApplicationLayer(cPacket* pkt, const char*);
};

#endif /* NODE_COMMUNICATION_ROUTING_NETFORMATION_NETFORMATION_H_ */
