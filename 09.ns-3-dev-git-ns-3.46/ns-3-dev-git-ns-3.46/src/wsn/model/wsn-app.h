#pragma once
#ifndef WSN_APP_H
#define WSN_APP_H

#include "ns3/application.h"
#include "ns3/event-id.h"

namespace ns3 {
namespace wsn {
class WsnApp : public Application
{
public:
    static TypeId GetTypeId();
    WsnApp();

private:
    virtual void StartApplication();
    virtual void StopApplication();
    void GeneratePacket();
};
} // namespace wsn
} // namespace ns3

#endif // WSN_APP_H