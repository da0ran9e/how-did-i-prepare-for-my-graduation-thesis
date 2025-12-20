// wsn-cellular-header.h
#ifndef WSN_CELLULAR_HEADER_H
#define WSN_CELLULAR_HEADER_H


#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/address.h"


namespace ns3 {
namespace wsncellular {

enum WsnCellularMsgType : uint8_t
{
    WSN_MSG_TIC = 1,
    WSN_MSG_TOC = 2
};


class WsnCellularHeader : public Header
{
public:
  enum MsgType : uint8_t
  {
    DATA = 1,
    BEACON = 2
  };

  WsnCellularHeader();

  static TypeId GetTypeId();
  TypeId GetInstanceTypeId() const override;

  void SetType(MsgType t) { m_type = t; }
  MsgType GetType() const { return m_type; }

  void SetSrc(uint16_t src){ m_src = src; };
  void SetDst(uint16_t dst){ m_dst = dst; };
  void SetSeq(uint16_t seq){ m_seq = seq; };
  void SetMsgType(MsgType type){ m_type = type; };

  uint16_t GetSrc() const { return m_src; };
  uint16_t GetDst() const { return m_dst; };
  uint16_t GetSeq() const { return m_seq; };
  MsgType GetMsgType() const { return m_type; };

  uint32_t GetSerializedSize() const override;
  void Serialize(Buffer::Iterator start) const override;
  uint32_t Deserialize(Buffer::Iterator start) override;
  void Print(std::ostream &os) const override;

private:
  uint16_t m_src;
  uint16_t m_dst;
  uint16_t m_seq;
  MsgType  m_type;
};



} // namespace wsncellular
} // namespace ns3


#endif // WSN_CELLULAR_HEADER_H