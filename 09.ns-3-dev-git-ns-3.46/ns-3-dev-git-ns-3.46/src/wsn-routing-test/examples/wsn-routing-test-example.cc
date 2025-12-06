#include "ns3/wsn-routing-test-module.h"
#include "ns3/core-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/mobility-module.h"
// #include "ns3/network-module.h"
// #include "ns3/ping-helper.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/yans-wifi-helper.h"

// #include <cmath>
#include <iostream>

using namespace ns3;

class WsnRoutingTestExample
{
  public:
    WsnRoutingTestExample();
    bool Configure(int argc, char** argv);
    void Run();
    void Report(std::ostream& os);

  private:
    int exampleParameter;
};

int
main(int argc, char** argv)
{
    WsnRoutingTestExample test;
    if (!test.Configure(argc, argv))
    {
        NS_FATAL_ERROR("Configuration failed. Aborted.");
    }

    test.Run();
    test.Report(std::cout);
    return 0;
}

//-----------------------------------------------------------------------------
WsnRoutingTestExample::WsnRoutingTestExample()
    : exampleParameter(0)
{
    std::cout << "WsnRoutingTestExample created." << std::endl;
}

bool
WsnRoutingTestExample::Configure(int argc, char** argv)
{
    SeedManager::SetSeed(12345);
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);
    return true;
}

void
WsnRoutingTestExample::Run()
{
    std::cout << "Starting simulation";

    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();
}

void
WsnRoutingTestExample::Report(std::ostream&)
{
}
