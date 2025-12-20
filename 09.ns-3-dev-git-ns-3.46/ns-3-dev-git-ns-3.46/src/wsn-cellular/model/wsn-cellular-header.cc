#include "wsn-cellular-header.h"

namespace ns3 {
namespace wsn-cellular {

NS_OBJECT_ENSURE_REGISTERED(WsnCellularHeader);

WsnCellularHeader::WsnCellularHeader()
    : m_msgType(0),
      m_seq(0),
      m_srcNodeId(0)
{
}

WsnCellularHeader::~WsnCellularHeader()
{
}
    
void
WsnCellularHeader::SetSeq(uint32_t seq)
{
    m_seq = seq;
}
    

void
WsnCellularHeader::SetSrcNodeId(uint32_t id)
{
    m_srcNodeId = id;
}


WsnCellularMsgType
WsnCellularHeader::GetMsgType() const
{
    return static_cast<WsnCellularMsgType>(m_msgType);
}


uint32_t
WsnCellularHeader::GetSeq() const
{
    return m_seq;
}


uint32_t
WsnCellularHeader::GetSrcNodeId() const
{
    return m_srcNodeId;
}


uint32_t
WsnCellularHeader::GetSerializedSize() const
{
    return 1 + 4 + 4; // msgType + seq + srcNodeId
}


void
WsnCellularHeader::Serialize(Buffer::Iterator start) const
{
    Buffer::Iterator i = start;
    i.WriteU8(m_msgType);
    i.WriteHtonU32(m_seq);
    i.WriteHtonU32(m_srcNodeId);
}


uint32_t
WsnCellularHeader::Deserialize(Buffer::Iterator start)
{
    Buffer::Iterator i = start;
    m_msgType = i.ReadU8();
    m_seq = i.ReadNtohU32();
    m_srcNodeId = i.ReadNtohU32();
    return GetSerializedSize();
}


void
WsnCellularHeader::Print(std::ostream &os) const
{
os << "[WSN-CELLULAR hdr] type=" << uint32_t(m_msgType)
<< " seq=" << m_seq
<< " srcNode=" << m_srcNodeId;
}


} // namespace wsn-cellular
} // namespace ns3