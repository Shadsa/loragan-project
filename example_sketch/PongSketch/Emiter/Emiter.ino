#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Sodaq_RN2483.h>

#if defined(ARDUINO_SODAQ_EXPLORER)
#define loraSerial Serial2
#else
#define loraSerial Serial1
#endif
#define debugSerial SerialUSB

#define LORA_BAUD 57600
#define DEBUG_BAUD 57600

int counter = 0;

void setup()
{
// put your setup code here, to run once:
// Enable LoRa module
#if defined(ARDUINO_SODAQ_AUTONOMO)
    pinMode(BEE_VCC, OUTPUT);
    digitalWrite(BEE_VCC, HIGH); //set input power BEE high
#endif

    loraSerial.begin(LORA_BAUD);
    debugSerial.begin(DEBUG_BAUD);

    // Wait forever for Serial Monitor
    while (!debugSerial)
        ;

#ifdef LORA_RESET
    //Hardreset the RN module
    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, HIGH);
    delay(100);
    digitalWrite(LORA_RESET, LOW);
    delay(100);
    digitalWrite(LORA_RESET, HIGH);
    delay(1000);

    // empty the buffer
    loraSerial.end();
#endif
    loraSerial.begin(LORA_BAUD);

    // get the Hardware DevEUI
    debugSerial.println("Get the hardware serial, sending \"sys get hweui\\r\\n\", expecting \"xxxxxxxxxxxxxxxx\", received: \"");
    delay(100);
    loraSerial.println("sys get hweui");
    delay(100);

    char buff[16];
    memset(buff, 0, sizeof(buff));

    loraSerial.readBytesUntil(0x20, buff, sizeof(buff));
    debugSerial.print(buff);

    debugSerial.println();

    debugSerial.println("LoRa Receiver");
}
void loop()
{
    
}
