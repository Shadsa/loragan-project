#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
  0x26, 0x01, 0x18, 0x67
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
  0x50, 0x25, 0xC9, 0x51, 0xC0, 0x60, 0x5B, 0xC5, 0x0B, 0xE5, 0x87, 0xF6, 0x02, 0x60, 0xAB, 0xDB
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
  0x93, 0x98, 0x7E, 0x46, 0xA1, 0x75, 0x97, 0x7A, 0xCF, 0x32, 0x74, 0x3E, 0x3C, 0x16, 0xCB, 0x87
};

// Some complete random hex
uint8_t testPayload[] =
{
  0x66, 0x66, 0x66, 0x66, 0x66
};

void setup()
{
  while ((!debugSerial) && (millis() < 10000));
  
  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());

  LoRaBee.setDiag(debugSerial); // optional

  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    debugSerial.println("Connection to the network was successful.");
  }
  else
  {
    debugSerial.println("Connection to the network failed!");
  }

 // Uncomment this line to for the RN2903 with the Actility Network
 // For OTAA update the DEFAULT_FSB in the library
 // LoRaBee.setFsbChannels(1);

}

void loop()
{
  debugSerial.println("Sleeping for 5 seconds before starting sending out test packets.");
  for (uint8_t i = 5; i > 0; i--)
  {
    debugSerial.println(i);
    delay(1000);
  }

  // send 10 packets, with at least a 5 seconds delay after each transmission (more seconds if the device is busy)
  uint8_t i = 10;
  while (i > 0)
  {
    switch (LoRaBee.send(1, testPayload, 5))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      i--;
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe program will now halt.");
      while (1) {};
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe program will now halt.");
      while (1) {};
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
    delay(60000);
  }
}
