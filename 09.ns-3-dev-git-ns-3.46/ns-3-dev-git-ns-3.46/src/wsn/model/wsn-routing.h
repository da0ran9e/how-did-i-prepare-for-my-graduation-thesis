#ifndef WSN_ROUTING_H
#define WSN_ROUTING_H

#include "wsn-object.h"

namespace ns3 {
namespace wsn {

class WsnRoutingProtocol : public ns3::wsn::WsnObject
{
public:
    WsnRoutingProtocol() : WsnObject("WsnRoutingProtocol", ""),
        collectTraceInfo(false),
        maxNetFrameSize(0),
        netDataFrameOverhead(0),
        netBufferSize(0) {}
    ~WsnRoutingProtocol() override = default;
    bool SetProperty(const std::string &key, const std::string &value) override;
    void Build() override;

private:
    bool collectTraceInfo;
	int maxNetFrameSize;		// in bytes
	int netDataFrameOverhead;	// in bytes
	int netBufferSize;			// in number of messages
};

} // namespace wsn
} // namespace ns3

#endif
