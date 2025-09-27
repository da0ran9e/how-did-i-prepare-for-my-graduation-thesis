#include "node/communication/routing/gstebRouting/GSTEBRouting.h"

Define_Module(GSTEBRouting);

void GSTEBRouting::startup()
{
// A

	// CH bắt đầu boardcast thông tinstart_time, slot_length, N (số node).
	
	// Mỗi node i tính EL_i = floor(ResidEnergy_i, E_unit) hoặc một hàm tỉ lệ

	// Neighbor discovery (time-slot per node): trong slot của node i, node i broadcast gói chứa {ID=i, coords, EL}; các node nhận lưu vào Table I (neighbors). 

	// Exchange neighbors-of-neighbors

// B

	// BS thu/chuẩn bị dữ liệu đầu vào

	// EL1 cho mỗi node (EL1 = EL mà BS tính ra / dự đoán).

	// BS xây routing tree

	// chọn các relay node sao cho nodes with larger EL trở thành relay nhiều hơn

	// năng lượng tiêu thụ để node i → relay r (E_tx k,d) cộng năng lượng r → BS (hoặc r → parent tiếp theo)

	// BS schedule TDMA (một slot cho từng “level” / leaf) và gói điều khiển

	// BS gửi CTRL_PKT tuần tự

// C
// Node i bật radio vào slot của nó (do BS chỉ định) và nhận CTRL_PKT: trích xuất parent_ID, my_slot_tx, my_slot_rx, expected_pkt_length (dự tính tổng dữ liệu phải truyền/nhận). 

// Lưu parent/children: node ghi parent = parent_ID; thêm children nếu BS cung cấp hoặc tự tính từ TableI/II bằng cách đối chiếu parent của neighbors (nếu BS không gửi danh sách children đầy đủ).

// Chuẩn bị năng lượng: node dựa trên expected_pkt_length tính xem nó có đủ năng lượng để thực hiện slot đó; nếu năng lượng hiện tại khác biệt đáng kể so với EL1 (BS tính), node set error_flag để gửi EL2 trong DATA_PKT. 

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
	GSTEBRoutingPacket *netPacket = new GSTEBRoutingPacket("GSTEBRouting packet", NETWORK_LAYER_PACKET);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	netPacket->setDestination(destination);
	encapsulatePacket(netPacket, pkt);
	toMacLayer(netPacket, resolveNetworkAddress(destination));
}

void GSTEBRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
	RoutingPacket *netPacket = dynamic_cast <RoutingPacket*>(pkt);
	if (netPacket) {
		string destination(netPacket->getDestination());
		if (destination.compare(SELF_NETWORK_ADDRESS) == 0 ||
		    destination.compare(BROADCAST_NETWORK_ADDRESS) == 0)
			toApplicationLayer(decapsulatePacket(pkt));
	}
}

void GSTEBRouting::timerFiredCallback(int index)
{
	// ch chọn node ngẫu nhiên làm root,  thông báo. ưu tiên node có EL cao

	// mỗi node (i) tự chọn cha ưu tiên node có El cao (r)

	// 

}

void GSTEBRouting::beacon()
{
	// 
}

void 