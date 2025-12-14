#pragma once
#ifndef NODE_H
#define NODE_H

#include <string>

#include "wsn-object.h"

namespace ns3 {
namespace wsn { 

class Node : public ns3::wsn::WsnObject
{
public:
    Node() : WsnObject("node", ""),
        xCoor(0.0),
        yCoor(0.0),
        zCoor(0.0),
        phi(0.0),
        theta(0.0),
        startupOffset(0.0),
        startupRandomization(0.05),
        ApplicationName(""),
        MACProtocolName(""),
        RadioProtocolName(""),
        RoutingProtocolName("")
    {
    };
    ~Node() override = default;
    
    bool SetProperty(const std::string &key, const std::string &value) override;
    void Build() override;

private:
    double xCoor; // default (0);
	double yCoor; // default (0);
	double zCoor; // default (0);
	double phi; // default (0);
	double theta; // default (0);
	
	double startupOffset; // default (0);							//node startup offset (i.e. delay), in seconds 
	double startupRandomization; // default (0.05);				//node startup randomisation, in seconds
	// Node will become active startupOffset + random(startupRandomization) 
	// seconds after the start of simulation

	std::string ApplicationName;										//the name of the implemented Application Module
	std::string MACProtocolName;										//the name of the implemented MAC Protocol Module
    std::string RadioProtocolName;									//the name of the implemented Radio Protocol Module
    std::string RoutingProtocolName;									//the name of the implemented Routing Protocol Module
};

}// namespace wsn
} // namespace ns3
#endif // NODE_H