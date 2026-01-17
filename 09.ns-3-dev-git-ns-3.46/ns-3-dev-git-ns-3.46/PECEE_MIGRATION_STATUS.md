# PECEE Routing Protocol Migration Status

## Completed ✅

### 1. Header Files Structure
- ✅ Uncommented all data structures in `pecee-routing-protocol.h`
- ✅ Added necessary includes (vector, map, queue, etc.)
- ✅ Fixed include path from `pecee-routing-header.h` to `pecee-header.h`
- ✅ Uncommented all private member variables
- ✅ Added timer enums
- ✅ Added global variables for simulation state

### 2. Packet Header Implementation
- ✅ Implemented `PeceeHeader` class with full serialization support
- ✅ Added packet type enums
- ✅ Added data structures (SSCHAnnouncementInfo, SSCellHopAnnouncementInfo, SSSensorInfo)
- ✅ Implemented Serialize/Deserialize methods

### 3. Core Methods
- ✅ Fixed TypeId naming (PeeceRoutingProtocol → PeceeRoutingProtocol)
- ✅ Implemented constructor with proper member initialization
- ✅ Implemented `Start()` method with:
  - Node registration in global lists
  - Cell calculation
  - Pre-calculation scheduling
  - CH announcement scheduling
- ✅ Implemented `FromMacLayer()` with packet type routing

## Remaining Work ⚠️

### Critical Refactoring Needed

The code was copied from OMNeT++ which uses different paradigms than ns-3. Major changes needed:

#### 1. Packet Management
**Problem:** Code uses `PeceeRoutingProtocolPacket*` (OMNeT++ style)
**Solution:** Need to use `Ptr<Packet>` with `PeceeHeader`

Example changes needed:
```cpp
// OLD (OMNeT++):
PeceeRoutingProtocolPacket* pkt = new PeceeRoutingProtocolPacket("CH Announcement", NETWORK_LAYER_PACKET);
pkt->setPacketType(CH_ANNOUNCEMENT_PACKET);
pkt->setCellSource(myCellId);

// NEW (ns-3):
Ptr<Packet> pkt = Create<Packet>();
PeceeHeader header;
header.SetPacketType(CH_ANNOUNCEMENT_PACKET);
header.SetCellSource(myCellId);
pkt->AddHeader(header);
```

Affected methods:
- `sendCHAnnouncement()`
- `handleCHAnnouncementPacket()`
- `sendCellHopAnnouncementPacket()`
- `handleCellHopAnnouncementPacket()`
- `sendSensorDataPacket()`
- `handleSensorDataPacket()`
- `sendAnnouncementQueue()`
- `sendCellPacket()`
- `savePacketCopy()`

#### 2. Timer Management
**Problem:** Code uses `setTimer(type, delay)` (OMNeT++ style)
**Solution:** Use `Simulator::Schedule()`

Example changes needed:
```cpp
// OLD (OMNeT++):
setTimer(SEND_ANNOUNCEMENT_QUEUE, uniform(1, 10));

// NEW (ns-3):
Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
double delay = rand->GetValue(1.0, 10.0);
Simulator::Schedule(MilliSeconds(delay), &PeceeRoutingProtocol::sendAnnouncementQueue, this);
```

All `setTimer()` calls need replacement (~15-20 locations)

#### 3. Random Number Generation
**Problem:** Code uses `uniform(min, max)` (OMNeT++ style)
**Solution:** Use ns-3's RandomVariableStream

```cpp
// OLD (OMNeT++):
uniform(0, 1)

// NEW (ns-3):
Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
rand->GetValue(0.0, 1.0)
```

#### 4. MAC Layer Interface
**Problem:** Code uses `toMacLayer(pkt, dest)` (lowercase 't')
**Solution:** Use `ToMacLayer(pkt, dest)` (capital 'T')

All `toMacLayer()` calls need to be changed to `ToMacLayer()` (~10 locations)

#### 5. String/Integer Conversions
**Problem:** Code uses `atoi(pkt->getSource())` and `std::to_string(x).c_str()`
**Solution:** Headers use `uint16_t` for addresses, not strings

Example:
```cpp
// OLD:
int sourceId = atoi(pkt->getSource());
pkt->setDestination(std::to_string(memberId).c_str());

// NEW:
uint16_t sourceId = header.GetSource();
header.SetDestination(memberId);
```

#### 6. Resource/Energy Module Integration
**Problem:** Code references `resModule->getSpentEnergy()`
**Solution:** Need to integrate with ns-3 energy model

Lines like:
```cpp
getNodeData(self)->castaliaConsumtion = resModule->getSpentEnergy();
```

Need to be replaced with proper ns-3 energy model API calls.

#### 7. Trace/Output System
**Problem:** Code uses `std::cout` for tracing
**Solution:** Use ns-3's `NS_LOG_*` macros or TraceSources

```cpp
// OLD:
if (traceMode == 0) std::cout << "#ROUTING_TABLE " << nodeId;

// NEW:
NS_LOG_INFO("#ROUTING_TABLE " << nodeId);
```

### Implementation Strategy

Given the scale of changes, I recommend:

1. **Phase 1: Helper Methods**
   - Add timer scheduling wrapper methods
   - Add packet creation/extraction helpers
   - Add random number generation helpers

2. **Phase 2: Method-by-Method Conversion**
   - Convert one method at a time
   - Test each method after conversion
   - Start with simpler methods (getters, calculators)
   - Then move to packet handlers

3. **Phase 3: Integration**
   - Wire up all timer callbacks
   - Test end-to-end packet flow
   - Integrate energy model
   - Add proper trace support

### Estimated Lines Requiring Changes
- ~60 lines for packet creation/manipulation
- ~20 lines for timer scheduling
- ~15 lines for MAC layer calls
- ~30 lines for header access patterns
- ~10 lines for energy model integration

**Total: ~135 lines of significant refactoring needed**

## Next Steps

Would you like me to:
1. **Complete the full refactoring** (all methods converted to ns-3 style)
2. **Create helper methods first** (then you convert methods incrementally)
3. **Convert specific methods** (tell me which ones to prioritize)
4. **Provide a detailed conversion guide** (with examples for each pattern)

## Build System Integration

Also need to verify:
- [ ] Add PECEE files to `src/wsn/CMakeLists.txt`
- [ ] Check if example simulation files are configured
- [ ] Verify all dependencies are linked

## Testing Checklist

Once conversion is complete:
- [ ] Build without errors
- [ ] Node initialization works
- [ ] Cell calculation correct
- [ ] Routing table pre-calculation works
- [ ] CH announcement propagates
- [ ] Sensor data forwarding works
- [ ] Energy tracking functions
- [ ] CH rotation mechanism works
