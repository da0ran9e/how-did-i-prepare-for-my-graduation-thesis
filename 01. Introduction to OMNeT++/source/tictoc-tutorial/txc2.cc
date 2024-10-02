#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc2 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc2);

void Txc2::initialize()
{
    if (strcmp("tic", getName()) == 0) {
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc2::handleMessage(cMessage *msg)
{
    send(msg, "out"); // send out the message
}
