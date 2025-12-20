// wsn-cellular-header.h
#ifndef WSN_CELLULAR_HEADER_H
#define WSN_CELLULAR_HEADER_H


#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/address.h"


namespace ns3 {
namespace wsn {

enum WsnCellularMsgType : uint8_t
{
    WSN_MSG_TIC = 1,
    WSN_MSG_TOC = 2
};


class WsnCellularHeader : public Header
{
public:
    WsnCellularHeader();
    ~WsnCellularHeader() override;


    void SetMsgType(WsnCellularMsgType type);
    void SetSeq(uint32_t seq);
    void SetSrcNodeId(uint32_t id);


    WsnCellularMsgType GetMsgType() const;
    uint32_t GetSeq() const;
    uint32_t GetSrcNodeId() const;


    static TypeId GetTypeId();
    virtual TypeId GetInstanceTypeId() const override;
    virtual void Serialize(Buffer::Iterator start) const override;
    virtual uint32_t Deserialize(Buffer::Iterator start) override;
    virtual uint32_t GetSerializedSize() const override;
    virtual void Print(std::ostream &os) const override;

private:
    uint8_t m_msgType; // TIC / TOC
    uint32_t m_seq; // sequence number
    uint32_t m_srcNodeId; // sender node id
};


} // namespace wsn
} // namespace ns3


#endif // WSN_CELLULAR_HEADER_H