#ifndef OBJECTS_H
#define OBJECTS_H
#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>


/* CONSTANT AND PARAMETER */
#define MAXGATEWAYPAYLOADSIZE 50;        //Max size for a payload destined to a gateway
#define MAXDOWNLINKPAYLOADSIZE 50;       //Max size for buffering Downlink Mote (used on Inscription message)
#define MAXROUTINGTABLEGATEWAYSIZE 50;    //Number of maximum gateway stored for each network.
#define MAXROUTINGTABLEMOTESIZE 500;      //Number of maximum Mote stored for each network.
#define MAXNETWORKAGREGATION 5; 

//enum ObjectType {Mote, Gateway, Station};
enum MessageType {Standard, ACK, Inscription, GlobalDif}; //To check, not really friendly user



//Define all structure for easier manipulation

typedef struct GPSPoint
{
    byte Longitude[2];  //struct of bits : 0 : sign | 000 0000 0 : 3 decimal for +/- 180 deg | 000 0000 : 2 decimal for precision.
    byte Latitude[2];   //precision around the kilometer.
                        //store alt and long in the format 000.00
};


typedef struct Network
{
    int ID;
    byte NetworkPrefix;
};


typedef struct Gateway
{
    Network* Network;
    byte GatewayEUI[4];
    GPSPoint Position;
    float DropRatio = 0;
};

typedef struct Mote
{
    Network* Network;
    byte DevADDR[4];    //the 7th first bits give the network prefix. May be redundant with Network* link.
    GPSPoint Position;
};

typedef struct Message
{
    byte TypeAndRange;  // 00 => message type on first 2 bites | 00 0000 => Message ACK range (can ACK 64 message at best)
                        //                                     |            Sat can only receive at best 30 msg in a minute, and a gateway will only ACK 50 message (paylaod size)
                        //                                                  so  it should be enough. For ACK only one message, range should be the same number repeated. In case of no
                        //                                                  ACK, leave it null (fill with 0)
    byte SenderDevADDR[4];   //DevADDR could be GatewayEUI also
    byte ReceiverDevADDR[4]; //DevADDR could be GatewayEUI also
    byte Payload[50];        //Decode depending the message type.
};




//Define all timers for the Sat Algo
typedef struct SatTimers
{
    byte ListenTime = 1;        //Listening time in minute for the case of global packet ACK
    byte MessageTimeout = 1;    //Time in day when a message will expire if not delivered
};


//Store Sat configuration
typedef struct Sat
{
    GPSPoint Position;
    SatTimers Timers;

    byte MaxGatewayPayloadSize = MAXGATEWAYPAYLOADSIZE;         //Max size for a payload destined to a gateway
    byte MaxDownlinkPayloadSize = MAXDOWNLINKPAYLOADSIZE;       //Max size for buffering Downlink Mote (used on Inscription message)
    int MaxRoutingTableGatewaySize = MAXROUTINGTABLEGATEWAYSIZE;//Number of maximum gateway stored for each network.
    int MaxRoutingTableMoteSize = MAXROUTINGTABLEMOTESIZE;      //Number of maximum Mote stored for each network.
    int MaxNetworkAgregation = MAXNETWORKAGREGATION;            //Number of maximum network deserved by the satellite. 
                                                                //Important because it multiply every value in term of memory size. 
                                                                //Add network only if you are sure that you have enough memory.
    
};

typedef struct RoutingTable
{
    /* data */
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
