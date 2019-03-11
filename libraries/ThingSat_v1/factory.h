#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>
#include "objects.h"

/* GPSPoint Manipulation*/
void IntToPoint(byte point[2], float value);
float PointToInt(byte point[2]);

/*GPS Function*/
boolean IsInSightOf(Gateway *gw);
Gateway *GetNextInsightGateway();

/* Routing Table Management */                     
boolean IsGatewayDropped(Gateway *gw);                  //Compute if the gateway have to be dropped
short GetNetworkId(Gateway *gw);                          //return -1 if gw is null
short GetNetworkId(byte DevADDR[4], RoutingTable *table); //return -1 if table null, -2 if not found
boolean AddGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table);// Return false in case of nullptr or gateway not found, true if it found it and add it
boolean DeleteGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table); // Return false in case of nullptr or gateway not found, true if it found it and delete it
boolean AddMote(Mote m);
boolean DeleteMote(Mote m);
Gateway *foundGatewayByEUI(byte GatewayEUI[8], RoutingTable *table);
Gateway *foundGatewayByID(byte GatewayID[2], RoutingTable *table);
Mote *foundMoteByDEVADDR(byte DEVADDR[4], RoutingTable *table);
Mote *foundMoteByID(byte MoteID[2], RoutingTable *table);
LPGANNetwork *foundNetworkByID(short NetID, RoutingTable *table);
LPGANNetwork *foundNetworkByPrefix(byte Prefix, RoutingTable *table);

/* Payload & message manipulation */
boolean PreparePayload(LPGANNetwork n);
boolean SendMessage(Message m);
boolean ReceiveMessage(Message m);
boolean ParseType(Message *m, MessageType *t);
boolean ParseACK(Message *m, int acks[2]);
boolean EncodeTypeAndACK(MessageType type, int lowRange, int highRange, Message m);
boolean ParsePayload(Message m);
boolean Flush(Message m, LPGANNetwork n);

/* Time things */
// TODO type : GetTime();
// TODO type : SetTime();


/* Others */
boolean ApplyDiff(Message m);

/* Storage */
boolean DeleteStorageMessage(Message m);
boolean AddStorageMessage(Message m);

#endif // FACTORY_H