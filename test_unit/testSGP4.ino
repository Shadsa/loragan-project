#ifndef INO
#define INO

#include <math.h>
#include "objects.h"
#include "factory.h"

#define debugSerial SerialUSB

long timing;
SGP4ATmega predict;
SGP4ATmega::geodetic_t *latlon, zero= {0,0,0,0};

void setup()
{
    // put your setup code here, to run once:
    SerialUSB.begin(9600);


    while (!SerialUSB && millis() < 1000)
        ;

    delay(5000);
    
    SerialUSB.println("hello");

    SGP4ATmega::tle_t co57 = {10,
    144.03510745,//ye, then time
    .00000045,//ndot/2 drag parameter
    00000.0,//n float dot/6 Drag Parameter
    0.000042, //bstar drag parameter
    98.7132,//inclination IN
    152.4464, //RA
    .000873,//eccentricity EC
    245.714100, //WP
    114.3119,//mean anomaly MA
    14.20500354,//mean motion MM
    3031, //Sat cat number
    8022, // element set number
    35761,//revolution Number at Epoch
    "CO-57", "03031J"};//international Designation


  

    predict.setElements(co57);

    predict.setTime(1552809087);
    SerialUSB.println("Calculating Elements");
    latlon = &zero;
    predict.calc(predict.elements, latlon);
    SerialUSB.print((*latlon).lat);
    SerialUSB.print(",");
    SerialUSB.println((*latlon).lon);

    SerialUSB.println("=======");

    SerialUSB.println(predict.toRegularLong((*latlon).lon));

}

void loop()
{
    delay(5000);
}

#endif //INO