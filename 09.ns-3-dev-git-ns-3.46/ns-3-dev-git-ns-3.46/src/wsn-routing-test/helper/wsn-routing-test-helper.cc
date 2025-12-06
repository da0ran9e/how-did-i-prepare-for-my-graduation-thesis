#include "wsn-routing-test-helper.h"

namespace ns3
{

WsnRoutingTestHelper::WsnRoutingTestHelper()
    : ()m_testHelperAttribute(20)
{
    std::cout << "WsnRoutingTestHelper created." << std::endl;
}

void WsnRoutingTestHelper::getTestHelperAttribute()
{
    std::cout << "Test Helper Attribute retrieved: " << m_testHelperAttribute << std::endl;
}
int WsnRoutingTestHelper::setTestHelperAttribute(int v)
{
    m_testHelperAttribute = v;
    std::cout << "Test Helper Attribute set to: " << m_testHelperAttribute << std::endl;
    return m_testHelperAttribute;
}

} // namespace ns3
