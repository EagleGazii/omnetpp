
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <omnetpp.h>

#define ROUND    new cMessage("round",    0) // Renk 0 = Kirmizi
#define COLOR    new cMessage("color",    1) // Renk 1 = Yesil
#define DISCARD    new cMessage("discard",    2) // Renk 2 = Mavi

using namespace omnetpp;

int nodeCount;
int *nodes_ID;
static std::vector<std::string> colorsArray { "red", "green", "blue", "white",
        "yellow", "pink", "grey" };

static std::vector<std::string> usedColor; // kullanilmis renkler dizisi

class Node: public cSimpleModule {

    bool round_over = false;
    bool round_recvd = false;
    bool colored = false; //her node icin bool tipinde renklenip renklenmedigini degisken
    std::vector<std::string> free_cols; //her node icin string tipinde vector listesi renklerin tutugu yerdir
    std::vector<int> curr_neighs; //her node icin tam sayi tipinde vector listesi komsularin tutugu yerdir
    std::vector<int> received; //her node icin tam sayi tipinde vector listesi komsulardan gelen mesajlari tutugu yerdir
    std::vector<int> lost_neighs; //her node icin tam sayi tipinde vector listesi kaybetigin komsulari tutugu yerdir
    void changeColor(cMessage *msg);

protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

Define_Module(Node);

void Node::initialize() {
    int i;

    nodeCount = getParentModule()->par("nodeCount"); //topolojinin node sayisi ned dosyasindan okunuyor
    int nMax = nodeCount, nMin = 0;
    int intRandom = nMin
            + (int) ((double) rand() / (RAND_MAX + 1) * (nMax - nMin + 1)); // 0 dan node sayisina kadar rastgele tam sayi uretiyor

    if (getId() == 2) {                 //nodes_ID - new_nodes_ID
        nodes_ID = new int[nodeCount]; //nodes_ID - topoloji uzerinde rastgele atanacak her node icin yeni id'ler dizisi,
                                       //kendi ID ler sirali olacagi icin ornege  en az renk kullanacagimizi saglamayacaktir,
                                       // cunku butun nodlar sirali initialize yapacagi icin kucukten buyuge siralacaktir.
        nodes_ID[getId() - 2] = intRandom; // Omnet++ node'larin id'leri = 2 den basliyor, (id = 1 network yapisina ayitir).
                                           //olusturdugumuz dizi ilk elemani rastgele sayini atiyoruz

    }

    for (i = 0; i < nodeCount; i++) {
        if (nodes_ID[i] == intRandom) { //her node initialize yapacagi icin olusturdugumuz dizi icinde daha once
                                        //atilmis id'ler bir daha ayni id atamazsin diye kontrol ediyor.
            intRandom = nMin
                    + (int) ((double) rand() / (RAND_MAX + 1)
                            * (nMax - nMin + 1)); //eger olusturdugumuz rastgele sayi daha once
                                                  // vardi o zaman yine rastgele sayi olsturuyor
            i = -1;
            continue;
        }

    }
    nodes_ID[getId() - 2] = intRandom; //kontrol yapisini gectikten sonra rastgele tam sayi eklenebilir.



    if (free_cols.empty()) {
        for (i = 0; i < colorsArray.size(); i++) { //global olusturdugumuz renk dizisinin her node icin local dizi icinde renkleri
            free_cols.push_back(colorsArray.at(i));     //atiyoruz
        }
    }
    if (curr_neighs.empty()) {
        for (i = 0; i < gateCount() / 2; i++) { //her node icin komsularin tuttugu komsular (kapilarin indislerini)
            curr_neighs.push_back(i);
        }
    }

    for (i = 0; i < gateCount() / 2; i++) { //butun node'lara ROUND mesaji ataniyor
        send(ROUND, "gate$o", i);
    }

}

void Node::handleMessage(cMessage *msg) {


    int i, max_ID; //  [olusturdugum rastegele id'lerin komsulardan en yuksek id'nin bulmak icin kullanilir]
    int my_Index = getId() - 2; // simdiki nodun indeksi [olusturdugum rastegele id'lerin bulmak icin kullanilacak]
    int neighbour_Index, neigbour_ID;
    int msgComingGate_ID = msg->getArrivalGate()->getIndex(); // mesajim geldigi kpi indeksi

    neigbour_ID =
            (this->gate("gate$o", msgComingGate_ID)->getNextGate()->getOwnerModule()->getId()); // benim mesajim geldigin node indisi
    neighbour_Index = neigbour_ID - 2; // benim mesajim geldigi nodun indeksi [olusturdugum rastegele id'lerin bulmak icin kullanilacak]

    if (strcmp(msg->getName(), "round") == 0) {  //round mesaj geldiyse
        changeColor(msg);


        round_recvd = true;
    } else if (strcmp(msg->getName(), "color") == 0) {
        cDisplayString &dispStr = getDisplayString();
        int colorIndex = msg->getKind();
        int control = 0;
        for (i = 0; i < usedColor.size(); i++) {

            if (usedColor.at(i) == colorsArray.at(colorIndex)) {

                control += 1;
                continue;
            }
        }
        if (control == 0) {
            usedColor.push_back(colorsArray.at(colorIndex));
        }

        int comingGate = msg->getArrivalGate()->getIndex();



        for (i = 0; i < free_cols.size(); i++) {
            if (free_cols.at(i) == colorsArray.at(colorIndex)) {
                free_cols.erase(free_cols.begin() + i);

            }
        }
        for (i = 0; i < curr_neighs.size(); i++) {
            if (curr_neighs.at(i) == comingGate) {
                curr_neighs.at(i) = -1;
                lost_neighs.push_back(i);
            }
        }
        if (!colored) {
            if (lost_neighs.size() == curr_neighs.size()) {
                EV << free_cols.at(0);
                colored = true;
                cDisplayString &dispStr = getDisplayString();
                std::string str = "i=device/pc;b=,,oval," + free_cols.at(0); //red
                const char *c = str.c_str();
                dispStr.parse(c);

            }

            int control = 0;
            for (i = 0; i < usedColor.size(); i++) {

                if (usedColor.at(i) == free_cols.at(0)) {

                    control += 1;
                    continue;
                }
            }
            if (control == 0) {
                usedColor.push_back(free_cols.at(0));
            }

        }

        changeColor(msg);

    }

    else if (strcmp(msg->getName(), "discard") == 0) {
        for (i = 0; i < curr_neighs.size(); i++) {
            if (curr_neighs.at(i) == -1) {
                continue;
            }
            received.push_back(i);
        }

    }

}
void Node::changeColor(cMessage *msg) {
    int i, max_ID; //  [olusturdugum rastegele id'lerin komsulardan en yuksek id'nin bulmak icin kullanilir]
    int my_Index = getId() - 2; // simdiki nodun indeksi [olusturdugum rastegele id'lerin bulmak icin kullanilacak]
    int neighbour_Index, neigbour_ID;
    int msgComingGate_ID = msg->getArrivalGate()->getIndex(); // mesajim geldigi kpi indeksi

    neigbour_ID =
            (this->gate("gate$o", msgComingGate_ID)->getNextGate()->getOwnerModule()->getId()); // benim mesajim geldigin node indisi
    neighbour_Index = neigbour_ID - 2;
    if (!colored) {         //eger node hala renklerndirilmemisse

        max_ID = nodes_ID[my_Index]; // bastan benim ID en yuksek olarak kabul ediyorum

        for (i = 0; i < curr_neighs.size(); i++) { //benim komsularim sayisina kadar donguyu dondur
            if (curr_neighs.at(i) == -1) { // herhangi bir komsu -1 ise demek ki artik o benim komsum sayilmaz
                continue;
            }
            neighbour_Index =
                    (this->gate("gate$o", i)->getNextGate()->getOwnerModule()->getId())
                            - 2; // butun kapilarimdan
            // bakarak en yuksek node_ID'ni bulmak isitiyorum
            if (max_ID < nodes_ID[neighbour_Index]) {
                max_ID = -1; // en yuksek id ben degilsem max_ID = -1 degisitirim dongu bittince

            }
        }
        if (max_ID != -1) { // eger max_ID degismemisse o zaman demek ki ben renklendirebilirim kenidimi
            colored = true;


            cDisplayString &dispStr = getDisplayString();
            std::string str = "i=device/pc;b=,,oval," + free_cols.at(0); //red
            const char *c = str.c_str();
            dispStr.parse(c);
            for (i = 0; i < colorsArray.size(); i++) {
                if (colorsArray.at(i) == free_cols.at(0)) { //local dizindeki rengi global dizindeki hangi indeks olursa onu komsularina gender
                    msg->setKind(i);
                }
            }

            for (i = 0; i < curr_neighs.size(); i++) {
                if (curr_neighs.at(i) == -1) { // komsularima color mesajini gonder
                    continue;
                }

                cMessage *newMsg = new cMessage("color");
                newMsg->setKind(msg->getKind());
                send(newMsg, "gate$o", i);

            }
        }

    } else {

         for (i = 0; i < curr_neighs.size(); i++) {
         if (curr_neighs.at(i) == -1) { // eger renklendirilmemissem komsularima discard mesajini gonder
         continue;
         }
         send(DISCARD, "gate$o", i);
         }
    }
}

void Node::finish() {
    int i;
    if (getId() == nodeCount - 1) {
        EV << "Butun renkler sirasiyla : ";
        for (i = 0; i < colorsArray.size(); i++) {
            EV << " " << colorsArray.at(i);
        }
        EV << " ; Toplam = " << i << " tane var.";

        EV << "\nKullanilmis renkler sirasiyla : ";
        for (i = 0; i < usedColor.size(); i++) {
            EV << "  " << usedColor.at(i);
        }
        EV << " ; Toplam = " << i << " tane kullanilmis.";
        EV << "\nToplam kullanilmamis renk sayisi = "
                  << colorsArray.size() - usedColor.size() << "\n";
    }
}

