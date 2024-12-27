//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "NetFormation.h"
#include "NetFormationPacket_m.h"

Define_Module(NetFormation);
//BROADCAST_SINK_IDS     = 1;
//SINK_ADVERTISE_CLUSTER = 2;
//ACCEPT_JOIN_CLUSTER    = 3;

void NetFormation::fromMacLayer(cPacket* pkt, int sourceId, double unknown, double RSSI){
    //process packet from other node
    NetFormationPacket* nFPkt = check_and_cast<NetFormationPacket*>(pkt);

    switch(nFPkt->getNetFormationPacketKind()){
    case BROADCAST_SINK_IDS:
        //some processing
        // check_memory
        // add to mem if unknown sink
        // if unknown sink, forward to neighbors toMacLayer(cPacket* pkt); with nFPkt = new ...
        // if known sink, not forwarding.
        break; //no return, just break to preserve packet, return deletes the packet
    }
}

void NetFormation::fromApplicationLayer(cPacket* pkt, const char*){
    //process packet from other node
}
