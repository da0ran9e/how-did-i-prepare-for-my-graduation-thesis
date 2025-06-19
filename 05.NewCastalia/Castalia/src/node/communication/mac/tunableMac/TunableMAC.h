/****************************************************************************
 * Copyright: National ICT Australia,  2007 - 2010                         *
 * Developed at the ATP lab, Networked Systems research theme              *
 * Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 * This file is distributed under the terms in the attached LICENSE file.  *
 * If you do not find this file, copies can be found by writing to:        *
 * *
 * NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 * Attention:  License Inquiry.                                        *
 * *
 ****************************************************************************/

#ifndef TUNABLEMACMODULE
#define TUNABLEMACMODULE

#include "node/communication/mac/VirtualMac.h"
#include "node/communication/mac/tunableMac/TunableMacControl_m.h"
#include "node/communication/mac/tunableMac/TunableMacPacket_m.h"

using namespace std;

enum backoffTypes {
    BACKOFF_CONSTANT = 2201,
    BACKOFF_MULTIPLYING = 2202,
    BACKOFF_EXPONENTIAL = 2203
};

enum MacStates {
    MAC_STATE_DEFAULT = 1,
    MAC_STATE_TX = 2,
    MAC_STATE_CONTENDING = 3,
    MAC_STATE_RX = 4,
    MAC_STATE_WAIT_FOR_CTS = 5,
    MAC_STATE_WAIT_FOR_ACK = 6
};

enum TunableMacTimers {
    START_CARRIER_SENSING = 3,
    ATTEMPT_TX = 4,
    SEND_DATA_PACKET = 5,
    CTS_TIMEOUT = 6,
    ACK_TIMEOUT = 7
};

class TunableMAC: public VirtualMac {
 private:
    /*--- Tham số từ file .ned ---*/
    double probTx;
    int numTx;
    double reTxInterval;
    int backoffType;
    double backoffBaseValue;
    double CSMApersistance;
    bool txAllPacketsInFreeChannel;
    bool sleepDuringBackoff;
    
    // Tham số cho RTS/CTS và ACK
    bool useRtsCts;
    int rtsThreshold;
    int rtsPacketSize;
    int ctsPacketSize;
    int ackPacketSize;
    int maxRetries;
    simtime_t ctsTimeout;

    int phyLayerOverhead;
    simtime_t phyDelayForValidCS;
    double phyDataRate;

    int macState;
    int backoffTimes;
    
    // Biến quản lý truyền lại và gói tin hiện tại
    int currentPacketRetries;
    int currentPacketDestination;

    // Cấu trúc dữ liệu và trạng thái cho EDCA (4 Access Categories)
    std::queue<cPacket*> TXBuffer_perAC[4];
    int AIFSN[4];
    int CWmin[4];
    int CWmax[4];
    int contentionWindow[4];
    int backoffCounter[4];
    bool contending[4];

 protected:
    void startup();
    void fromNetworkLayer(cPacket *, int);
    void fromRadioLayer(cPacket *, double, double);
    int handleControlCommand(cMessage * msg);
    void timerFiredCallback(int);
    void handleCarrierSenseResult(int);
    void attemptTx();
    void sendDataPacket();

    // Các hàm helper mới
    void startContention();
    void sendRTS();
    void sendCTS(int destination);
    void sendACK(int destination);
};

#endif //TUNABLEMACMODULE
