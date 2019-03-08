#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>
#include "objects.h"

/* GPSPoint Manipulation*/
void IntToPoint(byte point[], float value);
float PointToInt(byte point[]);


/* Gateway things */
boolean DropGateway(Gateway gw);
boolean IsGatewayDropped(Gateway gw);   //Compute if the gateway have to be dropped
Gateway GetNextInsightGateway();
int GetNetworkId(byte DEVADDR[4]);
boolean IsInSightOf(Gateway gw);

/* Payload & message manipulation */
boolean PreparePayload(LPGANNetwork n);
boolean SendMessage(Message m);
boolean ReceiveMessage(Message m);
MessageType ParseType(Message m);
boolean ParseACK(Message m, int acks[2]);
boolean EncodeTypeAndACK(MessageType type,int lowRange, int highRange, Message m);
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
boolean DropGateway(LPGANNetwork *network,Gateway gw, RoutingTable table);