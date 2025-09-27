#include "node/communication/routing/gstebRouting/GSTEBRouting.h"

Define_Module(GSTEBRouting);

void GSTEBRouting::startup()
{
	if (isSink) {
		setTimer(BS_BROADCAST, 1);
		setTimer(PHRASE_B, 2000);
	}





//D
	//TDMA theo schedule của BS: BS đã phân slot; trong mỗi slot, node biết khi nào nhận (as parent) và khi nào gửi (as child).

	// DATA_PKT kích thước: bài giả định DATA_PKT = 2000 bits (để mô phỏng); 

	// Parent như relay: parent kế thừa dữ liệu các children (không fuse)

	// Node báo EL2 (nếu khác EL1): nếu node phát hiện EL2 ≠ EL1, node gắn error_flag và kèm EL2 vào DATA_PKT; khi BS nhận, BS cập nhật EL1 cho round tiếp theo.

//E 
// 	EL update: BS dùng EL2 nhận từ DATA_PKT để điều chỉnh topology cho round sau. 

// Node sắp chết / broadcast (Case1 khác Case2): bài mô tả trong Case1 nodes có cơ chế báo dying broadcast; trong Case2 BS điều khiển nhiều hơn, nhưng nếu node chết giữa round, children sẽ detect parent unreachable và có 2 lựa chọn: (a) tạm chọn neighbor khác làm parent theo Table I/II (nếu có), hoặc (b) gửi trực tiếp tới BS (nếu đủ năng lượng). Bài khuyến nghị BS tái cấu trúc ở round tiếp theo.
}

void GSTEBRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void GSTEBRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{

}

void GSTEBRouting::timerFiredCallback(int index)
{
	switch (index) {
	case BS_BROADCAST: {
		bsBroadcast();
		break;
	}

	case PHRASE_A: {
		calculateEnergyLevels();
		setTimer(NEIGHBOR_DISCOVERY, uniform(0, 200));
		setTimer(NEIGHBOR_EXCHANGE, uniform(500, 900));
		setTimer(ACK_BS, 1000);
		break;
	}
	
	case NEIGHBOR_DISCOVERY: {
		// neighbor discovery
		neighborDiscovery();
		break;
	}
	case NEIGHBOR_EXCHANGE: {
		// exchange neighbors
		exchangeNeighbors();
		break;
	}

	case SCHEDULE_RX: {
		setTimer(SCHEDULE_RX, 2000);
		// Node i bật radio vào slot của nó (do BS chỉ định) và nhận CTRL_PKT: trích xuất parent_ID, my_slot_tx, my_slot_rx, expected_pkt_length (dự tính tổng dữ liệu phải truyền/nhận). 
		break;
	}

	case ACK_BS: {
		scheduleRx();
		sendACKToBS();
		break;
	}

	case PHRASE_B: {
		calculateScheduling();

	// BS gửi CTRL_PKT tuần tự
		setTimer(BS_SEND_CTRL, 500);

		break;
	}

	case BS_SEND_CTRL: {
		sendBSBroadcast();
		break;
	}

	default:
		break;
	}
}

void GSTEBRouting::bsBroadcast()
{
	// BS broadcast thông tin start_time, slot_length, N (số node).
}

void GSTEBRouting::sendBSBroadcast()
{
	// BS broadcast thông tin start_time, slot_length, N (số node).
}

void GSTEBRouting::handleBSBroadcast()
{
	// Nhận thông tin start_time, slot_length, N (số node).
	setTimer(PHRASE_I, start_time + self * slot_length);
}

void GSTEBRouting::calculateEnergyLevels()
{
	// Mỗi node i tính EL_i = floor(ResidEnergy_i, E_unit) hoặc một hàm tỉ lệ
	
}

void GSTEBRouting::neighborDiscovery()
{
	// Neighbor discovery (time-slot per node): trong slot của node i, node i broadcast gói chứa {ID=i, coords, EL}; các node nhận lưu vào Table I (neighbors). 

}

void GSTEBRouting::handleNeighborDiscovery()
{
	// Nhận gói từ neighbor, lưu vào Table I (neighbors)
}

void GSTEBRouting::exchangeNeighbors()
{
	// Exchange neighbors-of-neighbors
	// Mỗi node i gửi danh sách hàng xóm của nó đến BS
}

void GSTEBRouting::handleNeighborExchange()
{
	
}

void GSTEBRouting::scheduleRx()
{
	setTimer(SCHEDULE_RX, 2000);
}

void GSTEBRouting::sendACKToBS()
{
	// Gửi ACK đến BS
	// Gói ACK chứa {ID=i, EL_i, neighbors-of-neighbors}
}

void GSTEBRouting::handleACKFromNode()
{
	// Nếu là BS: BS thu/chuẩn bị dữ liệu đầu vào
	// Nếu không phải BS: gửi đến node có neighbor là BS 
						// nếu không có thì broadcast
}

void GSTEBRouting::calculateScheduling()
{
	// Tính EL1 cho mỗi node (EL1 = EL mà BS tính ra / dự đoán).

	// BS xây routing tree

	// chọn các relay node sao cho nodes with larger EL trở thành relay nhiều hơn

	// năng lượng tiêu thụ để node i → relay r (E_tx k,d) cộng năng lượng r → BS (hoặc r → parent tiếp theo)

	// BS schedule TDMA (một slot cho từng “level” / leaf) và gói điều khiển
}

void GSTEBRouting::handleBSControlPacket()
{
	// Nhận gói CTRL_PKT: trích xuất parent_ID, my_slot_tx, my_slot_rx, expected_pkt_length (dự tính tổng dữ liệu phải truyền/nhận). 

	// Lưu parent/children: node ghi parent = parent_ID; thêm children nếu BS cung cấp hoặc tự tính từ TableI/II bằng cách đối chiếu parent của neighbors (nếu BS không gửi danh sách children đầy đủ).

	// Chuẩn bị năng lượng: node dựa trên expected_pkt_length tính xem nó có đủ năng lượng để thực hiện slot đó; nếu năng lượng hiện tại khác biệt đáng kể so với EL1 (BS tính), node set error_flag để gửi EL2 trong DATA_PKT. 
}