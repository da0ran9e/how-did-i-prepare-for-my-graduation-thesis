#pragma once
#ifndef WSN_MAC_H
#define WSN_MAC_H

#include <string>

#include "wsn-object.h"

namespace ns3 {
namespace wsn {
class WsnMac : public ns3::wsn::WsnObject
{
public:
    explicit WsnMac(const std::string& name) : WsnObject("WsnMac", name),
        macMaxPacketSize(127),
        macBufferSize(50),
        macPacketOverhead(11)
    {
    }
    ~WsnMac() override = default;

    bool SetProperty(const std::string &key, const std::string &value) override;
    void Build() override;
    std::string GetTypeName() const override { return "MAC"; }

private:
	int macMaxPacketSize;	// in bytes
	int macBufferSize;		// in number of messages
	int macPacketOverhead;

}; // in bytes

} // namespace wsn
} // namespace ns3