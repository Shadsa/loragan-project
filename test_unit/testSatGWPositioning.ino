
#ifndef INO
#define INO
#include <Sodaq_RN2483.h>
#include <math.h>
#include "objects.h"
#include "factory.h"


#define debugSerial SerialUSB
#define loraSerial Serial2

/* check if gateway is in sight or will be in sight using Haversine formula */
/* target is gw */
boolean isWithinRadius(float sat_lat, float sat_lon, float target_lat, float target_lon, int sat_radius){
    //debugSerial.println("=========== INPUT =============");
    //debugSerial.println(sat_lat);
    //debugSerial.println(sat_lon);
    //debugSerial.println(target_lat);
    //debugSerial.println(target_lon);
    //debugSerial.println("========================");
    int R = 6371; // Radius of the earth in km
    float toRad = (M_PI/180);

    // using rad instead of degrees
    float dLat = (target_lat - sat_lat) * toRad; 
    float dLon = (target_lon - sat_lon) * toRad; 
    float a = sinf(dLat/2) * sinf(dLat/2) + cosf(sat_lat * toRad) * cosf(target_lat * toRad) * sinf(dLon/2) * sinf(dLon/2);

    float c = 2 * atan2f(sqrtf(a), sqrtf(1-a)); 
    float d = R * c; // Distance in km
    
    if(d < sat_radius){
        return true;
    }

    return false;
}

/* defines the satellites pre-computed coordinates over the time. Time 'ticks' correspond to the index of the array */
GPSPoint sat_coordinates[5];
int sat_radius = 500; // satellite LOS in Km 
GPSPoint gw_coordinates[5];

void populateSatCoordinates(){
    float real_coordinates[3][2] = {{48.8561, 2.3504}, {45.7774, 3.0818}, {-4.322, 15.3129}}; // Paris -> Clermont-fd -> Kinshasa
    for(int i = 0; i < 3; i++){
        float r_lat = real_coordinates[i][0];
        float r_lon = real_coordinates[i][1]; 
        IntToPoint(sat_coordinates[i].Latitude, r_lat);
        IntToPoint(sat_coordinates[i].Longitude, r_lon);
    }
}

void populateGWCoordinates(){
    float real_coordinates[4][2] = {{48.8561, 2.3504}, {45.7774, 3.0818}, {45.8556, 3.5478}, {-4.322, 15.3129}}; // Paris, Clermont-fd, Thiers (< 100 km near Clermont-fd), Kinshasa
    for(int i = 0; i < 4; i++){
        float r_lat = real_coordinates[i][0];
        float r_lon = real_coordinates[i][1]; 
        IntToPoint(gw_coordinates[i].Latitude, r_lat);
        IntToPoint(gw_coordinates[i].Longitude, r_lon);
    }
}

void setup()
{
    // put your setup code here, to run once:
    debugSerial.begin(9600);
    Serial2.begin(57600);

    pinMode(LED_BUILTIN, OUTPUT);


    while (!SerialUSB && millis() < 1000)
        ;

  
    digitalWrite(LED_BUILTIN, HIGH);

    delay(5000);

    debugSerial.println("Hello");

    populateSatCoordinates();
    populateGWCoordinates();

    debugSerial.println("=> Satellite and GWs populated, radius (km) = 100");
    debugSerial.println("=> Satellite and GWs populated");

    debugSerial.print("Time t=1, satellite in Paris, delivering to Paris (true) : ");
    bool b1 = isWithinRadius(PointToInt(sat_coordinates[0].Latitude), PointToInt(sat_coordinates[0].Longitude), PointToInt(gw_coordinates[0].Latitude), PointToInt(gw_coordinates[0].Longitude), 100);
    debugSerial.println(b1);

    debugSerial.print("Time t=1, satellite in Paris, not delivering to Clermont-Fd (false) : ");
    bool b2 = isWithinRadius(PointToInt(sat_coordinates[0].Latitude), PointToInt(sat_coordinates[0].Longitude), PointToInt(gw_coordinates[1].Latitude), PointToInt(gw_coordinates[1].Longitude), 100);
    debugSerial.println(b2);

    debugSerial.print("Time t=1, satellite in Paris, not delivering to Kinshasa (false) : ");
    bool b3 = isWithinRadius(PointToInt(sat_coordinates[0].Latitude), PointToInt(sat_coordinates[0].Longitude), PointToInt(gw_coordinates[3].Latitude), PointToInt(gw_coordinates[3].Longitude), 100);
    debugSerial.println(b3);

    debugSerial.print("Time t=2, satellite in Clermont-Ferrand, not delivering to Paris (false) : ");
    bool b4 = isWithinRadius(PointToInt(sat_coordinates[1].Latitude), PointToInt(sat_coordinates[1].Longitude), PointToInt(gw_coordinates[0].Latitude), PointToInt(gw_coordinates[0].Longitude), 100);
    debugSerial.println(b4);

    debugSerial.print("Time t=1, satellite in Clermont-Ferrand, delivering to Thiers (true) : ");
    bool b5 = isWithinRadius(PointToInt(sat_coordinates[1].Latitude), PointToInt(sat_coordinates[1].Longitude), PointToInt(gw_coordinates[2].Latitude), PointToInt(gw_coordinates[2].Longitude), 100);
    debugSerial.println(b5);

    debugSerial.println("Bye");
}

void loop()
{
    // put your main code here, to run repeatedly:

    // Some data to send
  

    delay(5000);
}

#endif //INO