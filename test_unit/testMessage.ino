
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

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    while (!SerialUSB && millis() < 1000)
        ;

  
    digitalWrite(LED_BUILTIN, HIGH);

    delay(5000);

    debugSerial.println("Hello");

    // test some message parsing

    // 1
    Message m;
    int range_low_bound = 0;
    int range_high_bound = 40;
    m.AckRange[0] = lowByte(range_low_bound);
    m.AckRange[1] = lowByte(range_high_bound);

    int acks[2];

    boolean state = ParseACK(&m, acks);
   
    SerialUSB.println(state);
    SerialUSB.println(acks[0]);
    SerialUSB.println(acks[1]);


    // 2
    Message m1;
    int range_low_bound1 = 40;
    int range_high_bound1 = 10;
    m1.AckRange[0] = lowByte(range_low_bound1);
    m1.AckRange[1] = lowByte(range_high_bound1);

    int acks1[2];

    boolean state1 = ParseACK(&m1, acks1);
   
    SerialUSB.println(state1);
    SerialUSB.println(acks1[0]);
    SerialUSB.println(acks1[1]);

    // 3
    Message m2;
    int range_low_bound2 = 40;
    int range_high_bound2 = 40;
    m2.AckRange[0] = lowByte(range_low_bound2);
    m2.AckRange[1] = lowByte(range_high_bound2);

    int acks2[2];

    boolean state2 = ParseACK(&m2, acks2);
   
    SerialUSB.println(state2);
    SerialUSB.println(acks2[0]);
    SerialUSB.println(acks2[1]);

     // 4
    Message m3;
    int range_low_bound3 = 0;
    int range_high_bound3 = 0;
    m3.AckRange[0] = lowByte(range_low_bound3);
    m3.AckRange[1] = lowByte(range_high_bound3);

    int acks3[2];

    boolean state3 = ParseACK(&m3, acks3);
   
    SerialUSB.println(state3);
    SerialUSB.println(acks3[0]);
    SerialUSB.println(acks3[1]);

    SerialUSB.println("===============");
    // test MessageType
    MessageType t4;
    Message m4;

    m4.Type = Standard;
    bool r4 = ParseType(&m4, &t4);
    SerialUSB.println(r4); // 1
    SerialUSB.println(t4); // 0

    MessageType t5;
    Message m5;

    m5.Type = ACK;
    bool r5 = ParseType(&m5, &t5);
    SerialUSB.println(r5); // 1
    SerialUSB.println(t5); // 1

    MessageType t6;
    Message m6;

    m6.Type = 6;
    bool r6 = ParseType(&m6, &t6);
    SerialUSB.println(r6); // 0
    SerialUSB.println(t6); // undef

    MessageType t7;
    Message m7;

    m7.Type = GlobalDif;
    bool r7 = ParseType(&m7, &t7);
    SerialUSB.println(r7); // 1
    SerialUSB.println(t7); // 3

    MessageType t8;
    Message m8;

    m8.Type = 5;
    bool r8 = ParseType(&m8, &t8);
    SerialUSB.println(r8); // 1
    SerialUSB.println(t8); // 5

    SerialUSB.println("Bye");
    
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO
