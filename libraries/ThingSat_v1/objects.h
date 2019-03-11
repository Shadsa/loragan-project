#ifndef OBJECTS_H
#define OBJECTS_H
#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>

/* CONSTANT AND PARAMETER */
const int MAXGATEWAYPAYLOADSIZE = 50;      //Max size for a payload destined to a gateway
const int MAXDOWNLINKPAYLOADSIZE = 50;     //Max size for buffering Downlink Mote (used on Inscription message)
const int MAXROUTINGTABLEGATEWAYSIZE = 50; //Number of maximum gateway stored for each network.
const int MAXROUTINGTABLEMOTESIZE = 500;   //Number of maximum Mote stored for each network.
const int MAXSTORAGE = 1000;               //Number of maximum messages stored.
const int MAXNETWORKAGREGATION = 5;

//enum ObjectType {Mote, Gateway, Station};
enum MessageType
{
    Standard,    // 00
    ACK,         // 01
    Inscription, // 10
    GlobalDif    // 11
};               //To check, not really friendly user

//Define all structure for easier manipulation

typedef struct GPSPoint
{
    byte Longitude[2]; //struct of bits : 0 : sign | 000 0000 0 : 3 decimal for +/- 180 deg | 000 0000 : 2 decimal for precision.
    byte Latitude[2];  //precision around the kilometer.
                       //store alt and long in the format 000.00
};

typedef struct LPGANNetwork
{
    short ID;
    byte NetworkPrefix; //We support only network wich have 1 byte of prefix for the moment
};

typedef struct Gateway
{
    LPGANNetwork *Network;
    byte GatewayID[2];
    byte GatewayEUI[8];
    GPSPoint Position;
    float DropRatio = 0; // it's the Success/Transmit ratio. Below 0.5, the gateway will be droped.
};

typedef struct Mote
{
    LPGANNetwork *Network;
    byte DevADDR[4]; //the 7th first bits give the network prefix. May be redundant with Network* link.
    byte MoteID[2];
    GPSPoint Position;
};

typedef struct Message
{
    byte Type;        // 00 => message type on first 2 bits
    byte AckRange[2]; // byte 0 : low boundary - byte 1 : high boundary => Message ACK range (can ACK 64 message at best)
                      // Sat can only receive at best 30 msg in a minute, and a gateway will only ACK 50 message (paylaod size)
                      // so  it should be enough. For ACK only one message, range should be the same number repeated.
                      // In case of no ACK, leave it null (fill with 0)
    byte Payload[50]; //Decode depending the message type.
};

typedef struct Diff
{
    short DiffNumber;
    byte AddFlag;
    byte *DiffData[47];
};




//Define all timers for the Sat Algo
typedef struct SatTimers
{
    byte ListenTime = 1;     //Listening time in minute for the case of global packet ACK
    byte MessageTimeout = 1; //Time in day when a message will expire if not delivered
};

//Store Sat configuration
typedef struct Sat
{
    GPSPoint Position;
    SatTimers Timers;

    byte MaxGatewayPayloadSize = MAXGATEWAYPAYLOADSIZE;          //Max size for a payload destined to a gateway
    byte MaxDownlinkPayloadSize = MAXDOWNLINKPAYLOADSIZE;        //Max size for buffering Downlink Mote (used on Inscription message)
    int MaxRoutingTableGatewaySize = MAXROUTINGTABLEGATEWAYSIZE; //Number of maximum gateway stored for each network.
    int MaxRoutingTableMoteSize = MAXROUTINGTABLEMOTESIZE;       //Number of maximum Mote stored for each network.
    int MaxNetworkAgregation = MAXNETWORKAGREGATION;             /*Number of maximum network deserved by the satellite. 
                                                                *Important because it multiply every value in term of memory size. 
                                                                *Add network only if you are sure that you have enough memory.*/
};

typedef struct RoutingTable
{

    int *NetworksLocalDif[MAXNETWORKAGREGATION];                         //List of dif stage by Network
    int GlobalDif;                                                       //Num of the GlobalDif (Autoritative server only)
    LPGANNetwork *Networks[MAXNETWORKAGREGATION];                        //List of Networks allowed and their ID
    Gateway *Gateways[MAXNETWORKAGREGATION][MAXROUTINGTABLEGATEWAYSIZE]; //List of allowed gateway by network
    Mote *Motes[MAXNETWORKAGREGATION][MAXROUTINGTABLEMOTESIZE];          //List of subscribed mote by Network
};

#endif // OBJECTS_H

/*
@&&@@&&&&&&%%%%%%%%%%(((///:,.,://((((%%%%%%%%%%&&&&&&@@&&@
&%%&&&&%%%%%%###%%%%%#(///::,.,:///((#%%%%%###%%%%%%&&&&%%&
%##%%%%%%%%%%%#((#%%%%#(/::,,,,,::/(#%%%%####%%%%%%%%%&%##%
#//#%%%%%%%%%%%#(((#%###(/:,:,:::/(###%#(((#%%%%%%%%%%%#//#
/::#####%%%%%%%##(///##%#(/:::::/(#%##///(##%%%%%%%#####::/
:,:(#####%%%%%%%%#//:,,###(/:://(###,,://#%%%%%%%%#####(,,:
,.:######((((####(#(:,..,##(/:/(##...,/(#(####(((((####(,.,
. ,(################(/:.  ,(/:/(,  .:/(################(, .
.  ,/://(##########%%#(:,   ,:,   ,:(#%%##########(////,  .
    .,::(############(%%/:       :(%#(############(:,,     
     ,,:/(#######%#%#(/:,(/.   ,/(,:/##%%#%######/::,,     
      .,::/(####%%%##(/:,,,(/((/,,,:/(##%%%####(/::,.      
       .,,,. ./####((((((((#&&&((((((/((####:. .,,,.       
          .     /:(/:,,,/%%&&&&&%%/,,,:/(::               .
.                 ,,(#%%####%%%####%%%(,,                 .
,               .(###%%%%%%#(:(#%%%%%%###(,     .         ,
:.              .###%%%%%%%%%/%%%%%%%%%###               .:
/,.             /###%%%%%%%%%%%%%%%%%%%%##/             .,/
(/,.             (#%%%%%%#########%%%%%%#(             .,/(
#(/,.           , #%%%&&%%%%%#%%%%%&&%%%# .           .:/(#
(/:,.         . (##%%%%&&&&&&&&&%%%###( .         .,:/(%&
@&%#(/,..        :#########%%&%%#########:        ..:/(#%&@
*/