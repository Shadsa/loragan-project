#ifndef INO
#define INO
#include <Sodaq_RN2483.h>
#include "objects.h"
#include "factory.h"


#define debugSerial SerialUSB
#define loraSerial Serial2


void setup()
{
    // put your setup code here, to run once:
    debugSerial.begin(9600);
    Serial2.begin(57600);

    while (!SerialUSB && millis() < 1000)
        ;

    delay(5000);

    debugSerial.println("Hello");

    // test Gateway function

    Gateway gw1;
    gw1.DropRatio = 0.25;
    gw1.Network->ID = 100;
    gw1.GatewayEUI[0] = '0';
    gw1.GatewayEUI[1] = '0';
    gw1.GatewayEUI[2] = '0';
    gw1.GatewayEUI[3] = '0';
    gw1.GatewayEUI[4] = '0';
    gw1.GatewayEUI[5] = '1';
    gw1.GatewayEUI[6] = '1';
    gw1.GatewayEUI[7] = '1';

    Gateway gw2;
    gw2.DropRatio = 0.89;
    gw2.Network->ID = 200;
    gw1.GatewayEUI[0] = '0';
    gw1.GatewayEUI[1] = '0';
    gw1.GatewayEUI[2] = '0';
    gw1.GatewayEUI[3] = '0';
    gw1.GatewayEUI[4] = '0';
    gw1.GatewayEUI[5] = '2';
    gw1.GatewayEUI[6] = '2';
    gw1.GatewayEUI[7] = '1';

    Gateway gw3;
    gw3.DropRatio = 1;
    gw3.Network->ID = 200;
    //gw3.GatewayEUI = "0000222";

    SerialUSB.print("Is Gateway 1 dropped (yes) : ");
    SerialUSB.println(IsGatewayDropped(&gw1));
    
    SerialUSB.print("Gateway 1 NW ID (100) : ");
    SerialUSB.println(GetNetworkId(&gw1));

    SerialUSB.println("Is Gateway 2 dropped (yes) : " + IsGatewayDropped(&gw2));

    RoutingTable *table;

    // add GW 1 to table
    bool s = AddGateway(&gw1, gw1.Network, table);
    if(s){
        SerialUSB.println("Add suceeded");
    } else {
        SerialUSB.println("Add failed");
    }
    
    // check routing table size
    //sizeof(table->Gateways[network->ID])
    
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  
    SerialUSB.println("hb");
    delay(1000);
}

#endif //INO