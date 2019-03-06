
#ifndef OBJECTS_H
#define OBJECTS_H
#include <Arduino.h>

#include <stdint.h>
#include <Stream.h>

//enum ObjectType {Mote, Gateway, Station};
//enum MessageType {Message, ACK, GlobalDif}; //To check, not really friendly user

//Define all structure for easier manipulation

typedef struct GPSPoint
{
        
   
    byte Longitude[2];  //struct of bits : 0 : sign | 000 0000 0 : 3 decimal for +/- 180 deg | 000 0000 : 2 decimal for precision.
    byte Latitude[2];   //precision around the kilometer.
                        //store alt and long in the format 000.00
};


typedef struct Network
{
    byte ID[2];
    byte NetworkPrefix;
};


typedef struct Gateway
{
    Network* network;
    byte GatewayEUI[4];
    byte Status;
    GPSPoint position;
};

typedef struct Mote
{
    Network* network;
    byte DevADDR[4];    //the 7th first bits give the network prefix. May be redundant with Network* link.
    byte Status;
    GPSPoint Position;
};

typedef struct Message
{
    byte TypeAndRange;  // 00 => message type on first 2 bites | 00 0000 => Message ACK range (can ACK 64 message at best)
                        //                                     |            Sat can only receive at best 30 msg in a minute, and a gateway will only ACK 50 message (paylaod size)
                        //                                                  so  it should be enough.
    
    byte Payload[50];   //Decode depending the message type.
};


//Define all timers for the Sat Algo
typedef struct SatTimers
{
    byte ListenTime = 1; //Listening time in minute for the cas of global packet ACK
};


//Store Sat configuration
typedef struct Sat
{
    GPSPoint Position;
    byte MaxPayloadSize = 50;
    SatTimers Timers;
};


#endif // OBJECTS_H