/*
 * Node.cc
 *
 *  Created on: Dec 18, 2019
 *      Author: Gazmor
 */

#include <string.h>
#include <omnetpp.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

using namespace omnetpp;


class Node : public cSimpleModule
{
    int possibleParent;
    int myParentId = -1;
    std::string parentName;
    std::string arrivalGateName;
    std::vector<bool> visited;
    std::string edge = "";
    std::string kenar = "";
    int myChildId = -1;


    int getRandomNumber(int starting,int finishing);

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Node);

void Node::initialize()
{
   // EV << "ParentModule : "<< getParentModule()->getIndex() << "\n";

    if(strcmp(getFullName(), "node[0]") == 0) {
        int number = getParentModule()->par("nodeCount");
        EV << "nodeCount : " << number << "\n";
        cDisplayString& dispStr = getDisplayString();
        dispStr.parse("i=device/pc;b=,,oval,#008000");
        parentName = "root";
        cMessage *msg = new cMessage("Token");
        int n = gateSize("gate$o");
        if(visited.empty()){
            for(int i = 0; i < n; i++){
                visited.push_back(0);
            }
        }
        if(myChildId == -1){
            myChildId = getRandomNumber(0,n);
        }
        EV << "MyChildId : " << myChildId << " - parentName : " <<parentName << "\n";
        visited[myChildId] = 1;

        send(msg, "gate$o", myChildId);

    }
}


void Node::handleMessage(cMessage *msg)
{
   // EV << "ParentModule : " << msg->getOwner()->getFullName() <<"\n";

    //EV << "GetArrivalModuleName - " << msg->getArrivalModule()->getFullName();
    possibleParent = msg->getArrivalGate()->getIndex();
    arrivalGateName = msg->getArrivalGate()->getPreviousGate()->getOwner()->getFullName();
    //EV << "ArrivalGate : " << arrival_gate->getPreviousGate()->getOwner()->getFullName() << "\n";

    if(parentName == "root"){
        myParentId = -1;
    }
    else if(myParentId == -1){
        myParentId = possibleParent;
        parentName = arrivalGateName;

    }


    int n = gateSize("gate$o");


    if(visited.empty()){
        for(int i = 0; i < n; i++){
            visited.push_back(0);
        }
    }



    if(strcmp(msg->getName(), "Token") == 0){
        cDisplayString& dispStr = getDisplayString();
        if(parentName == arrivalGateName){
            while(true){
                int number = getRandomNumber(0,n);
                if(myParentId == number){
                    if(visited.size() == 1){
                        send(msg, "gate$o",myParentId);
                        dispStr.parse("i=device/pc;b=,,oval,red");
                        break;
                    }
                    continue;
                }
                else if(myChildId == -1){
                    myChildId = number;
                }
                visited[myChildId] = 1;
                dispStr.parse("i=device/pc;b=,,oval,#008000");
                send(msg, "gate$o",myChildId);
                 break;
            }


        }else if(!msg->arrivedOn(myChildId)){

            if(visited[possibleParent] == 0){

                EV << "{" << getFullName() <<"," << msg->getArrivalGate()->getPreviousGate()->getOwner()->getFullName() <<"}\n";
                visited[msg->getArrivalGate()->getIndex()] = 1;   //root icin ve digerler icin (o kapidan hala mesaj atmamis ama baska
                dispStr.parse("i=device/pc;b=,,oval,#008000");
                send(msg, "gate$o",possibleParent); //kapilardan mesaj atmistir o yuzden edge olur
            }else{

                int j = visited.size()-1;
                for(int i = 0;i<visited.size();i++){
                if(i == myParentId and visited[i] == 0){
                   if(j == i){
                       dispStr.parse("i=device/pc;b=,,oval,red");
                       send(msg, "gate$o",myParentId);
                       break;
                   }
                   continue;
                }else if(visited[i] == 1){
                    if(j == i){
                        if(parentName == "root"){
                            dispStr.parse("i=device/pc;b=,,oval,red");
                            delete msg;
                        }
                        else{
                            dispStr.parse("i=device/pc;b=,,oval,red");
                            send(msg, "gate$o",myParentId);
                            break;
                        }

                    }
                    continue;
                }else{
                visited[i] = 1;
                myChildId = i;
                dispStr.parse("i=device/pc;b=,,oval,#008000");
                send(msg, "gate$o",i);
                break;
                }


            }
            }


        }
    }

}

int Node::getRandomNumber(int starting,int finishing){
    srand(time(NULL));
    int random_number = starting + rand() % finishing;
    return random_number;
}





