#include "pecee-header.h"
#include "ns3/log.h"

namespace ns3 {
namespace wsn {

NS_LOG_COMPONENT_DEFINE("PeceeHeader");
NS_OBJECT_ENSURE_REGISTERED(PeceeHeader);

PeceeHeader::PeceeHeader()
    : m_packetType(HELLO_PACKET),
      m_clusterHead(-1),
      m_cellSent(-1),
      m_cellNext(-1),
      m_cellNextNext(-1),
      m_cellSource(-1),
      m_cellDestination(-1),
      m_cellHopCount(0),
      m_ttl(0)
{
    for (int i = 0; i < 1000; i++) {
        m_cellPath[i] = -1;
    }
    m_sensorData.destinationCH = -1;
    m_sensorData.dataId = 0;
    m_sensorData.sensorId = -1;
    m_sensorData.hopCount = 0;
    m_chAnnouncementData.chId = -1;
    m_cellHopAnnouncementData.nextCell = -1;
}

PeceeHeader::~PeceeHeader()
{
}

TypeId PeceeHeader::GetTypeId()
{
    static TypeId tid = TypeId("ns3::wsn::PeceeHeader")
        .SetParent<WsnRoutingHeader>()
        .SetGroupName("Wsn")
        .AddConstructor<PeceeHeader>();
    return tid;
}

TypeId PeceeHeader::GetInstanceTypeId() const
{
    return GetTypeId();
}

uint32_t PeceeHeader::GetSerializedSize() const
{
    // Base header + packet type + cell info + paths + sensor data + announcement data
    // Simplified: return fixed size for now
    return WsnRoutingHeader::GetSerializedSize() + 4 + 32 + 4000 + 24 + 8 + 4008;
}

void PeceeHeader::Serialize(Buffer::Iterator start) const
{
    WsnRoutingHeader::Serialize(start);
    
    start.WriteU32(static_cast<uint32_t>(m_packetType));
    start.WriteU32(m_clusterHead);
    start.WriteU32(m_cellSent);
    start.WriteU32(m_cellNext);
    start.WriteU32(m_cellNextNext);
    start.WriteU32(m_cellSource);
    start.WriteU32(m_cellDestination);
    start.WriteU32(m_cellHopCount);
    start.WriteU32(m_ttl);
    
    // Write cell path array
    for (int i = 0; i < 1000; i++) {
        start.WriteU32(m_cellPath[i]);
    }
    
    // Write sensor data
    start.WriteU32(m_sensorData.destinationCH);
    start.WriteU32((uint32_t)m_sensorData.dataId);  // Convert double to uint32 for ns-3 compatibility
    start.WriteU32(m_sensorData.sensorId);
    start.WriteU32(m_sensorData.hopCount);
    
    // Write CH announcement data
    start.WriteU32(m_chAnnouncementData.chId);
    
    // Write cell hop announcement data
    start.WriteU32(m_cellHopAnnouncementData.nextCell);
    for (int i = 0; i < 1000; i++) {
        start.WriteU32(m_cellHopAnnouncementData.cellPath[i]);
    }
}

uint32_t PeceeHeader::Deserialize(Buffer::Iterator start)
{
    WsnRoutingHeader::Deserialize(start);
    
    m_packetType = static_cast<PeceePacketType>(start.ReadU32());
    m_clusterHead = start.ReadU32();
    m_cellSent = start.ReadU32();
    m_cellNext = start.ReadU32();
    m_cellNextNext = start.ReadU32();
    m_cellSource = start.ReadU32();
    m_cellDestination = start.ReadU32();
    m_cellHopCount = start.ReadU32();
    m_ttl = start.ReadU32();
    
    // Read cell path array
    for (int i = 0; i < 1000; i++) {
        m_cellPath[i] = start.ReadU32();
    }
    
    // Read sensor data
    m_sensorData.destinationCH = start.ReadU32();
    m_sensorData.dataId = (double)start.ReadU32();  // Convert uint32 back to double
    m_sensorData.sensorId = start.ReadU32();
    m_sensorData.hopCount = start.ReadU32();
    
    // Read CH announcement data
    m_chAnnouncementData.chId = start.ReadU32();
    
    // Read cell hop announcement data
    m_cellHopAnnouncementData.nextCell = start.ReadU32();
    for (int i = 0; i < 1000; i++) {
        m_cellHopAnnouncementData.cellPath[i] = start.ReadU32();
    }
    
    return GetSerializedSize();
}

void PeceeHeader::Print(std::ostream &os) const
{
    WsnRoutingHeader::Print(os);
    os << " PacketType=" << m_packetType
       << " CellSrc=" << m_cellSource
       << " CellDst=" << m_cellDestination
       << " CH=" << m_clusterHead
       << " TTL=" << m_ttl;
}

}
}
