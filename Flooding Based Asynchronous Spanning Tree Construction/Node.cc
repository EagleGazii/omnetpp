
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#define PROBE  new cMessage("probe",  2) // Renk 2 = Mavi
#define ACK    new cMessage("ack",    1) // Renk 1 = Yesil
#define REJECT new cMessage("reject", 0) // Renk 0 = Kirmizi

class Node: public cSimpleModule {

    int parent_gate;
    std::vector<cGate*> childs;
    std::vector<cGate*> others;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Node);


void Node::initialize() {

    parent_gate = -1;

    cDisplayString &dispStr = getDisplayString();
    dispStr.parse("i=device/pc;b=,,oval,#008000");

    if (strcmp(getFullName(), "node[0]") == 0) {

        parent_gate = 0;

        for (int i = 0; i < this->gateCount() / 2; i++) { // Her bir komsu icin
            send(PROBE, "gate$o", i);                  // Probe mesajlar gonder
        }

    }
}
void Node::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "probe") == 0) {
        cGate *arrival_gate = msg->getArrivalGate();
        cDisplayString &dispStr = getDisplayString();

        if (arrival_gate != NULL) {
            if (parent_gate == -1) {
                parent_gate = arrival_gate->getIndex();
                send(ACK, "gate$o", arrival_gate->getIndex());
                dispStr.parse("i=device/pc;b=,,oval,#FF2D00");
                for (int i = 0; i < this->gateCount() / 2; i++) {
                    if (msg->arrivedOn("gate$i", i)) {
                        continue;
                    }

                    send(PROBE, "gate$o", i);
                }
            } else {
                send(REJECT, "gate$o", arrival_gate->getIndex());

            }
        }

    } else if (strcmp(msg->getName(), "ack") == 0) {
        childs.push_back(msg->getArrivalGate());
        EV << this->getFullName() << "'s childs: ";
        for (int i = 0; i < childs.size(); i++) {
            EV << childs.at(i)->getPreviousGate()->getOwner()->getFullName()
                      << " , ";
        }
        EV << "\n";

    } else if (strcmp(msg->getName(), "reject") == 0) {
        others.push_back(msg->getArrivalGate());

    } else {

    }
    delete msg;
}
