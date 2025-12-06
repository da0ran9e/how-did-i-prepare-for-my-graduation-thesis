#include "wsn-test-model.h"

namespace ns3 {
namespace wsn {

int WsnTestModel::setValue(int v)
{
    m_value = v;
    std::cout << "Value set to: " << m_value << std::endl;
    return m_value;
}

int WsnTestModel::getValue()
{
    std::cout << "Value retrieved: " << m_value << std::endl;
    return m_value;
}

} // namespace wsn
} // namespace ns3