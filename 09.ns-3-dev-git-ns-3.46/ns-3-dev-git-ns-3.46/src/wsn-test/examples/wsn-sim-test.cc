#include "ns3/core-module.h"
#include "ns3/wsn-module.h"

using namespace ns3;

class WsnSimTest
{
public:
    WsnSimTest();
    bool configure(int argc, char *argv[]);
    void run();
    void report(std::ostream &os);
    
private:
    int m_test_value;
};

int main(int argc, char *argv[])
{
    WsnSimTest sim;
    if (!sim.configure(argc, argv)) {
        NS_FATAL_ERROR("Configuration failed.");
    }

    sim.run();
    sim.report(std::cout);
    std::cout << "Test done" << std::endl;

    return 0;
}

WsnSimTest::WsnSimTest()
    : m_test_value(10)
{
}

bool WsnSimTest::configure(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.AddValue("test-value", "Input INI file", m_test_value);
    cmd.Parse(argc, argv);

    WsnTestModel model;
    model.setValue(m_test_value);
    model.getValue();
    return true;
}


void WsnSimTest::run()
{

}

void WsnSim::report(std::ostream &os)
{
    os << "Simulation completed." << std::endl;
}
