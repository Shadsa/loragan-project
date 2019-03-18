#ifndef OBJECTS_H
#define OBJECTS_H
#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>
#include "spg4.h"


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
    /* Message from Ground*/
    Standard,    // 00
    ACK,         // 01
    Inscription, // 10
    GlobalDif,   // 11

    /* Message from Sat */
    SatSync, //Mote syncronous message
    SatACK   //Gateway ACK on diff receive
};           //To check, not really friendly user

//Define all structure for easier manipulation

typedef struct GPSPoint
{
    byte Longitude[2] = {0, 0}; //struct of bits : 0 : sign | 000 0000 0 : 3 decimal for +/- 180 deg | 000 0000 : 2 decimal for precision.
    byte Latitude[2] = {0, 0};  //precision around the kilometer.
                                //store alt and long in the format 000.00
};

SGP4ATmega::tle_t CurrentTLE = {10,
                                144.03510745, //ye, then time
                                .00000045,    //ndot/2 drag parameter
                                00000.0,      //n float dot/6 Drag Parameter
                                0.000042,     //bstar drag parameter
                                98.7132,      //inclination IN
                                152.4464,     //RA
                                .000873,      //eccentricity EC
                                245.714100,   //WP
                                114.3119,     //mean anomaly MA
                                14.20500354,  //mean motion MM
                                3031,         //Sat cat number
                                8022,         // element set number
                                35761,        //revolution Number at Epoch
                                "CO-57", "03031J"};

typedef struct LPGANNetwork
{
    short ID = -1;           //ID at -1 when not initialize
    byte StorageID = -1;     //Used to remember the index of the struct in Network list
    byte NetworkPrefix = -1; //We support only network wich have 1 byte of prefix for the moment
};

typedef struct Gateway
{
    LPGANNetwork *Network = nullptr;
    short GatewayID = -1; //ID at -1 when not initialize
    byte GatewayEUI[8];
    GPSPoint Position;
    float DropRatio = 0; // it's the Success/Transmit ratio. Below 0.5, the gateway will be droped.
};

typedef struct Mote
{
    LPGANNetwork *Network = nullptr;
    byte DevADDR[4] = {0, 0, 0, 0}; //the 7th first bits give the network prefix. May be redundant with Network* link.
    short MoteID = -1;              //ID at -1 when not initialize
    GPSPoint Position;
};

typedef struct Message
{
    byte Type = -1;                  // 00 => message type on first 2 bits
    short MessageID = -1;            // Never send in message, local storage utility only for buffering. -1 if null or not initialize
    long date = -1;                  //Use for timeout storage management
    LPGANNetwork *Network = nullptr; //Network targeted
    byte AckRange[2];                // byte 0 : low boundary - byte 1 : high boundary => Message ACK range (can ACK 64 message at best)
                                     // Sat can only receive at best 30 msg in a minute, and a gateway will only ACK 50 message (paylaod size)
                                     // so  it should be enough. For ACK only one message, range should be the same number repeated.
                                     // In case of no ACK, leave it null (fill with 0)
    byte Payload[50];                //Decode depending the message type.
};

typedef struct Diff
{
    short DiffNumber = -1;   /*Number used to identify and store different Diff.
                                We assume that 2 byte will be enough to ensure that two
                                diff will not have index collision in a short period of time.
                                DiffNumber at -1 when not initialize */
    byte AddFlag = -1;       //Number of Add instruction in the data buffer before Delete instruction
    byte MessageNumber = -1; //Number of message in the payload
    byte *DiffData[47];      /* instruction : For an Add statement :
                                        Gateway : [2] byte for GatewayID + [4] byte for a GPS point (Long and lat on 2 byte each)
                                        Network : exatcly the same structure as the LPGANNetwork structure define above

                                        For Delete statement :
                                        Gateway : [2] byte for GatewayID
                                        Network : [2] byte for NetworkID
                            */
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
    long MessageCounter = 0;

    long clock;
    int CONE_RADIUS = 100;

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

    int NetworksLocalDif[MAXNETWORKAGREGATION];                         //List of dif stage by Network
    int GlobalDif = 0;                                                      //Num of the GlobalDif (Autoritative server only)
    LPGANNetwork Networks[MAXNETWORKAGREGATION];                        //List of Networks allowed and their ID
    Gateway Gateways[MAXNETWORKAGREGATION][MAXROUTINGTABLEGATEWAYSIZE]; //List of allowed gateway by network
    Mote Motes[MAXNETWORKAGREGATION][MAXROUTINGTABLEMOTESIZE];          //List of subscribed mote by Network
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