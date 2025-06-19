/***************************************************************************
 * Copyright: National ICT Australia,  2007 - 2012                        *
 * Developed at the ATP lab, Networked Systems theme                      *
 * Author(s): Athanassios Boulis, Yuriy Tselishchev                       *
 * This file is distributed under the terms in the attached LICENSE file. *
 * If you do not find this file, copies can be found by writing to:       *
 * *
 * NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia            *
 * Attention:  License Inquiry.                                       *
 ***************************************************************************/

#include "TunableMAC.h"
#include <vector>
#include <cmath> // Cần cho hàm pow()

Define_Module(TunableMAC);

void TunableMAC::startup()
{
    // --- Đọc các tham số cơ bản ---
    probTx = par("probTx");
    reTxInterval = ((double)par("reTxInterval")) / 1000.0;
    backoffType = par("backoffType");
    CSMApersistance = par("CSMApersistance");
    txAllPacketsInFreeChannel = par("txAllPacketsInFreeChannel");
    sleepDuringBackoff = par("sleepDuringBackoff");

    phyDataRate = par("phyDataRate");
    phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0;
    phyLayerOverhead = par("phyFrameOverhead");

    // --- Đọc các tham số cho ACK và RTS/CTS ---
    useRtsCts = par("useRtsCts");
    rtsThreshold = par("rtsThreshold");
    rtsPacketSize = par("rtsPacketSize");
    ctsPacketSize = par("ctsPacketSize");
    ackPacketSize = par("ackPacketSize");
    maxRetries = par("maxRetries");
    ctsTimeout = ((double)par("ctsTimeout")) / 1000.0;

    // --- <<< SỬA LỖI: Đọc các tham số EDCA từ chuỗi bằng cStringTokenizer và asVector() ---
    cStringTokenizer aifsnTokenizer(par("AIFSN").stringValue());
    std::vector<std::string> aifsnVector = aifsnTokenizer.asVector();

    cStringTokenizer cwMinTokenizer(par("CWmin").stringValue());
    std::vector<std::string> cwMinVector = cwMinTokenizer.asVector();

    cStringTokenizer cwMaxTokenizer(par("CWmax").stringValue());
    std::vector<std::string> cwMaxVector = cwMaxTokenizer.asVector();

    // Kiểm tra số lượng tham số
    if (aifsnVector.size() != 4 || cwMinVector.size() != 4 || cwMaxVector.size() != 4) {
        // Sử dụng opp_error để báo lỗi và dừng mô phỏng
        throw cRuntimeError("TunableMAC EDCA parameters (AIFSN, CWmin, CWmax) must have exactly 4 integer values each, separated by spaces.");
    }

    // Gán giá trị vào các mảng
    for (int i = 0; i < 4; i++) {
        AIFSN[i] = atoi(aifsnVector[i].c_str());
        CWmin[i] = atoi(cwMinVector[i].c_str());
        CWmax[i] = atoi(cwMaxVector[i].c_str());
        contentionWindow[i] = CWmin[i]; // Khởi tạo CW bằng CWmin
        backoffCounter[i] = 0;
        contending[i] = false;
    }

    backoffBaseValue = ((double)par("backoffBaseValue")) / 1000.0;

    toRadioLayer(createRadioCommand(SET_STATE, RX));

    macState = MAC_STATE_DEFAULT;
    backoffTimes = 0;
    currentPacketRetries = 0;
    currentPacketDestination = -1;

    declareOutput("TunableMAC packet breakdown");
}

void TunableMAC::fromNetworkLayer(cPacket * netPkt, int destination)
{
    TunableMacPacket *macPkt = new TunableMacPacket("TunableMac data packet", MAC_LAYER_PACKET);
    encapsulatePacket(macPkt, netPkt);
    macPkt->setSource(SELF_MAC_ADDRESS);
    macPkt->setDestination(destination);
    macPkt->setFrameType(DATA_FRAME);

    // Xác định độ ưu tiên từ gói tin của tầng Application
    // Giả định tầng Application có tham số 'priority'
    int priority = 0;
    if (macPkt->getEncapsulatedPacket()->hasPar("priority")) {
        priority = macPkt->getEncapsulatedPacket()->par("priority");
    }
    if (priority < 0 || priority > 3) {
        priority = 0; // Mặc định là AC_BE (Best Effort) nếu không có hoặc giá trị không hợp lệ
    }

    // <<< SỬA LỖI: Tự hiện thực logic buffer thay vì gọi hàm bufferPacket cũ
    if ((int)TXBuffer_perAC[priority].size() >= macBufferSize) {
        // Hàng đợi của AC này đã đầy
        collectOutput("TunableMAC packet breakdown", "Overflown", priority);
        trace() << "WARNING: Tunable MAC buffer overflow for AC " << priority << ". Packet dropped.";
        cancelAndDelete(macPkt); // Hủy gói tin

        // Gửi thông báo buffer đầy lên tầng trên (tùy chọn)
        MacControlMessage *fullBuffMsg =
            new MacControlMessage("MAC buffer full", MAC_CONTROL_MESSAGE);
        fullBuffMsg->setMacControlMessageKind(MAC_BUFFER_FULL);
        send(fullBuffMsg, "toNetworkModule");

    } else {
        // Buffer còn chỗ, thêm gói tin vào hàng đợi
        TXBuffer_perAC[priority].push(macPkt);
        trace() << "Packet buffered for AC " << priority << ", buffer state: "
            << TXBuffer_perAC[priority].size() << "/" << macBufferSize;
        collectOutput("TunableMAC packet breakdown", "Received from App");

        // Nếu MAC đang rảnh, bắt đầu quá trình tranh chấp
        if (macState == MAC_STATE_DEFAULT) {
            startContention();
        }
    }
}

// <<< THÊM MỚI: Bắt đầu quá trình tranh chấp kênh cho các AC có dữ liệu
void TunableMAC::startContention() {
    if (macState != MAC_STATE_DEFAULT) return;

    // Quét các AC từ ưu tiên cao đến thấp
    for (int ac = 3; ac >= 0; ac--) {
        if (!TXBuffer_perAC[ac].empty()) {
            // Tìm thấy gói tin ở AC có ưu tiên cao nhất
            trace() << "Found packet in AC " << ac << ". Starting contention process.";
            contending[ac] = true;
            // Tính toán AIFS delay
            simtime_t aifs_delay = AIFSN[ac] * 0.00001; // Giả định aSlotTime = 10us
            
            // Lấy một số backoff ngẫu nhiên
            backoffCounter[ac] = intrand(contentionWindow[ac]);
            trace() << "AC " << ac << ": AIFS wait " << aifs_delay << "s, backoff counter set to " << backoffCounter[ac];

            // Đặt timer để bắt đầu cảm nhận kênh sau khi chờ AIFS
            // Lưu ý: Đây là một cách hiện thực đơn giản hóa. 
            // EDCA đầy đủ phức tạp hơn với việc giảm backoff đồng thời.
            setTimer(ATTEMPT_TX, aifs_delay);
            return; // Chỉ xử lý AC ưu tiên cao nhất tại một thời điểm
        }
    }
}

// <<< THAY ĐỔI: Hàm này giờ chỉ chịu trách nhiệm quyết định và bắt đầu cảm nhận kênh
void TunableMAC::attemptTx()
{
	// Logic tranh chấp phức tạp hơn giờ nằm trong startContention() và timerFiredCallback
    // Hàm này được gọi sau khi chờ AIFS và sẵn sàng giảm backoff/cảm nhận kênh.
    macState = MAC_STATE_CONTENDING;
    setTimer(START_CARRIER_SENSING, 0); // Bắt đầu cảm nhận kênh ngay lập tức
}


void TunableMAC::timerFiredCallback(int timer)
{
	switch (timer) {
		case START_CARRIER_SENSING: {
			handleCarrierSenseResult(radioModule->isChannelClear());
			break;
		}

		case ATTEMPT_TX: {
			attemptTx();
			break;
		}
		
		case SEND_DATA_PACKET: {
			sendDataPacket();
			break;
		}

        // <<< THÊM MỚI: Xử lý timeout cho CTS và ACK
        case CTS_TIMEOUT: {
            trace() << "CTS Timeout for packet to " << currentPacketDestination;
            currentPacketRetries++;
            if (currentPacketRetries < maxRetries) {
                macState = MAC_STATE_DEFAULT;
                startContention(); // Thử lại từ đầu
            } else {
                trace() << "Packet to " << currentPacketDestination << " failed after max retries.";
                collectOutput("TunableMAC packet breakdown", "Fail, No CTS");
                // Xóa gói tin
                // ... (cần xác định AC của gói tin để xóa đúng)
                macState = MAC_STATE_DEFAULT;
                startContention();
            }
            break;
        }

        case ACK_TIMEOUT: {
            trace() << "ACK Timeout for packet to " << currentPacketDestination;
            currentPacketRetries++;
            // ... (Logic tương tự như CTS_TIMEOUT)
            break;
        }

		default:{
			trace() << "WARNING: unknown timer callback " << timer;
		}
	}
}

void TunableMAC::handleCarrierSenseResult(int returnCode)
{
    // Tìm AC đang tranh chấp (trong phiên bản đơn giản hóa, chỉ có 1)
    int currentAC = -1;
    for(int i=0; i<4; i++) {
        if(contending[i]) {
            currentAC = i;
            break;
        }
    }
    if (currentAC == -1) return; // Không có AC nào đang tranh chấp

	switch (returnCode) {
		case CLEAR: {
            // Giảm bộ đếm backoff
            if (backoffCounter[currentAC] > 0) {
                backoffCounter[currentAC]--;
                trace() << "Channel clear, AC " << currentAC << " backoff now at " << backoffCounter[currentAC];
                setTimer(START_CARRIER_SENSING, 0.00001); // Chờ 1 slot time
                return;
            }

            // Backoff đã xong, tiến hành gửi gói
            TunableMacPacket *pktToSend = (TunableMacPacket *)TXBuffer_perAC[currentAC].front();

            if (pktToSend->getDestination() != BROADCAST_MAC_ADDRESS && useRtsCts && pktToSend->getByteLength() > rtsThreshold) {
                sendRTS();
            } else {
                sendDataPacket();
            }
            break;
		}

		case BUSY: {
			trace() << "Channel busy for AC " << currentAC;
            // Tăng cửa sổ tranh chấp
            contentionWindow[currentAC] = min(contentionWindow[currentAC] * 2, CWmax[currentAC]);
            // Bắt đầu lại quá trình tranh chấp
            contending[currentAC] = false;
            macState = MAC_STATE_DEFAULT;
            startContention();
			break;
		}

		case CS_NOT_VALID:
		case CS_NOT_VALID_YET: {
			setTimer(START_CARRIER_SENSING, phyDelayForValidCS);
			trace() << "CS not valid yet, trying again.";
			break;
		}
	}
}

void TunableMAC::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	TunableMacPacket *macFrame = dynamic_cast <TunableMacPacket*>(pkt);
	if (macFrame == NULL) return;

	int destination = macFrame->getDestination();
    int source = macFrame->getSource();

	if (destination != SELF_MAC_ADDRESS && destination != BROADCAST_MAC_ADDRESS) {
        // NAV - Network Allocation Vector (Đơn giản hóa)
        // Nếu nghe được RTS/CTS không phải cho mình, cần im lặng
        if (macFrame->getFrameType() == RTS_FRAME || macFrame->getFrameType() == CTS_FRAME) {
            //... logic xử lý NAV
        }
		return;
	}

	switch (macFrame->getFrameType()) {
		case DATA_FRAME: {
			toNetworkLayer(decapsulatePacket(macFrame));
			collectOutput("TunableMAC packet breakdown", "received data pkts");

            if (destination == SELF_MAC_ADDRESS) {
                sendACK(source);
            }
			break;
		}
        // <<< THÊM MỚI: Xử lý các gói điều khiển
        case RTS_FRAME: {
            if (macState == MAC_STATE_DEFAULT || macState == MAC_STATE_RX) {
                trace() << "RTS received from " << source << ". Replying with CTS.";
                // Gửi CTS
                // ...
            }
            break;
        }
        case CTS_FRAME: {
            if (macState == MAC_STATE_WAIT_FOR_CTS && source == currentPacketDestination) {
                trace() << "CTS received from " << source << ". Sending DATA.";
                cancelTimer(CTS_TIMEOUT);
                sendDataPacket();
            }
            break;
        }
        case ACK_FRAME: {
            // ... (Logic xử lý ACK như đã thảo luận)
            break;
        }
		default: {
			trace() << "WARNING: Received packet type UNKNOWN";
		}
	}
}

/**
 * handleControlCommand - Xử lý các lệnh điều khiển từ tầng trên hoặc các module khác.
 *
 * Hàm này được gọi khi MAC nhận được một tin nhắn có kind là MAC_CONTROL_COMMAND.
 * Nó cho phép thay đổi các tham số của MAC một cách linh động trong lúc mô phỏng đang chạy.
 */
