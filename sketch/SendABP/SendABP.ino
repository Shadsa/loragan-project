#include <TheThingsNetwork.h>
#include <CayenneLPP.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "26011867";
const char *nwkSKey = "93987E46A175977ACF32743E3C16CB87";
const char *appSKey = "5025C951C0605BC50BE587F60260ABDB";

//Original setup (initial position mainly)
float original_lat = 5.724524;
float original_long = 45.188529;
float original_alt = 300;

//For standard IFTTT integration or byte-to-byte analysis process
byte nbSat;
byte negative;
union {
    unsigned long lng;
    byte longitude[4];
} longitude;
union {
    unsigned long lat;
    byte latitude[4];
} latitude;
union {
    unsigned long alt;
    byte altitude[4];
} altitude;
union {
    unsigned long ut;
    byte UTime[4];
} UTime;

//For Cayenne integration
float ca_lat;
float ca_long;
float ca_alt;
float ca_nbsat;

//Color code
// 0 : all green
// 1 : slight movement, but nothing to worry
// 2 : medium moovement, need a look
// 3 : long and harsh movement, danger detected.
// 4 : already alert a state 3, standby to reset
uint8_t color;

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

    color = 0;
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
    //should reset state after downlink, assuming that a downlink
    // is aknowledging the alert state 3
    color =0;
}

void GPSSimulation()
{
    latitude.lat = random(2147483647);
    longitude.lng = random(2147483647);
    altitude.alt = random(2147483647);
    UTime.ut = random(2147483647);
    nbSat = random(10);
}

void Acceleration_Simulation()
{
    //All we do there si to say that a abnormal evenemtn is a lot more exceptional than no event for exemple

    int simu = random(19);
    if(simu <=14)
        color =0;
    else if(simu <=17)

        color=1;
    else if(simu <=18)
        color =2;
    else{
        color =3;
    }
}

void loop()
{
    //IFFTT and byte-to-byte code
    /*
    uint16_t temp = getTemperature();
    //16 byte for long information GPS format (https://things4u.github.io/DeveloperGuide/Message%20Format/message_format.html)
    GPSSimulation();
    negative = 1;

    /*
    byte[0-3] = LAT, 
    byte[4-7] = LNG, 
    byte[8-11]=ALT,
    byte[12-15] =UTime,
    byte[16] = nr of satellites
    byte[17] = negative fields. 1 = none
                                2= lat
                                3= LNG
                                4= alt
                                + combinaison (23,24,34)
    */
    /*
    byte payload[17];
    payload[0] = latitude.latitude[0];
    payload[1] = latitude.latitude[1];
    payload[2] = latitude.latitude[2];
    payload[3] = latitude.latitude[3];
    payload[4] = longitude.longitude[0];
    payload[5] = longitude.longitude[1];
    payload[6] = longitude.longitude[2];
    payload[7] = longitude.longitude[3];
    payload[8] = altitude.altitude[0];
    payload[9] = altitude.altitude[1];
    payload[10] = altitude.altitude[2];
    payload[11] = altitude.altitude[3];
    payload[12] = UTime.UTime[0];
    payload[13] = UTime.UTime[1];
    payload[14] = UTime.UTime[2];
    payload[15] = UTime.UTime[3];
    payload[16] = nbSat;
    payload[17] = negative;

    //+ add acceleration and color code

    debugSerial.println("-- PAYLOAD");
    ttn.sendBytes(payload, sizeof(payload));

    */



    //NOTE : In the function below,  the delay() call should be replace by a sample
    // of an existing accelerometer. Since the ExPlorer board don't support it,
    //there's just a simulation. We just take in case the two different situation :
    //Abnormal but not too important, and the critical state, and we have a different 
    //delay between both. 



   //first, check if the situation was T3 or not, than if not, ressimulate
    if(color>=0 && color <=2)
        Acceleration_Simulation();
    debugSerial.print("color =");
    debugSerial.println(color);
    //Second, check output of the simulation
    if(color>0 && color <=2){
        lpp.reset();
        //lpp.addTemperature(1, CA_getTemperature());
        lpp.addGPS(1,original_lat,original_long,original_alt);
        lpp.addDigitalOutput(2,color);
        ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
        debugSerial.println("Send payload for a slight to abnormal movement");
        color =0;
        delay(10000);
    //In case of a big shift in acceleration, we consider that there is a danger
    //So we continuously send our GPS position with a red alert until a downlink paquet is send
    //(See message function for mroe information)
    }else if(color>2){
        lpp.reset();
        //lpp.addTemperature(1, CA_getTemperature());
        lpp.addGPS(1,original_lat,original_long,original_alt);
        lpp.addDigitalOutput(2,color);
        ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
        debugSerial.println("Send an alert for a very important and abnormal displacement");
        delay(10000);
    }
    
    
}