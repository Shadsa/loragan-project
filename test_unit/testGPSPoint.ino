
#ifndef INO
#define INO
#include <Sodaq_RN2483.h>
#include "libraries/ThingSat/objects.h"
#include "libraries/ThingSat/factory.h"


#define debugSerial SerialUSB
#define loraSerial Serial2


void setup()
{
    // put your setup code here, to run once:
    debugSerial.begin(9600);
    Serial2.begin(57600);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    while (!SerialUSB && millis() < 1000)
        ;

  
    digitalWrite(LED_BUILTIN, HIGH);

    delay(5000);

    debugSerial.println("Hello");

    // test  GPS Point Manipulation function
    byte test_long1[2];
    float value1 = 11.22;
    IntToPoint(test_long1, value1);
    float result1 = PointToInt(test_long1);
    SerialUSB.print("Result 1 : ");
    SerialUSB.println(result1);

    byte test_long2[2];
    float value2 = 25.6625;
    IntToPoint(test_long2, value2);
    float result2 = PointToInt(test_long2);
    SerialUSB.print("Result 2 : ");
    SerialUSB.println(result2);

    byte test_long3[2];
    float value3 = 180.6625;
    IntToPoint(test_long3, value3);
    float result3 = PointToInt(test_long3);
    SerialUSB.print("Result 3 : ");
    SerialUSB.println(result3);

    byte test_long4[2];
    float value4 = -180.6625;
    IntToPoint(test_long4, value4);
    float result4 = PointToInt(test_long4);
    SerialUSB.print("Result 4 : ");
    SerialUSB.println(result4);
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO