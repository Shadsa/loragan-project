#ifndef INO
#define INO
#include <Sodaq_RN2483.h>
#include "../libraries/ThingSat_v1/objects.h"
#include "../libraries/ThingSat_v1/factory.h"


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

    // test Gateway function
    
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO