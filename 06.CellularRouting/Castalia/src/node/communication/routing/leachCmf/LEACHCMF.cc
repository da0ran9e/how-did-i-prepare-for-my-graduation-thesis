#include "node/communication/routing/leachCmf/LEACHCMF.h"

Define_Module(LEACHCMF);


void LEACHCMF::startup()
{
    cellRadius = par("cellRadius");
    isCH = par("isCH"); 
    isSink = par("isSink");

    if (isSink) {
        if (traceMode == 0) trace() << "#Sink " << self;
    } else if (isCH) {
        if (traceMode == 0) trace() << "#CH " << self;
    } else {
        if (traceMode == 0) trace() << "#Node " << self;
    }
}

void LEACHCMF::timerFiredCallback(int index)
{
    switch (index) {
        case PRECALCULATE_TIMERS:
            // Precalculate the simulation results
            PrecalculateSimulationResults();
            break;
    }
}

void LEACHCMF::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void LEACHCMF::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    
}
