#include "ns3/core-module.h"
#include "ns3/wsn-module.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    std::string iniFile = "input.ini";
    std::string traceFile = "trace.txt";

    CommandLine cmd;
    cmd.AddValue("ini", "Input ini file", iniFile);
    cmd.AddValue("trace", "Output trace file", traceFile);
    cmd.Parse(argc, argv);

    IniParser ini;
    if (!ini.Load(iniFile)) {
        NS_FATAL_ERROR("Cannot load INI file");
    }

    WsnScenario scenario;
    scenario.Configure(ini);
    scenario.Trace(traceFile);

    std::cout << "INI loaded. Trace written to " << traceFile << std::endl;

    return 0;
}
