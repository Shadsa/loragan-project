#include <Sodaq_RN2483.h>
#include "factory.h"

// GPS Point Manipulation function 
void IntToPoint(byte point[], float value){

    float integ;
    float frac = modff(value, &integ);
    int i = floor(abs(integ));
    int f = floor(abs(frac)*100);

    byte _i = lowByte(i);
    *point = _i << 1;
    if(value < 0){
        bitWrite(*point, 0, 1);
    } else {
        bitWrite(*point, 0, 0);
    }

    byte _f = lowByte(f);
    *(point+1) = _f << 1;
    bitWrite(*(point+1), 0, bitRead(_i, 7));
}

float PointToInt(byte point[]){
    SerialUSB.println();
   
    bool sign = bitRead(*point, 0);

    byte i = *point >> 1;
    byte f = *(point+1);
    //byte f = *(point+1) & 127; // not working - need to set bit manually
    bitWrite(i, 7, bitRead(f, 0));
  
    float nv = (float)i;
    nv += ((f >> 1) * 0.01);
    if(sign){
        return -nv;
    }

    return nv;
}

/*ROUTING TABLE MANAGEMENT*/
boolean DropGateway(LPGANNetwork *network, Gateway *gw, RoutingTable *table){
    if(gw == nullptr || table == nullptr)
        return false;
    boolean found = false;
    int i=0;
    while(i<sizeof(table->Gateways[network->ID]) && !found){
        if(table->Gateways[network->ID][i] != nullptr && table->Gateways[network->ID][i]->GatewayEUI == gw->GatewayEUI){
            table->Gateways[network->ID][i] = nullptr;
            found = true;
        }
        i++;           
    }

    return true;
}
boolean IsGatewayDropped(Gateway *gw){
    if(gw->DropRatio<0.5)
        return true;
    return false;
}

boolean ParseACK(Message *m, int acks[2]){
    acks[0] = (int)(m->AckRange[0]);
    acks[1] = (int)(m->AckRange[1]);

    if((acks[0] < 0 || acks[1] < 0) || (acks[1] < acks[0])){
        return false;
    }

    return true;
}

boolean ParseType(Message *m, MessageType *t){
    switch(m->Type){
        case 0:
            *t = Standard;
            return true;
            break;
        case 1:
            *t = ACK;
            return true;
            break;
        case 2:
            *t = Inscription;
            return true;
            break;
        case 3:
            *t = GlobalDif;
            return true;
            break;
        default:
            return false;
    }
}