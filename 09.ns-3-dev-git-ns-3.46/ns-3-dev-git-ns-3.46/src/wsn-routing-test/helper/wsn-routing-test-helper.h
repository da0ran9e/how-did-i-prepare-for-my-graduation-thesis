#ifndef WSN_ROUTING_TEST_HELPER_H
#define WSN_ROUTING_TEST_HELPER_H

#include <iostream>

namespace ns3
{
  
class WsnRoutingTestHelper : public Ipv4RoutingHelper
{
  public:
    WsnRoutingTestHelper();

    void getTestHelperAttribute();
    int setTestHelperAttribute(int v);

  private:
    int m_testHelperAttribute;
};

} // namespace ns3

#endif /* WSN_ROUTING_TEST_HELPER_H */
