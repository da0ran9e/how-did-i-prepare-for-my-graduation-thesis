#include "node/communication/routing/cellularRouting/CellularRouting.h"

Define_Module(CellularRouting);

void CellularRouting::startup()
{
    // Initialize parameters: coordinates, communication interval, routing table, etc.
    helloInterval = par("helloInterval");
    cellRadius = par("cellRadius");

    myCL_id = -1;
    myCH_id = -1;

    cModule *parentNode = getParentModule();
    myX = parentNode->getParentModule()->par("xCoor");
    myY = parentNode->getParentModule()->par("yCoor");
    bool isSink = par("isSink");

    if (isSink) {
        amI_CH = true;
        myCH_id = self;
        myRole = CLUSTER_HEAD;
    }
    myRole = amI_CH ? CLUSTER_HEAD : NORMAL_NODE;
    myCellId = -1;
    myColor = -1;
    myNextHopId = -1;
    neighborTable.clear();

    calculateCellInfo();
    grid_offset = par("gridOffset");

    trace() << "[EVENT] Node " << self << " initialized at (" << myX << ", " << myY
            << "). Cell ID: " << myCellId << ", Color: " << myColor
            << ", Is CH: " << (amI_CH ? "Yes" : "No");

    // Set initial timer for STATE_0
    // Set timer for STATE_1
}

void CellularRouting::timerFiredCallback(int index)
{
    // case STATE_0:
        // Start the SEND_HELLO_TIMER
        // Start the CL_ELECTION_TIMER
        // Start the CL_CONFIRMATION_TIMER
        // Start the GATEWAY_SELECTION_TIMER
        // Start the ROUTING_TREE_CALCULATION_TIMER
        // Start the ROUTING_TABLE_ANNOUNCEMENT_TIMER
        // Start the FINALIZE_ROUTING_TIMER

    // case SEND_HELLO_TIMER:
        // sendHelloPacket();

    // case CL_ELECTION_TIMER:
        // calculateFitnessScore();
        // start CL_ADVERTISING_TIMER base on fitness score

    // case CL_ADVERTISING_TIMER:
        // sendCLAnnouncement();

    // case CL_CONFIRMATION_TIMER:
        // if not CL: sendCLConfirmationPacket();

    // case GATEWAY_SELECTION_TIMER:
        // if CL: gatewaySelection();

    // case ROUTING_TREE_CALCULATION_TIMER:
        // if CL: calculateRoutingTree();

    // case ROUTING_TABLE_ANNOUNCEMENT_TIMER:
        // if CL: announceRoutingTable();

    // case FINALIZE_ROUTING_TIMER:
        // if CL: finalizeRouting();

    // case STAGE_1:
        // start the CH_ADVERTISING_TIMER
        // start the CH_ELECTION_TIMER

    // case CH_ADVERTISING_TIMER:
        // sendCHAnnouncement();

    // case CH_ELECTION_TIMER:
        // if CL: sellectClusterHead();
}

void CellularRouting::fromApplicationLayer(cPacket * pkt, const char *destination)
{

}

void CellularRouting::fromMacLayer(cPacket * pkt, int srcMacAddress, double rssi, double lqi)
{
    // case HELLO_PACKET:
        // handleHelloPacket();

    // case CL_ANNOUNCEMENT:
        // handleCLAnnouncementPacket();

    // case CL_CONFIRMATION:
        // handleCLConfirmationPacket();

    // case GATEWAY_SELECTION:
        // handleGatewaySelectionPacket();

    // case LINK_REQUEST:
        // handleLinkRequestPacket();

    // case LINK_CONFIRMATION:
        // handleLinkConfirmationPacket();

    // case ROUTING_TABLE_ANNOUNCEMENT:
        // handleRoutingTableAnnouncementPacket();

    // case FINALIZE_ROUTING:
        //
}

