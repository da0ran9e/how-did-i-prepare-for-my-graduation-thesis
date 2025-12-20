#include "wsn-cellular-header.h"

namespace ns3 {
namespace wsncellular {

NS_OBJECT_ENSURE_REGISTERED(WsnCellularHeader);

WsnCellularHeader::WsnCellularHeader()
  : m_src(0),
    m_dst(0),
    m_seq(0),
    m_type(DATA)
{
}

WsnCellularHeader::~WsnCellularHeader()
{
}

uint32_t
WsnCellularHeader::GetSerializedSize() const
{
  return 2 + 2 + 2 + 1;
}

void
WsnCellularHeader::Serialize(Buffer::Iterator i) const
{
  i.WriteU16(m_src);
  i.WriteU16(m_dst);
  i.WriteU16(m_seq);
  i.WriteU8(static_cast<uint8_t>(m_type));
}

uint32_t
WsnCellularHeader::Deserialize(Buffer::Iterator i)
{
  m_src = i.ReadU16();
  m_dst = i.ReadU16();
  m_seq = i.ReadU16();
  m_type = static_cast<MsgType>(i.ReadU8());
  return GetSerializedSize();
}


void
WsnCellularHeader::Print(std::ostream &os) const
{
os << "[WSN-CELLULAR hdr] type=" << uint32_t(m_type)
<< " seq=" << m_seq
<< " srcNode=" << m_src;
}


} // namespace wsncellular
} // namespace ns3