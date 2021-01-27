#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;


#define NEXT    new cMessage("Sonraki_Node",    1)  // Renk 1 = Yesil
#define BACK    new cMessage("parent_node_geri_git",    0)    // Renk 0 = Kirmizi

double *count;      // butun yollarin maliyetini tutan dizidir (global)
bool *notVisited;    // butun nodlari tutan dizidir algoritma #gezdigi node ise false #  gezmedigi node ise true (global)

int nodeCount;    // agin topolojisinin node sayisi
int root_id;      //root nodu


class Node: public cSimpleModule {
    int parent_id = -1;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

Define_Module(Node);

void Node::initialize() {

    int starting_node = 3;      //starting node mesela node[0] ;
    root_id = starting_node + 2 ;   //network'ta nodun indisi 2'den baslaliyor                                                                                       //o yuzden basladigimiz nodun sayisi + 2 yaparak  ,// gercek nodun idisi ulasmis oluruz
    double min = 0.5, max = 3, doubleRandom;
    nodeCount = getParentModule()->par("nodeCount"); // ned dosyadan nodeCount degiskeni okuyarak agda kac node oldugunu ogrenir
    int i;
    for (i = 0; i < this->gateCount() / 2; i++) {
        doubleRandom = (max - min) * ((double) rand() / (double) RAND_MAX);
        check_and_cast<cDelayChannel*>(gate("gate$o", i)->getChannel())->setDelay(
                doubleRandom);

    }

    if (getId() == root_id) {      //gelen node id root ile esit ise if yapisina gir
        parent_id = -1;
        count = new double[nodeCount];  // node syisina gore maliyetini tutan dizi olusturuyoruz
        notVisited = new bool[nodeCount];   // node syisina gore ziyaret edecegimiz nodelar tutan dizi olusturuyoruz
        for (i = 0; i < nodeCount; i++) {
            count[i] = 9999;       // butun nodlara buyuk bir sayi atiyoruz sonsuz gibi
            notVisited[i] = true;  // butun gezilmemis nodlara true atiyoruz
        }
        int my_Index = starting_node;  // ayni anda programda ihtiyac duyulan her nodun
                                         //olustudugumuz diziler icin idisini bilmemiz lazim
                                          // ve indeksi ise = baslandigimiz nodu ile esit.
        count[my_Index] = 0;               // baslangic noduna 0 maliyetini atiyoruz
        notVisited[my_Index] = false;         // baslangic nodunu ile islem yapacagimiz icin visited dizi icinde false yaptik

        for (i = 0; i < this->gateCount() / 2; i++) {   //baslangic nodunun komsularini kontrol ediyoruz en az maliyetli youlunu seciyoruz
            double delay = check_and_cast<cDelayChannel*>(
                    gate("gate$o", i)->getChannel())->getDelay().dbl();
            int neighbour_index = (this->gate("gate$o", i)->getNextGate()->getOwnerModule()->getId()) - 2;
            double toplama = count[my_Index] + delay;

            if (toplama < count[neighbour_index]) {  // her komsu icin en kis yolu gunceliyoruz
                count[neighbour_index] = toplama;
            }
        }

        int sonraki_node = -1;
        double current_min_delay = 9999;
        for (i = 0; i < this->gateCount() / 2; i++) {

            int neighbour_index = (this->gate("gate$o", i)->getNextGate()->getOwnerModule()->getId()) - 2;

            if (count[neighbour_index] <= current_min_delay) {  // en kisa olan komsu secip mesaj gonderiyoruz.
                current_min_delay = count[neighbour_index];
                sonraki_node = i;

            }
        }

        send(NEXT, "gate$o", sonraki_node);

    }

}

void Node::handleMessage(cMessage *msg) {
    int i;
    int my_index = getId() - 2; //my_index - my_index degistirelim daha iyi oluyor
    notVisited[my_index] = false;
    if (strcmp(msg->getName(), "Sonraki_Node") == 0) {
        parent_id = msg->getArrivalGate()->getIndex();
    }

    for (i = 0; i < this->gateCount() / 2; i++) {  //baslangic nodunun komsularini kontrol ediyoruz en az maliyetli youlunu seciyoruz
        double delay = check_and_cast<cDelayChannel*>(gate("gate$o", i)->getChannel())->getDelay().dbl();
        int neighbour_index = (this->gate("gate$o", i)->getNextGate()->getOwnerModule()->getId())  - 2;
        if (notVisited[neighbour_index] == false) {
            continue;
        }
        double toplama = count[my_index] + delay;

        if (toplama < count[neighbour_index]) { // her komsu icin en kisa yolu gunceliyoruz

            count[neighbour_index] = toplama;
        }
    }

    int sonraki_node = -1;      //kucuk maliyetli olan arayip seciyoruz
    double current_min_delay = 9999;
    for (i = 0; i < this->gateCount() / 2; i++) {
        int neighbour_index =
                (this->gate("gate$o", i)->getNextGate()->getOwnerModule()->getId())
                        - 2;
        if (notVisited[neighbour_index] == false) {     //parent'e geri donmesin diye
            continue;
        }


        if (count[neighbour_index] <= current_min_delay) {  // en kisa olan komsu secip mesaj gonderiyoruz.
            current_min_delay = count[neighbour_index];
            sonraki_node = i;

        }

    }
    if (sonraki_node == -1) {
        if(getId() == root_id){
            return;
        }
        send(BACK, "gate$o", parent_id);
    } else {
        send(NEXT, "gate$o", sonraki_node);
    }

}

void Node::finish() {
    //

    int root_index = root_id - 2;
    if (getId() == root_id) {
        int i;

        for (i = 0; i < nodeCount; i++) {
            if(i == root_index){
                continue;
            }
            EV << "node[" << root_index << "] - node[" << i << "] - " << count[i]
                      << " saniye\n";
        }

    }

}

