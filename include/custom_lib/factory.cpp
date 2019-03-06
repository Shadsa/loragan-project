#include <Sodaq_RN2483.h>
#include "factory.h"

// GPS Point Manipulation function 
void IntToPoint(byte* point[2], float value){
    
}

int PointToInt(byte* point[2]){

    return 0;
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