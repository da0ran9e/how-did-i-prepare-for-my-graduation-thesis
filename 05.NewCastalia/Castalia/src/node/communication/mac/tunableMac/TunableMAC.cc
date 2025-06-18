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

Define_Module(TunableMAC);

void TunableMAC::startup()
{
	// <<< THAY ĐỔI: Loại bỏ việc đọc các tham số không cần thiết
	// dutyCycle = par("dutyCycle");
	// listenInterval = ((double)par("listenInterval")) / 1000.0;
	// beaconIntervalFraction = par("beaconIntervalFraction");
	// randomTxOffset = ((double)par("randomTxOffset")) / 1000.0;
	// beaconFrameSize = par("beaconFrameSize");
	
	probTx = par("probTx");
	numTx = par("numTx");
	reTxInterval = ((double)par("reTxInterval")) / 1000.0;
	backoffType = par("backoffType");
	CSMApersistance = par("CSMApersistance");
	txAllPacketsInFreeChannel = par("txAllPacketsInFreeChannel");
	sleepDuringBackoff = par("sleepDuringBackoff");

	phyDataRate = par("phyDataRate");
	phyDelayForValidCS = (double)par("phyDelayForValidCS") / 1000.0;
	phyLayerOverhead = par("phyFrameOverhead");

	// <<< THAY ĐỔI: Không cần tính beaconTxTime
	// beaconTxTime = ((double)(beaconFrameSize + phyLayerOverhead)) * 8.0 / (1000.0 * phyDataRate);

	switch (backoffType) {
		case 0:{ // <<< THAY ĐỔI: case này không còn hợp lệ vì dutyCycle không tồn tại, cần được xử lý
			trace() << "WARNING: backoffType 0 (BACKOFF_SLEEP_INT) is deprecated. Using BACKOFF_CONSTANT instead.";
			backoffType = BACKOFF_CONSTANT;
			break;
		}
		case 1:{
			backoffType = BACKOFF_CONSTANT;
			break;
		}
		case 2:{
			backoffType = BACKOFF_MULTIPLYING;
			break;
		}
		case 3:{
			backoffType = BACKOFF_EXPONENTIAL;
			break;
		}
		default:{
			opp_error("\n[TunableMAC]:\n Illegal value of parameter \"backoffType\" in omnetpp.ini.");
			break;
		}
	}
	backoffBaseValue = ((double)par("backoffBaseValue")) / 1000.0;

	// <<< THAY ĐỔI: Radio luôn ở trạng thái RX
	toRadioLayer(createRadioCommand(SET_STATE, RX));
	
	macState = MAC_STATE_DEFAULT;
	numTxTries = 0;
	backoffTimes = 0;

	declareOutput("TunableMAC packet breakdown");
}

void TunableMAC::timerFiredCallback(int timer)
{
	switch (timer) {
		// <<< THAY ĐỔI: Loại bỏ các case của timer không dùng nữa
		/*
		case START_SLEEPING:{
			...
			break;
		}

		case START_LISTENING:{
			...
			break;
		}
		*/

		case START_CARRIER_SENSING:{
			// Radio luôn ở trạng thái RX, không cần bật lại
			// toRadioLayer(createRadioCommand(SET_STATE, RX));
			handleCarrierSenseResult(radioModule->isChannelClear());
			break;
		}

		case ATTEMPT_TX:{
			attemptTx();
			break;
		}
		
		// <<< THAY ĐỔI: Đổi tên timer và hàm được gọi
		case SEND_DATA_PACKET:{
			sendDataPacket();
			break;
		}

		default:{
			trace() << "WARNING: unknown timer callback " << timer;
		}
	}
}

void TunableMAC::handleCarrierSenseResult(int returnCode)
{
	switch (returnCode) {

		case CLEAR:{
			if (CSMApersistance > 0 && CSMApersistance < genk_dblrand(1)){
				setTimer(START_CARRIER_SENSING, phyDelayForValidCS);
				break;
			}
			
			backoffTimes = 0;
			
			// <<< THAY ĐỔI: Không cần tính toán và gửi beacon nữa
			// remainingBeaconsToTx = 0;

			macState = MAC_STATE_TX;
			trace() << "Channel Clear, MAC_STATE_TX, sending data";
			sendDataPacket(); // Gọi hàm đã được đổi tên
			break;

		}

		case BUSY:{
			if (CSMApersistance > 0) {
				setTimer(START_CARRIER_SENSING, phyDelayForValidCS);
				trace() << "Channel busy, persistent mode: checking again in " << phyDelayForValidCS << " secs";
				break;
			}
			
			double backoffTimer = 0;
			backoffTimes++;

			switch (backoffType) {
				// ... (logic tính backoffTimer không đổi)
				case BACKOFF_CONSTANT:{
					backoffTimer = backoffBaseValue;
					break;
				}

				case BACKOFF_MULTIPLYING:{
					backoffTimer = (double)(backoffBaseValue * backoffTimes);
					break;
				}

				case BACKOFF_EXPONENTIAL:{
					backoffTimer = backoffBaseValue *
							pow(2.0, (double)(((backoffTimes - 1) < 0) ? 0 : backoffTimes - 1));
					break;
				}
			}

			backoffTimer = genk_dblrand(1) * backoffTimer;
			setTimer(START_CARRIER_SENSING, backoffTimer);
			trace() << "Channel busy, backing off for " << backoffTimer << " secs";

			// <<< THAY ĐỔI: Xóa logic chuyển sang chế độ ngủ khi backoff
			// if ((sleepDuringBackoff) || ((dutyCycle > 0.0) && (dutyCycle < 1.0)))
			// 	toRadioLayer(createRadioCommand(SET_STATE, SLEEP));
			break;
		}

		case CS_NOT_VALID:
		case CS_NOT_VALID_YET:{
			setTimer(START_CARRIER_SENSING, phyDelayForValidCS);
			trace() << "CS not valid yet, trying again.";
			break;
		}
	}
}

void TunableMAC::fromNetworkLayer(cPacket * netPkt, int destination)
{
	TunableMacPacket *macPkt = new TunableMacPacket("TunableMac data packet", MAC_LAYER_PACKET);
	encapsulatePacket(macPkt, netPkt);
	macPkt->setSource(SELF_MAC_ADDRESS);
	macPkt->setDestination(destination);
	macPkt->setFrameType(DATA_FRAME);
	collectOutput("TunableMAC packet breakdown", "Received from App");

	if (bufferPacket(macPkt)) {
		if (macState == MAC_STATE_DEFAULT && TXBuffer.size() == 1) {
			// <<< THAY ĐỔI: Không cần hủy các timer của duty cycle
			// if ((dutyCycle > 0.0) && (dutyCycle < 1.0)) {
			// 	cancelTimer(START_LISTENING);
			// 	cancelTimer(START_SLEEPING);
			// }
			numTxTries = numTx;
			attemptTx();
		}
	} else {
		collectOutput("TunableMAC packet breakdown", "Overflown");
		trace() << "WARNING Tunable MAC buffer overflow";
	}
}

void TunableMAC::attemptTx()
{
	trace() << "attemptTx(), buffer size: " << TXBuffer.size() << ", numTxTries: " << numTxTries;

	if (numTxTries <= 0) {
		if (TXBuffer.size() > 0) {
			cancelAndDelete(TXBuffer.front());
			TXBuffer.pop();
		}

		if (TXBuffer.size() > 0) {
			numTxTries = numTx;
			attemptTx();
		} else {
			macState = MAC_STATE_DEFAULT;
			trace() << "MAC_STATE_DEFAULT, no more pkts to attemptTx";
			// <<< THAY ĐỔI: Không đặt lại timer ngủ
			// if ((dutyCycle > 0.0) && (dutyCycle < 1.0))
			// 	setTimer(START_SLEEPING, 0);
		}
		return;
	}

	macState = MAC_STATE_CONTENDING;

	if (genk_dblrand(0) < probTx) {
		// <<< THAY ĐỔI: Loại bỏ randomTxOffset, bắt đầu cảm nhận kênh ngay lập tức
		setTimer(START_CARRIER_SENSING, 0);
		trace() << "MAC_STATE_CONTENDING, attempt " << numTx - numTxTries +1 << "/" << numTx << " contending";
	} else {
		setTimer(ATTEMPT_TX, reTxInterval);
		trace() << "MAC_STATE_CONTENDING, attempt " << numTx - numTxTries +1 << "/" << numTx << " skipped";
		numTxTries--;
	}
}

// <<< THAY ĐỔI: Đổi tên hàm và loại bỏ logic beacon
void TunableMAC::sendDataPacket()
{
	// <<< THAY ĐỔI: Logic beacon đã bị xóa hoàn toàn.
	if (TXBuffer.empty()) {
		numTxTries = 0; // safeguarding
		attemptTx(); 	// Quay về trạng thái mặc định nếu không còn gì để gửi
		return;
	}

	toRadioLayer(TXBuffer.front()->dup());
	toRadioLayer(createRadioCommand(SET_STATE, TX));

	if (numTxTries == numTx){
		trace() << "Sending data packet";
		collectOutput("TunableMAC packet breakdown", "sent data pkts");
	}
	else{
		trace() << "Sending copy of data packet";
		collectOutput("TunableMAC packet breakdown", "copies of sent data pkts");
	}

	double packetTxTime = ((double)(TXBuffer.front()->getByteLength() +
		  phyLayerOverhead)) * 8.0 / (1000.0 * phyDataRate);
	numTxTries--;

	if (txAllPacketsInFreeChannel){
		setTimer(SEND_DATA_PACKET, packetTxTime * 0.95); // <<< THAY ĐỔI: Đổi tên timer
		if (numTxTries <= 0){
			cancelAndDelete(TXBuffer.front());
			TXBuffer.pop();
			numTxTries = numTx;
		}
		return;
	}
	
	if (numTxTries > 0) {
		setTimer(ATTEMPT_TX, packetTxTime + reTxInterval);
	} else {
		setTimer(ATTEMPT_TX, packetTxTime);
	}
}

void TunableMAC::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	TunableMacPacket *macFrame = dynamic_cast <TunableMacPacket*>(pkt);
	if (macFrame == NULL){
		collectOutput("TunableMAC packet breakdown", "filtered, other MAC");
		return;
	}

	int destination = macFrame->getDestination();
	if (destination != SELF_MAC_ADDRESS && destination != BROADCAST_MAC_ADDRESS){
		collectOutput("TunableMAC packet breakdown", "filtered, other dest");
		return;
	}

	// <<< THAY ĐỔI: Xóa logic xử lý BEACON_FRAME
	switch (macFrame->getFrameType()) {
		case DATA_FRAME:{
			toNetworkLayer(decapsulatePacket(macFrame));
			collectOutput("TunableMAC packet breakdown", "received data pkts");
			// Sau khi nhận xong, nếu đang rảnh thì kiểm tra buffer để gửi gói tiếp theo
			if (macState == MAC_STATE_DEFAULT) {
				attemptTx();
			}
			break;
		}
		default:{
			trace() << "WARNING: Received packet type UNKNOWN";
			collectOutput("TunableMAC packet breakdown", "unrecognized type");
		}
	}
}

int TunableMAC::handleControlCommand(cMessage * msg)
{
	TunableMacControlCommand *cmd = check_and_cast <TunableMacControlCommand*>(msg);

	// <<< THAY ĐỔI: Loại bỏ các case không còn phù hợp
	switch (cmd->getTunableMacCommandKind()) {
		/* Các case sau cần được xóa đi:
		 * SET_DUTY_CYCLE
		 * SET_LISTEN_INTERVAL
		 * SET_BEACON_INTERVAL_FRACTION
		 * SET_RANDOM_TX_OFFSET
		*/
		case SET_PROB_TX:{

			double tmpValue = cmd->getParameter();
			if ((tmpValue < 0.0) || (tmpValue > 1.0))
				trace() << "WARNING: invalid ProbTX value sent to TunableMac";
			else
				probTx = tmpValue;
			break;
		}

		case SET_NUM_TX:{

			double tmpValue = cmd->getParameter();
			if (tmpValue < 0 || tmpValue - ceil(tmpValue) != 0)
				trace() << "WARNING: invalid NumTX value sent to TunableMac";
			else
				numTx = (int)tmpValue;
			break;
		}

		case SET_RANDOM_TX_OFFSET:{

			double tmpValue = cmd->getParameter() / 1000.0;
			if (tmpValue <= 0.0)
				trace() << "WARNING: invalid randomTxOffset value sent to TunableMac";
			else
				randomTxOffset = tmpValue;
			break;
		}

		case SET_RETX_INTERVAL:{

			double tmpValue = cmd->getParameter() / 1000.0;
			if (tmpValue <= 0.0)
				trace() << "WARNING: invalid reTxInterval value sent to TunableMac";
			else
				reTxInterval = tmpValue;
			break;
		}

		case SET_BACKOFF_TYPE:{

			double tmpValue = cmd->getParameter();

			if (tmpValue == 0.0) {
				if ((dutyCycle > 0.0) && (dutyCycle < 1.0))
					backoffType = BACKOFF_SLEEP_INT;
				else
					trace() << "WARNING: invalid backoffType value sent to TunableMac. Backoff timer = sleeping interval, but sleeping interval is not defined because duty cycle is zero, one, or invalid.";
			}

			else if (tmpValue == 1.0) {
				if (backoffBaseValue <= 0.0)
					trace() << "WARNING: unable to set backoffType. Parameter backoffBaseValue has conflicting value";
				else
					backoffType = BACKOFF_CONSTANT;
			}

			else if (tmpValue == 2.0) {
				if (backoffBaseValue <= 0.0)
					trace() << "WARNING: unable to set backoffType. Parameter backoffBaseValue has conflicting value";
				else
					backoffType = BACKOFF_MULTIPLYING;
			}

			else if (tmpValue == 3.0) {
				if (backoffBaseValue <= 0.0)
					trace() << "WARNING: unable to set backoffType. Parameter backoffBaseValue has conflicting value";
				else
					backoffType = BACKOFF_EXPONENTIAL;
			}

			else
				trace() << "WARNING: invalid backoffType value sent to TunableMac";

			break;
		}

		case SET_BACKOFF_BASE_VALUE:{

			double tmpValue = cmd->getParameter() / 1000.0;
			if (tmpValue < 0)
				trace() << "WARNING: invalid backoffBaseValue sent to TunableMac";
			else
				backoffBaseValue = tmpValue;
			break;
		}
	}

	delete cmd;
	return 1;
}
