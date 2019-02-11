#include <TheThingsNetwork.h>
#include <CayenneLPP.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "26011867";
const char *nwkSKey = "93987E46A175977ACF32743E3C16CB87";
const char *appSKey = "5025C951C0605BC50BE587F60260ABDB";

#define loraSerial Serial2
#define debugSerial SerialUSB

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);
uint8_t led = 0;

void setup()
{
    loraSerial.begin(57600);
    debugSerial.begin(9600);

    // Wait a maximum of 10s for Serial Monitor
    while (!debugSerial && millis() < 10000)
        ;

    debugSerial.println("-- PERSONALIZE");
    ttn.personalize(devAddr, nwkSKey, appSKey);

    debugSerial.println("-- STATUS");
    ttn.showStatus();

    // Set callback for incoming messages
    ttn.onMessage(message);
    pinMode(LED_BUILTIN, OUTPUT);

    randomSeed(analogRead(0));
}

uint16_t getTemperature()
{
    //10mV per C, 0C is 500mV
    float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
    int temp = (mVolts - 500) * 10;

    debugSerial.print((mVolts - 500) / 10);
    debugSerial.println(" Celcius");
    return int(temp);
}
float CA_getTemperature(){
  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.00 / 1023.00;
  float temp = (mVolts - 500.00) / 10.0;

  debugSerial.print((mVolts - 500) / 10);
  debugSerial.println(" Celcius");
  return float(temp);
}

void message(const byte *payload, size_t size, port_t port)
{
    debugSerial.println("-- MESSAGE");

    if (size != 4 && payload[0] != 2 && payload[1] != 00)
    {
        debugSerial.println("Order not supported");
    }

    if (payload[2] > 0)
    {
        debugSerial.println("LED ON");
        led = 1;
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        debugSerial.println("LED OFF");
        led = 0;
        digitalWrite(LED_BUILTIN, LOW);
    }
    
}

void loop()
{
   
}