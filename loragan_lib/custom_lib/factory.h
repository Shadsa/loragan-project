#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>

#include <stdint.h>
#include <Stream.h>


/* Gateway things */
boolean DropGateway(Gateway gw);
boolean IsGatewayDropped(Gateway gw);
Gateway GetNextInsightGateway();
Network GetNetworkId(Network n);
boolean IsInSightOf(Gateway gw);
float GetGatewayLat(Gateway gw);
float GetGatewayLong(Gateway gw)

/* Payload & message manipulation */
boolean PreparePayload(Network n);
boolean SendMessage(Message m);
boolean ReceiveMessage(Message m);
boolean ParsePayload(Message m);
int ParseType();
boolean IsTypeInsertion(Message m);
boolean IsTypeDeletion(Message m);
// TODO type : ParseTime();
// TODO type : ParseAck();
boolean CheckCrc(Message m);
boolean Flush(Message m, Network n);


/* Time things */
// TODO type : GetTime();
// TODO type : SetTime();

/* Mode */
boolean AddMote(Mote m);
boolean DeleteMote(Mote m);

/* Others */
Wait();
boolean ApplyDiffId(Message m);

/* Storage */
boolean DeleteStorageMessage(Message m);
boolean AddStorageMessage(Message m);

/* GPS */
float GetGPSLat(Gps g);
float GetGPSLong(Gps g);
// TODO type : GetGPSTime();

#endif // FACTORY_H