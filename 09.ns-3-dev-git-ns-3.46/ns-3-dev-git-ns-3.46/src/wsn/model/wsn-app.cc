#include "wsn-app.h"

namespace ns3 {
namespace wsn {
    
NS_OBJECT_ENSURE_REGISTERED(WsnApp);

TypeId WsnApp::GetTypeId()
{
    static TypeId tid = TypeId("ns3::wsn::WsnApp")
        .SetParent<Application>()
        .SetGroupName("Wsn");
    return tid;
}

WsnApp::WsnApp() {}

void WsnApp::StartApplication()
{
}

void WsnApp::StopApplication()
{
}

} // namespace wsn
} // namespace ns3