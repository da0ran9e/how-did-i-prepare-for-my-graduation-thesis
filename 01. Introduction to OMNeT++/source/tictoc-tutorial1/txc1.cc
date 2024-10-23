 #include <string.h>
 #include <omnetpp.h>

 using namespace omnetpp;

 bool I = false;

 class Txc1 : public cSimpleModule
 {
   protected:
     virtual void initialize() override;
     virtual void handleMessage(cMessage *msg) override;
   private:
           void createNode(const std::string& nodeName, int depth);
           int maxDepth;
 };

 Define_Module(Txc1);

 void Txc1::initialize()
 {
     if (strcmp("tic", getName()) == 0) {
         cMessage *msg = new cMessage("tictocMsg");
         send(msg, "out");
     }
     createNode("node1", 0);
 }

 void Txc1::handleMessage(cMessage *msg)
 {
     send(msg, "out");
 }

 void Txc1::createNode(const std::string& nodeName, int depth)
 {
     if (depth >= maxDepth) return; // Base case to stop recursion

     // Create a node dynamically
     cModuleType *nodeType = cModuleType::get("Txc1");
     cModule *node = nodeType->create(nodeName.c_str(), this);

     // Set up the node
     node->finalizeParameters();
     node->buildInside();
     node->callInitialize();
     //node->scheduleStart(simTime() + 3);

     // Recursively create the next node
     std::string nextNodeName = nodeName + std::to_string(depth + 1);
     createNode(nextNodeName, depth + 1);

//     // Optionally connect gates between nodes
//     cGate *outGate = node->gate("out");
//     //cGate *nextInGate = node->getParentModule()->getModuleByRelativePath(nextNodeName.c_str())->gate("in");
//     outGate->connectTo(cModuleType::get("Txc1")->gate("out"));
 }