int TunableMAC::handleControlCommand(cMessage * msg)
{
    TunableMacControlCommand *cmd = check_and_cast <TunableMacControlCommand*>(msg);

    switch (cmd->getTunableMacCommandKind()) {

        case SET_PROB_TX: {
            double tmpValue = cmd->getParameter();
            if ((tmpValue < 0.0) || (tmpValue > 1.0))
                trace() << "WARNING: invalid ProbTX value sent to TunableMac";
            else
                probTx = tmpValue;
            break;
        }

        case SET_NUM_TX: {
            int tmpValue = (int)cmd->getParameter();
            if (tmpValue < 0)
                trace() << "WARNING: invalid NumTX value sent to TunableMac";
            else
                numTx = tmpValue;
            break;
        }

        case SET_RETX_INTERVAL: {
            double tmpValue = cmd->getParameter() / 1000.0;
            if (tmpValue <= 0.0)
                trace() << "WARNING: invalid reTxInterval value sent to TunableMac";
            else
                reTxInterval = tmpValue;
            break;
        }

        case SET_BACKOFF_TYPE: {
            int tmpValue = (int)cmd->getParameter();
            if (tmpValue >= BACKOFF_CONSTANT && tmpValue <= BACKOFF_EXPONENTIAL) {
                if (backoffBaseValue <= 0.0)
                    trace() << "WARNING: unable to set backoffType. Parameter backoffBaseValue has conflicting value";
                else
                    backoffType = tmpValue;
            } else {
                trace() << "WARNING: invalid backoffType value sent to TunableMac";
            }
            break;
        }

        case SET_BACKOFF_BASE_VALUE: {
            double tmpValue = cmd->getParameter() / 1000.0;
            if (tmpValue < 0)
                trace() << "WARNING: invalid backoffBaseValue sent to TunableMac";
            else
                backoffBaseValue = tmpValue;
            break;
        }

        // <<< THÊM MỚI: Xử lý các lệnh điều khiển không còn được hỗ trợ
        case SET_DUTY_CYCLE:
        case SET_LISTEN_INTERVAL:
        case SET_BEACON_INTERVAL_FRACTION:
        case SET_RANDOM_TX_OFFSET: {
            trace() << "WARNING: Control command for a deprecated parameter received. Command ignored.";
            break;
        }

        default: {
            trace() << "WARNING: Unknown control command received.";
            break;
        }
    }

    delete cmd;
    return 1;
}

// Gửi gói tin Request-To-Send (RTS)
void TunableMAC::sendRTS() {
    // Giả định rằng gói tin DATA đã nằm ở đầu hàng đợi của AC đang tranh chấp
    int currentAC = -1;
    for(int i = 3; i >= 0; i--) {
        if(contending[i] && !TXBuffer_perAC[i].empty()) {
            currentAC = i;
            break;
        }
    }
    if (currentAC == -1) {
        trace() << "ERROR: sendRTS called but no AC is contending or buffer is empty.";
        return;
    }

    TunableMacPacket* dataPkt = (TunableMacPacket *)TXBuffer_perAC[currentAC].front();
    currentPacketDestination = dataPkt->getDestination();

    trace() << "Sending RTS to " << currentPacketDestination;

    // Tạo gói RTS
    TunableMacPacket *rtsPkt = new TunableMacPacket("TunableMAC RTS", MAC_LAYER_PACKET);
    rtsPkt->setFrameType(RTS_FRAME);
    rtsPkt->setSource(SELF_MAC_ADDRESS);
    rtsPkt->setDestination(currentPacketDestination);
    rtsPkt->setByteLength(rtsPacketSize);

    // Gửi gói tin và chuyển radio sang trạng thái TX
    toRadioLayer(rtsPkt);
    toRadioLayer(createRadioCommand(SET_STATE, TX));
    collectOutput("TunableMAC packet breakdown", "sent RTS");

    // Chuyển sang trạng thái chờ CTS và đặt timeout
    macState = MAC_STATE_WAIT_FOR_CTS;
    // Thời gian chờ CTS = SIFS + Thời gian truyền CTS + 2*trễ lan truyền
    // Ở đây ta dùng giá trị cấu hình được từ file .ini
    setTimer(CTS_TIMEOUT, ctsTimeout);
}

