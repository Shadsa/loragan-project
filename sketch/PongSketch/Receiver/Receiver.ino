#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <Sodaq_RN2483.h>

#define loraSerial Serial2
#define debugSerial SerialUSB

void setup() {

  loraSerial.begin(57600);
  debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("LoRa Receiver");
}

void loop() {
  // try to parse packet
  int packetSize = loraSerial.parsePacket();
  if (packetSize) {
    // received a packet
    debugSerial.print("Received packet '");

    // read packet
    while (loraSerial.available()) {
      debugSerial.print((char)loraSerial.read());
    }

    // print RSSI of packet
    debugSerial.print("' with RSSI ");
    debugSerial.println(loraSerial.packetRssi());
  }
}