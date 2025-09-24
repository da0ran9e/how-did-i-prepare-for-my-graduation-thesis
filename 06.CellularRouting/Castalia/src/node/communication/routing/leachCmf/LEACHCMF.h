#ifndef _LEACHCMF_H_
#define _LEACHCMF_H_

#include "node/communication/routing/VirtualRouting.h"
#include "LEACHCMFPacket_m.h"
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <cmath>
#include <algorithm>

using namespace std;

struct NodeRoutingUpdateInfo {
    
};

class CellularRouting : public VirtualRouting {
 private:
    double cellRadius;       
    bool isCH;   
    bool isSink;    


 protected:
    void startup() override;
    void timerFiredCallback(int) override;
    void fromApplicationLayer(cPacket *, const char *) override;
    void fromMacLayer(cPacket *, int, double, double) override;
};

#endif //_LEACHCMF_H_
