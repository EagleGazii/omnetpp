/*
 * neighDFS.cc
 *
 *  Created on: Dec 30, 2019
 *      Author: Gazmor
 */


#include <string.h>
#include <omnetpp.h>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "token_m.h"
#include <typeinfo>


using namespace omnetpp;


class Node : public cSimpleModule
{
    int possibleParent;
    int myParentId = -1;
    int parentUniqe ;
    std::string parentName;
    std::string arrivalGateName;
    std::vector<int> visited;
    std::string edge = "";
    std::string kenar = "";
    int myChildId = -1;
    std::string nodeName;


    int getRandomNumber(int starting,int finishing);

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Node);

void Node::initialize()
{
    //EV << "ParentModule : "<< getParentModule()->getIndex() << "\n";

    if(strcmp(getFullName(), "node[0]") == 0) {
        Token *token = new Token("Token");
        int nodeNumber = getParentModule()->par("nodeCount");
        token->setVisitedArraySize(nodeNumber);
        for(int i =0;i<nodeNumber;i++){
            token->setVisited(i,0);
        }
        cDisplayString& dispStr = getDisplayString();
        dispStr.parse("i=device/pc;b=,,oval,#008000");
        parentName = "root";

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
        parentUniqe = -1;
        token->setVisited(0, gate("gate$o",myChildId)->getOwnerModule()->getId());
        //token->setVisited(1, gate("gate$o",myChildId)->getNextGate()->getOwnerModule()->getId());
        //EV << "\nSenderGate : "<< gate("gate$o",myChildId)->getNextGate()->getOwnerModule()->getFullName();
        send(token, "gate$o", myChildId);

    }
}


void Node::handleMessage(cMessage *msg)
{
    Token *token = check_and_cast<Token *>(msg);

    possibleParent = token->getArrivalGate()->getIndex();
    arrivalGateName = token->getArrivalGate()->getPreviousGate()->getOwner()->getFullName();
    int myNodeId = token->getArrivalGate()->getOwnerModule()->getId();



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




    if(strcmp(token->getName(), "Token") == 0){
        cDisplayString& dispStr = getDisplayString();
        if(parentName != arrivalGateName){
            int i = 0;
            for(i = 0;i<visited.size();i++){
                int j = 0;
                if(visited[i]==1){
                    continue;
                }
                if(visited.size() == 1){
                    dispStr.parse("i=device/pc;b=,,oval,red");
                    send(token,"gate$o",myParentId);
                    break;
                }
                for(j = 0;j<token->getVisitedArraySize();j++){
                    if(token->getVisited(j) == gate("gate$o",i)->getNextGate()->getOwnerModule()->getId()){
                        visited[i] = 1;
                        goto down;
                    }

                }
                if(j == token->getVisitedArraySize() && i != myParentId){
                    visited[i] = 1;
                    myChildId = i;
                    dispStr.parse("i=device/pc;b=,,oval,#008000");
                    send(token,"gate$o",myChildId);
                    break;
                 }else if(i == myParentId){
                     visited[i] = 1;
                 }
                down:
                if(true){

                }
            }
            if(i == visited.size()){
                if(parentUniqe == -1){
                    dispStr.parse("i=device/pc;b=,,oval,red");

                }else{
                dispStr.parse("i=device/pc;b=,,oval,red");
                send(token,"gate$o",myParentId);

                }

            }



        }else if(parentName == arrivalGateName){
            for(int i = 0;i<token->getVisitedArraySize();i++){
                if(token->getVisited(i) == 0){
                    token->setVisited(i, myNodeId);
                    break;
                }
            }


            int i = 0;
            for(i = 0;i<visited.size();i++){
                int j = 0;
                if(visited[i]==1){

                    continue;
                }
                if(visited.size() == 1){
                    dispStr.parse("i=device/pc;b=,,oval,red");
                    send(token,"gate$o",myParentId);
                    break;
                }
                for(j = 0;j<token->getVisitedArraySize();j++){
                    if(token->getVisited(j) == gate("gate$o",i)->getNextGate()->getOwnerModule()->getId()){
                        visited[i] = 1;

                        goto down2;
                    }

                }

                if(j == token->getVisitedArraySize() && i != myParentId){

                    visited[i] = 1;
                    myChildId = i;
                    dispStr.parse("i=device/pc;b=,,oval,#008000");
                    send(token,"gate$o",myChildId);
                    break;

                }

                else if(i == myParentId){
                    visited[i] = 1;
                }

                down2:
                if(i == visited.size()-1){
                    dispStr.parse("i=device/pc;b=,,oval,red");
                    send(token,"gate$o",myParentId);
                    continue;
                }
            }

        }

            for(int i =0;i<token->getVisitedArraySize();i++){
                EV << token->getVisited(i) << "\t";
            }
            EV << "\n";




        }

    }






int Node::getRandomNumber(int starting,int finishing){
    srand(time(NULL));
    int random_number = starting + rand() % finishing;
    return random_number;
}




