#ifndef FACTORY_H
#define FACTORY_H

#include <Arduino.h>
#include <stdint.h>
#include <Stream.h>
#include <math.h>
#include <stdio.h>
#include "objects.h"

#define xke		7.43669161E-2
#define tothrd		6.6666666666666666E-1	/* 2/3 */
#define ck2		5.413079E-4
#define ae		1.0
#define xkmper		6.378137E3		/* WGS 84 Earth radius km */
#define SIMPLE_FLAG            0x000020
#define twopi		6.28318530717958623	/* 2*Pi  */
#define s		1.012229
#define qoms2t		1.880279E-09
#define xj3		-2.53881E-6		/* J3 Harmonic (WGS '72) */   
#define ck4		6.209887E-7
#define pio2		1.57079632679489656	/* Pi/2 */
#define x3pio2		4.71238898038468967	/* 3*Pi/2 */
#define pi		3.14159265358979323846	/* Pi */
#define e6a		1.0E-6
#define secday		8.6400E4	/* Seconds per day */
#define omega_E		1.00273790934	/* Earth rotations/siderial day */
#define f		3.35281066474748E-3	/* Flattening factor */
#define SGP4_INITIALIZED_FLAG  0x000002
#define epoch_start     2440587.50000
#define minday		1.44E3			/* Minutes per day */
#define deg2rad		1.745329251994330E-2	/* Degrees to radians */
#define DEEP_SPACE_EPHEM_FLAG  0x000040


/* GPSPoint Manipulation*/
void IntToPoint(byte point[2], float value);
float PointToInt(byte point[2]);

/*GPS Function*/
boolean IsInSightOf(Gateway &gw);
Gateway &GetNextInsightGateway();

/* Routing Table Management */
boolean IsGatewayDropped(Gateway &gw);                                          //Compute if the gateway have to be dropped
short GetNetworkId(Gateway &gw);                                                //return -1 if gw is null
short GetNetworkId(byte DevADDR[4], RoutingTable &table);                       //return -1 if table null, -2 if not found
boolean AddGateway(Gateway &gw, LPGANNetwork &network, RoutingTable &table);    // Return false in case of nullptr or gateway not found, true if it found it and add it
boolean DeleteGateway(Gateway &gw, LPGANNetwork &network, RoutingTable &table); // Return false in case of nullptr or gateway not found, true if it found it and delete it
boolean AddMote(Mote m);
boolean DeleteMote(Mote m);
Gateway &foundGatewayByEUI(byte GatewayEUI[8], RoutingTable &table, Gateway &temp);
Gateway &foundGatewayByID(byte GatewayID[2], RoutingTable &table, Gateway &temp);
Mote &foundMoteByDEVADDR(byte DEVADDR[4], RoutingTable &table, Mote &temp);
Mote &foundMoteByID(byte MoteID[2], RoutingTable &table,Mote &temp);
LPGANNetwork &foundNetworkByID(short NetID, RoutingTable &table, LPGANNetwork &temp);
LPGANNetwork &foundNetworkByPrefix(byte Prefix, RoutingTable &table, LPGANNetwork &temp);

/* Payload & message manipulation */
boolean PreparePayload(LPGANNetwork n);
boolean SendMessage(Message m);
boolean ReceiveMessage(Message m);
boolean ParseType(Message &m, MessageType &t);
boolean ParseACK(Message &m, int acks[2]);
boolean EncodeTypeAndACK(MessageType type, int lowRange, int highRange, Message m);
boolean ParsePayload(Message m);
boolean Flush(Message m, LPGANNetwork n);

/* Time things */
long GetTime();
void SetTime(long t);



/* Storage */
boolean DeleteStorageMessage(Message &m, Message buffer[]);
boolean AddStorageMessage(Message &m, Message buffer[]);

/* DIFF FUNCTIONS  */
boolean ApplyDiff(Diff &d, RoutingTable &table);

#endif // FACTORY_H