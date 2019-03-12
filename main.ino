#ifndef INO
#define INO

#include <Sodaq_RN2483.h>
#include "libraries/ThingSat_v1/objects.h"
#include "libraries/ThingSat_v1/factory.h"

#define debugSerial SerialUSB
#define loraSerial Serial2

int8_t trPower = 1;              // Transreceiver power  ( can be -3 to 15)
String SprFactor = "sf12";       // Spreadingsfactor     (can be sf7 to sf12)
uint8_t max_dataSize = 100;      // Maximum charcount to avoid writing outside of string
unsigned long readDelay = 10000; // Time to read for messages in ms (max 4294967295 ms, 0 to disable)
                                 // 10S instead of 60 in order to divide the listen time in more easy to use delay

const char CR = '\r';
const char LF = '\n';

// Configuring the RN2483 for P2P
void LoraP2P_Setup()
{

    Serial2.print("sys reset\r\n");
    delay(200);
    Serial2.print("radio set pwr ");
    Serial2.print(trPower);
    Serial2.print("\r\n");
    delay(100);
    Serial2.print("radio set sf ");
    Serial2.print(SprFactor);
    Serial2.print("\r\n");
    delay(100);
    Serial2.print("radio set wdt ");
    Serial2.print(readDelay);
    Serial2.print("\r\n");
    delay(100);
    Serial2.print("mac pause\r\n");
    delay(100);

    FlushSerialBufferIn();
    SerialUSB.write("End of P2P setup\n");
}

// Send Data array (in HEX)
void LORA_Write(char *Data)
{
    SerialUSB.write("Start Writing in Lora\n");
    Serial2.print("radio tx ");
    Serial2.print(Data);
    Serial2.print("\r\n");
    Serial2.flush();

    waitTillMessageGone();
    SerialUSB.write("End Writing in Lora\n");
    FlushSerialBufferIn();
}

// Waits until the data transmit is done
void waitTillMessageGone()
{
    while (!Serial2.available())
    {
    }
    delay(10);
    while (Serial2.available() > 0)
      SerialUSB.write(Serial2.read());
}

// Setting up the receiver to read for incomming messages
void StartLoraRead()
{
    Serial2.print("radio rx 1000\r\n");
    delay(100);

    FlushSerialBufferIn();
}

//////////////////////////////////////
// Read message from P2P TX module  //
// Returns 1 if there is a message  //
// Returns 2 if there is no message //
//////////////////////////////////////
int LORA_Read(char *Data)
{
    int messageFlag = 0;
    String dataStr = "radio_rx  ";
    String errorStr = "radio_err";
    String Buffer = "";

    SerialUSB.println("Start Reading");

    StartLoraRead();

    while (messageFlag == 0) // As long as there is no message
    {
        while (!Serial2.available())
            ;

        delay(50); // Some time for the buffer to fill

        // Read message from RN2483 LORA chip
        while (Serial2.available() > 0 && Serial2.peek() != LF)
        {
            Buffer += (char)Serial2.read();
        }

        // If there is an incoming message
        if (Buffer.startsWith(dataStr, 0)) // if there is a message in the buffer
        {
            int i = 10; // Incoming data starts at the 11th character

            // Seperate message from string till end of datastring
            while (Buffer[i] != CR && i - 10 < max_dataSize)
            {
                Data[i - 10] = Buffer[i];
                i++;
            }
            messageFlag = 1; // Message received
        } 
        else if (Buffer.startsWith(errorStr, 0))
        {
            messageFlag = 2; // Read error or Watchdogtimer timeout
        }
    }

#ifdef DEBUG
    SerialUSB.println(Buffer);
#endif
    SerialUSB.println("End Reading");
    return (messageFlag);
}

// Flushes any message available
void FlushSerialBufferIn()
{
    while (Serial2.available() > 0)
    {
        Serial2.read();
    }
}

void setup()
{
    // put your setup code here, to run once:
    SerialUSB.begin(57600);
    Serial2.begin(57600);

    //pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    while (!SerialUSB && millis() < 1000)
        ;

    LoraP2P_Setup();
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_GREEN,LOW  ); // Light up LED if there is a message
    delay(5000);
    digitalWrite(LED_GREEN, HIGH);

}

void LoopRead(){
    char Answer[100] = "";
    long actualtime = millis();
    SerialUSB.println("**************** ENTERING LISTENING LOOP *********************");
    while (millis() < actualtime + 6 * readDelay)
    {
        if (LORA_Read(Answer) == 1)
        {
            digitalWrite(LED_GREEN, LOW); // Light up LED if there is a message
            SerialUSB.println(Answer);
            digitalWrite(LED_GREEN, HIGH);
        }

    }
    SerialUSB.println("**************** Exiting LISTENING LOOP *********************");
}

void LoopRead(long next_interruption){
    char Answer[100] = "";
    long actualtime = millis();
    SerialUSB.println("**************** ENTERING LISTENING LOOP *********************");
    while (millis() < actualtime + 6 * readDelay)
    {
        if (LORA_Read(Answer) == 1)
        {
            digitalWrite(LED_GREEN,LOW  ); // Light up LED if there is a message
            SerialUSB.println(Answer);
            digitalWrite(LED_GREEN, HIGH);
        }

        if(millis()+1000 >= next_interruption){
            break;
        }
    }
    SerialUSB.println("**************** Exiting LISTENING LOOP *********************");
}

void loop()
{
    // put your main code here, to run repeatedly:

    for (int i = 0; i < MAXNETWORKAGREGATION; i++) {
        for (int j = 0; j < MAXROUTINGTABLEGATEWAYSIZE;j++) {
            if (IsGatewayDropped(table.Gateways[i][j])) {
                DeleteGateway(table.Gateways[i][j], table.Networks[i], table);
            }
        }
    }

    long TimeNextZE = NextZE();
    Gateway* NextGateways[] = GetNextInsightGateways();
    Gateway* NextMotes[] = GetNextInsightMotes();
    PreparePayload(NextGateways,NextMotes);
    while (millis()<TimeNextZE) {
        LoopRead(TimeNextZE);
    }
    while (IsInSightOf(TimeNextZE) /*&& notEmpty(payload)*/) {
        // send(payload);
        delay(2000);
    }

    // Some data to send
    char Data[1000] = "4141414142";

    LORA_Write(Data);

    delay(1000); //DO NOT TOUCH !! BREAKING CODE EVERY TIME

    LoopRead();
}

#endif