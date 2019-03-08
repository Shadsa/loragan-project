#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>
#include "objects.h"

/* GPSPoint Manipulation*/
void IntToPoint(byte point[2], float value);
float PointToInt(byte point[2]);

/* Gateway things */                     
boolean IsGatewayDropped(Gateway *gw);                  //Compute if the gateway have to be dropped
int GetNetworkId(Gateway *gw);                          //return -1 if gw is null
int GetNetworkId(byte DevADDR[4], RoutingTable *table); //return -1 if table null, -2 if not found
boolean IsInSightOf(Gateway *gw);
Gateway *GetNextInsightGateway();
boolean AddGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table);// Return false in case of nullptr or gateway not found, true if it found it and add it
boolean DeleteGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table); // Return false in case of nullptr or gateway not found, true if it found it and delete it

/* Payload & message manipulation */
boolean PreparePayload(LPGANNetwork n);
boolean SendMessage(Message m);
boolean ReceiveMessage(Message m);
MessageType ParseType(Message m);
boolean ParseACK(Message m, int acks[2]);
boolean EncodeTypeAndACK(MessageType type, int lowRange, int highRange, Message m);
boolean ParsePayload(Message m);
boolean Flush(Message m, LPGANNetwork n);

/* Time things */
// TODO type : GetTime();
// TODO type : SetTime();

/* Mode */
boolean AddMote(Mote m);
boolean DeleteMote(Mote m);

/* Others */
boolean ApplyDiff(Message m);

/* Storage */
boolean DeleteStorageMessage(Message m);
boolean AddStorageMessage(Message m);

#endif // FACTORY_H
boolean DropGateway(LPGANNetwork *network, Gateway gw, RoutingTable table);