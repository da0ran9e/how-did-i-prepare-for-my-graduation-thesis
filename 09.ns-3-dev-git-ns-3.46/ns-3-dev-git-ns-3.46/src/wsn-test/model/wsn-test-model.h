#ifndef WSN_TEST_MODEL_H
#define WSN_TEST_MODEL_H

#include <iostream>      

namespace ns3 {
namespace wsn {

class WsnTestModel
{
private:
    int m_value;
public:
    int setValue(int v);
    int getValue();

};

} // namespace wsn
} // namespace ns3

#endif // WSN_TEST_MODEL_H