void CellularRouting::calculateCellInfo()
{
    double frac_q = (sqrt(3.0)/3.0 * myX - 1.0/3.0 * myY) / cellRadius;
    double frac_r = (2.0/3.0 * myY) / cellRadius;
    double frac_s = -frac_q - frac_r;

    int q = round(frac_q);
    int r = round(frac_r);
    int s = round(frac_s);

    double q_diff = abs(q - frac_q);
    double r_diff = abs(r - frac_r);
    double s_diff = abs(s - frac_s);

    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    } else {
        s = -q - r;
    }

    const int grid_offset = 100;
    myCellId = q + r * grid_offset;
    myColor = ((q - r) % 3 + 3) % 3;
}

void CellularRouting::sendHelloPacket()
{
    // Broadcast hello packet with my coordinates and cell ID
}

void CellularRouting::handleHelloPacket(CellularRoutingPacket* pkt)
{
    // Save the information from the hello packet to neighbor table
}

void CellularRouting::calculateFitnessScore()
{
    // Calculate fitness score based on id, energy, distance, etc.
    // Or just let the application layer handle it
}

void CellularRouting::sendCLAnnouncement()
{
    // send CL announcement packet to neighbors with my fitness score
}

void CellularRouting::handleCLAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // make comparisons with my best fitness score
    // If I have a worse score, cancel my CL_ADVERTISING_TIMER if running
    // Forward the packet to other cell members
}

void CellularRouting::sendCLConfirmationPacket()
{
    // Send confirmation packet to the best candidate CL with my information
}

void CellularRouting::handleCLConfirmationPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save the confirmation and update my cell members
    // If I am not the CL, forward the confirmation to my CL
}

void CellularRouting::gatewaySelection()
{
    // Calculate the best gateway candidates based on distance
    // Or just let the application layer handle it
    // sendGatewaySelectionPacket(); after a random delay
}

void CellularRouting::sendGatewaySelectionPacket()
{
    // Send gateway selection packet to the CGW
}

void CellularRouting::handleGatewaySelectionPacket(CellularRoutingPacket* pkt)
{
    // sendLinkRequestPacket()
}

void CellularRouting::sendLinkRequestPacket()
{
    // Send link request packet to the best candidate NGW
}

void CellularRouting::handleLinkRequestPacket(CellularRoutingPacket* pkt)
{
    // If I am the NGW, forward the request to my CL
    // If I am the CL, sendLinkConfirmationPacket()
}

void CellularRouting::sendLinkConfirmationPacket(CellularRoutingPacket* pkt)
{
    // Send link confirmation packet to the NGW
}

void CellularRouting::handleLinkConfirmationPacket(CellularRoutingPacket* pkt)
{
    // If I am the NGW, update my state and forward the confirmation to my CGW
    // If I am the CGW, forward the confirmation to my CL
    // If I am the CL, update my routing table
}

void CellularRouting::calculateRoutingTree()
{
    // Calculate the intra-cell routing trees with CGWs and CL are roots
    // all members should have a routing table entry to every CGW and CL
    // Or just let the application layer handle it
}

void CellularRouting::announceRoutingTable()
{
    // for each cell member, send a routing table update packet
    // sendRoutingTableAnnouncementPacket();
}

void CellularRouting::sendRoutingTableAnnouncementPacket()
{
    // Create and send a routing table announcement packet
}

void CellularRouting::handleRoutingTableAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If for me: Update my routing table based on the received announcement
    // If not for me: Forward the packet to the destination if in range
}

void CellularRouting::finalizeRouting()
{
    // Node has completed routing setup
    // Send a final confirmation to CL
}

void CellularRouting::sendCHAnnouncement()
{
    // If I am the CH, send a CH announcement packet CL
}

void CellularRouting::handleCHAnnouncementPacket(CellularRoutingPacket* pkt)
{
    // If I am the CL, save my cell information to packet metadata
        // and forward the CH announcement to my cell members with destination as all neighbor cells

    // If I am a cell member,
        // if the destination is our cell, forward the packet to my CL
        // if the destination is not our cell, forward the packet to CGW

    // If I am a CGW,
        // if the destination is our cell, forward the packet to my CL
        // if the destination is not our cell, forward the packet to my NGW
}

void CellularRouting::selectClusterHead()
{
    // select the closest CH
    // Or just let the application layer handle it
}
