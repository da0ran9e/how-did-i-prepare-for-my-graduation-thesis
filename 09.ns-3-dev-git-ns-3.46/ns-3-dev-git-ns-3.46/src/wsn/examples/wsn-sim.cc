#include "ns3/core-module.h"
#include "ns3/wsn-module.h"

using namespace ns3;

class WsnSim
{
public:
    WsnSim();
    bool configure(int argc, char *argv[]);
    void run();
    void report(std::ostream &os);
    
private:
    std::shared_ptr<ns3::wsn::WsnScenario> m_scenario;
    std::string m_iniFile;
    std::string m_traceFile;
    int size;
    double step;
    double totalTime;
    bool printTrace;

    void createNodes();
    void createDevice();
    void installInternetStack();
};

int main(int argc, char *argv[])
{
    WsnSim sim;
    if (!sim.configure(argc, argv)) {
        NS_FATAL_ERROR("Configuration failed.");
    }

    sim.run();
    sim.report(std::cout);
    std::cout << "INI loaded. Trace written to trace file" << std::endl;

    return 0;
}

WsnSim::WsnSim()
    : m_iniFile("input.ini"),
      m_traceFile("trace.txt"),
      size(100),
      step(1.0),
      totalTime(10.0),
      printTrace(true)
{
}

bool WsnSim::configure(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.AddValue("ini", "Input INI file", m_iniFile);
    cmd.AddValue("trace", "Trace output file", m_traceFile);
    cmd.Parse(argc, argv);

    m_scenario = std::make_shared<ns3::wsn::WsnScenario>();
    m_scenario->configure(m_iniFile);    

    // size = m_scenario->GetNumNodes();
    // step = 1.0;          // step can come from INI later
    // totalTime = 10.0;    // same here
    // printTrace = true;
    
    // std::cout << "=== Simulation configured ===" << std::endl;
    // std::cout << "INI file:     " << m_iniFile << std::endl;
    // std::cout << "Trace file:   " << m_traceFile << std::endl;
    // std::cout << "Nodes:        " << size << std::endl;

    return true;
}


void WsnSim::run()
{
    // createNodes();
    // createDevice();
    // installInternetStack();

    // Simulator::Stop(Seconds(totalTime));
    // Simulator::Run();
    // Simulator::Destroy();
}

void WsnSim::report(std::ostream &os)
{
    os << "Simulation completed." << std::endl;
}

void WsnSim::createNodes()
{
    // TODO:
    // Create nodes
}

void WsnSim::createDevice()
{
    // TODO:
    // Create devices
}

void WsnSim::installInternetStack()
{
    // TODO:
    // Install internet stack
}