
#ifndef INO
#define INO
#include <Sodaq_RN2483.h>
#include <math.h>
#include "objects.h"
#include "factory.h"


#define debugSerial SerialUSB
#define loraSerial Serial2


/* defines test paramters */
Gateway gw1, gw2, gw3;

void populateGWCoordinates(){
    IntToPoint(gw1.Position.Latitude, -57.11);
    IntToPoint(gw1.Position.Longitude, -40.38);

    IntToPoint(gw2.Position.Latitude, -57.25);
    IntToPoint(gw2.Position.Longitude, -41.20);

    IntToPoint(gw3.Position.Latitude, -62);
    IntToPoint(gw3.Position.Longitude, -43.3);
}

void printGPSPoint(GPSPoint gp){
    SerialUSB.print(PointToInt(gp.Latitude));
    SerialUSB.print(",");
    SerialUSB.println(PointToInt(gp.Longitude));
}

void setup()
{
    // put your setup code here, to run once:
    debugSerial.begin(9600);
    Serial2.begin(57600);

    pinMode(LED_BUILTIN, OUTPUT);


    while (!SerialUSB && millis() < 1000)
        ;

  
    digitalWrite(LED_BUILTIN, HIGH);

    delay(2000);

    debugSerial.println("Hello");

    SetTime(1552809087);

    SerialUSB.print("Reference time is (1552809087) : ");
    SerialUSB.println(GetTime());

    SerialUSB.print("Satellite radius is : ");
    SerialUSB.println(sat.CONE_RADIUS);

    populateGWCoordinates();
    debugSerial.println("=> GWs populated");

    RoutingTable *table;

    Gateway gw1;
    gw1.GatewayID = 1;
    AddGateway(&gw1, gw1.Network, table);

    GPSPoint pt1 = ComputeSatPositionAtTime(GetTime());
    printGPSPoint(pt1); // expected -57.11,-40.38

    debugSerial.print("Checking gw in sight for now (1552809087) : ");
    Gateway *isgw1 = GetNextInsightGateway(0, table);


    debugSerial.print("Checking gw in sight for 100 seconds later (1552809187) : ");
    Gateway *isgw2 = GetNextInsightGateway(100, table)

    for(int i = 0; i < MAXNETWORKAGREGATION * MAXROUTINGTABLEGATEWAYSIZE; i++){
        if(*(isgw2+i).GatewayID != -1){
            SerialUSB.print("Found GW ");
            SerialUSB.print(*(isgw2+i).GatewayID);
            SerialUSB.print(" ");
            SerialUSB.print(PointToInt(*(isgw2+i).Position.Latitude));
            SerialUSB.print(" , ");
            SerialUSB.print(PointToInt(*(isgw2+i).Position.Longitude));
        }
    }
    

    debugSerial.println("Bye");
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO