
#ifndef OBJECTS_H
#define OBJECTS_H
#include <Arduino.h>

#include <stdint.h>
#include <Stream.h>

enum ObjectType {Mote, Gateway, Station};
enum MessageType {Message, ACK, GlobalDif}; //To check, not really friendly user

//Define all structure for easier manipulation

typedef struct Gateway
{
    byte GatewayEUI[4];
    float Longitude;
    float Latitude;
};

typedef struct Network
{
    byte ID[2];
};

typedef struct Mote
{
    byte DevADDR[4];
};

typedef struct Message
{
    byte TypeAndRange;  // 00 => message type on first 2 bites | 00 0000 => Message ACK range (can ACK 64 message at best)
                        //                                     |            Sat can only receive at best 30 msg in a minute
                        //                                                  so  it should be enough.
    
    byte Payload[50];
};


//Define all timers for the Sat Algo
typedef struct SatTimers
{
    byte ListenTime = 1; //Listening time in minute for the cas of global packet ACK
};


typedef struct Sat
{
    float Longitude;
    float Latitude;

};

// TODO CONFIRM
typedef struct Gps
{
    float Longitude;
    float Latitude;
    
};


#endif // OBJECTS_H