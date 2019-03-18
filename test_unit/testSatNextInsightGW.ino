
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

    GPSPoint pt1 = ComputeSatPositionAtTime(GetTime());
    printGPSPoint(pt1); // expected -57.11,-40.38

    debugSerial.print("Checking gw in sight for now : ");
    Gateway *isgw1 = GetNextInsightGateway(0, );


    debugSerial.print("Checking gw in sight for 100 seconds later : ");
    Gateway *isgw2 = GetNextInsightGateway(100, )


    debugSerial.print("Time t=1, delivering GW 2 (yes) : ");
    bool b2 = IsInSightOf(gw2);
    debugSerial.println(b2);

    debugSerial.print("Time t=1, delivering GW 3 (no) : ");
    bool b3 = IsInSightOf(gw3);
    debugSerial.println(b3);

    SetTime(1552809187);
    SerialUSB.print("Time is now (1552809187) : ");
    SerialUSB.println(GetTime());

    debugSerial.print("Time t=2, delivering to GW 1 (no) : ");
    bool b4 = IsInSightOf(gw1);
    debugSerial.println(b4);

    debugSerial.print("Time t=2, delivering to GW 2 (no) : ");
    bool b5 = IsInSightOf(gw2);
    debugSerial.println(b5);
    
    debugSerial.print("Time t=2, delivering to GW 3 (yes) : ");
    bool b6 = IsInSightOf(gw3);
    debugSerial.println(b6);
    

    debugSerial.println("Bye");
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO