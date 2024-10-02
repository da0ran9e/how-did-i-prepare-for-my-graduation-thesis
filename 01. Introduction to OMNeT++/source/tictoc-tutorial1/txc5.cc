#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc5 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc5);

void Txc5::initialize()
{
    if (strcmp("tic", getName()) == 0) {
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc5::handleMessage(cMessage *msg)
{
    send(msg, "out");
}