// Gửi gói tin Clear-To-Send (CTS)
void TunableMAC::sendCTS(int destination) {
    trace() << "Sending CTS to " << destination;

    // Tạo gói CTS
    TunableMacPacket *ctsPkt = new TunableMacPacket("TunableMAC CTS", MAC_LAYER_PACKET);
    ctsPkt->setFrameType(CTS_FRAME);
    ctsPkt->setSource(SELF_MAC_ADDRESS);
    ctsPkt->setDestination(destination);
    ctsPkt->setByteLength(ctsPacketSize);

    // Gửi ngay lập tức (thường là sau một khoảng SIFS, được xử lý bởi logic nhận RTS)
    toRadioLayer(ctsPkt);
    toRadioLayer(createRadioCommand(SET_STATE, TX));
    collectOutput("TunableMAC packet breakdown", "sent CTS");
    
    // Sau khi gửi CTS, MAC sẽ ở trạng thái mặc định và chờ nhận DATA
    macState = MAC_STATE_DEFAULT;
}


// Gửi gói tin Acknowledgement (ACK)
void TunableMAC::sendACK(int destination) {
    trace() << "Sending ACK to " << destination;

    // Tạo gói ACK
    TunableMacPacket *ackPkt = new TunableMacPacket("TunableMAC ACK", MAC_LAYER_PACKET);
    ackPkt->setFrameType(ACK_FRAME);
    ackPkt->setSource(SELF_MAC_ADDRESS);
    ackPkt->setDestination(destination);
    ackPkt->setByteLength(ackPacketSize);

    // Gửi ngay lập tức (thường là sau một khoảng SIFS, được xử lý bởi logic nhận DATA)
    toRadioLayer(ackPkt);
    toRadioLayer(createRadioCommand(SET_STATE, TX));
    collectOutput("TunableMAC packet breakdown", "sent ACK");

    // Sau khi gửi ACK, MAC sẽ ở trạng thái mặc định
    macState = MAC_STATE_DEFAULT;
}


// Gửi gói tin DATA (đã được sửa đổi để hỗ trợ cả Unicast và Broadcast)
void TunableMAC::sendDataPacket() {
    int currentAC = -1;
    for(int i = 3; i >= 0; i--) {
        if(contending[i] && !TXBuffer_perAC[i].empty()) {
            currentAC = i;
            break;
        }
    }
    if (currentAC == -1) {
        trace() << "ERROR: sendDataPacket called but no AC is contending or buffer is empty.";
        macState = MAC_STATE_DEFAULT;
        startContention();
        return;
    }
    
    TunableMacPacket* pktToSend = (TunableMacPacket *)TXBuffer_perAC[currentAC].front();
    currentPacketDestination = pktToSend->getDestination();
    
    // Gửi một bản sao của gói tin
	toRadioLayer(pktToSend->dup());
	toRadioLayer(createRadioCommand(SET_STATE, TX));

    // Đặt lại trạng thái tranh chấp cho AC này
    contending[currentAC] = false;

	if (currentPacketRetries == 0){
		trace() << "Sending DATA packet to " << currentPacketDestination;
		collectOutput("TunableMAC packet breakdown", "sent data pkts");
	} else {
		trace() << "Re-transmitting DATA packet to " << currentPacketDestination 
                << " (Attempt " << currentPacketRetries + 1 << "/" << maxRetries << ")";
		collectOutput("TunableMAC packet breakdown", "re-sent data pkts");
	}

    // Tính toán thời gian truyền gói tin
    simtime_t packetTxTime = ((double)(pktToSend->getByteLength() + phyLayerOverhead)) * 8.0 / (phyDataRate * 1000.0);

    if (currentPacketDestination != BROADCAST_MAC_ADDRESS) {
        // Gói UNICAST, cần chờ ACK
        macState = MAC_STATE_WAIT_FOR_ACK;
        // Thời gian chờ ACK = SIFS + Thời gian truyền ACK + 2*trễ lan truyền
        simtime_t ackWaitDuration = packetTxTime + 0.0001; // Giả định SIFS+ACK_TX_Time là 100us, cần tính chính xác hơn
        setTimer(ACK_TIMEOUT, ackWaitDuration);
        
    } else {
        // Gói BROADCAST, không chờ ACK
        // Xóa gói khỏi buffer và bắt đầu tranh chấp cho gói tiếp theo
        collectOutput("TunableMAC packet breakdown", "Success, Broadcast");
        cancelAndDelete(TXBuffer_perAC[currentAC].front());
        TXBuffer_perAC[currentAC].pop();
        currentPacketRetries = 0; // Reset cho gói mới
        
        // Sau khi truyền xong, chuyển về trạng thái mặc định và bắt đầu tranh chấp mới
        macState = MAC_STATE_DEFAULT;
        // Đặt một timer nhỏ để đảm bảo radio đã chuyển về RX trước khi bắt đầu tranh chấp mới
        setTimer(ATTEMPT_TX, packetTxTime); 
    }
}
