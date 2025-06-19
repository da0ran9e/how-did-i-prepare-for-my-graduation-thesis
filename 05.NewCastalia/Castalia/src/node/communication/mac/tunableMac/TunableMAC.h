/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef TUNABLEMACMODULE
#define TUNABLEMACMODULE

#include "node/communication/mac/VirtualMac.h"
#include "node/communication/mac/tunableMac/TunableMacControl_m.h"
#include "node/communication/mac/tunableMac/TunableMacPacket_m.h"

using namespace std;

// Giữ nguyên enum này
enum backoffTypes {
	BACKOFF_CONSTANT = 2201,
	BACKOFF_MULTIPLYING = 2202,
	BACKOFF_EXPONENTIAL = 2203
};

// <<< THAY ĐỔI: Mở rộng MacStates để quản lý RTS/CTS và ACK
enum MacStates {
	MAC_STATE_DEFAULT = 1,      // Trạng thái mặc định, sẵn sàng
	MAC_STATE_TX = 2,           // Đang truyền gói tin vật lý
	MAC_STATE_CONTENDING = 3,   // Đang trong quá trình tranh chấp kênh (backoff)
	MAC_STATE_RX = 4,           // Đã nhận gói tin và đang xử lý
	MAC_STATE_WAIT_FOR_CTS = 5, // Đã gửi RTS, đang đợi CTS
	MAC_STATE_WAIT_FOR_ACK = 6  // Đã gửi DATA, đang đợi ACK
};

// <<< THAY ĐỔI: Mở rộng TunableMacTimers để quản lý timeout
enum TunableMacTimers {
	START_CARRIER_SENSING = 3,
	ATTEMPT_TX = 4,
	SEND_DATA_PACKET = 5,       // Đã đổi tên từ SEND_BEACONS_OR_DATA
    CTS_TIMEOUT = 6,            // Timer cho việc chờ CTS
    ACK_TIMEOUT = 7             // Timer cho việc chờ ACK
};

class TunableMAC: public VirtualMac {
 private:
	/*--- Tham số từ file .ned ---*/
	// Các tham số liên quan đến duty cycle và beacon đã được xóa
	double probTx;
	int numTx;
	double reTxInterval;
	int backoffType;
	double backoffBaseValue;
	double CSMApersistance;
	bool txAllPacketsInFreeChannel;
	bool sleepDuringBackoff;
    
    // <<< THÊM MỚI: Các tham số cho RTS/CTS và ACK
    bool useRtsCts;
    int rtsThreshold;
    int ctsPacketSize;
    int ackPacketSize;
    int maxRetries;
    simtime_t ctsTimeout; // Thời gian chờ CTS

	int phyLayerOverhead;
	simtime_t phyDelayForValidCS;
	double phyDataRate;

	int macState;
	int backoffTimes;
    
    // <<< THAY ĐỔI: Biến quản lý truyền lại được thiết kế lại
    int currentPacketRetries; // Số lần thử lại còn lại cho gói tin HIỆN TẠI

    // <<< THÊM MỚI: Cấu trúc dữ liệu cho EDCA (4 Access Categories)
    std::queue<cPacket*> TXBuffer_perAC[4];
    int AIFSN[4];
    int CWmin[4];
    int CWmax[4];
    int contentionWindow[4];   // Giá trị CW hiện tại cho mỗi AC
    int backoffCounter[4];     // Bộ đếm backoff cho mỗi AC

 protected:
	void startup();
	void fromNetworkLayer(cPacket *, int);
	void fromRadioLayer(cPacket *, double, double);
	int handleControlCommand(cMessage * msg);
	void timerFiredCallback(int);
	void handleCarrierSenseResult(int);
	void attemptTx();
	
    // <<< THAY ĐỔI: Tên hàm đã được cập nhật
	void sendDataPacket();

    // <<< THÊM MỚI: Các hàm helper cho logic mới
    void startContention();
    void sendRTS();
    void sendACK(int destination);

};

#endif //TUNABLEMACMODULE