#include <Sodaq_RN2483.h>
#include "Arduino.h"
#include "factory.h"
#include <math.h>
#include "spg4.h"

/* GPS POINT MANIPULATION FUNCTION */
void IntToPoint(byte point[2], float value)
{
    float integ;
    float frac = modff(value, &integ);
    int i = floor(abs(integ));
    int r = floor(abs(frac) * 100);

    byte _i = lowByte(i);
    *point = _i << 1;
    if (value < 0)
    {
        bitWrite(*point, 0, 1);
    }
    else
    {
        bitWrite(*point, 0, 0);
    }

    byte _f = lowByte(r);
    *(point + 1) = _f << 1;
    bitWrite(*(point + 1), 0, bitRead(_i, 7));
}

float PointToInt(byte point[2])
{
    bool sign = bitRead(*point, 0);

    byte i = *point >> 1;
    byte r = *(point + 1);
    //byte f = *(point+1) & 127; // not working - need to set bit manually
    bitWrite(i, 7, bitRead(r, 0));

    float nv = (float)i;
    nv += ((r >> 1) * 0.01);
    if (sign)
    {
        return -nv;
    }

    return nv;
}

/* GPS FUNCTION */
/* check if gateway is in sight or will be in sight using Haversine formula */
/* target is gw */
boolean IsWithinRadius(float sat_lat, float sat_lon, float target_lat, float target_lon, int sat_radius){
    //debugSerial.println("=========== INPUT =============");
    //debugSerial.println(sat_lat);
    //debugSerial.println(sat_lon);
    //debugSerial.println(target_lat);
    //debugSerial.println(target_lon);
    //debugSerial.println("========================");
    int R = 6371; // Radius of the earth in km
    float toRad = (M_PI/180);

    // using rad instead of degrees
    float dLat = (target_lat - sat_lat) * toRad; 
    float dLon = (target_lon - sat_lon) * toRad; 
    float a = sinf(dLat/2) * sinf(dLat/2) + cosf(sat_lat * toRad) * cosf(target_lat * toRad) * sinf(dLon/2) * sinf(dLon/2);

    float c = 2 * atan2f(sqrtf(a), sqrtf(1-a)); 
    float d = R * c; // Distance in km
    
    if(d < sat_radius){
        return true;
    }

    return false;
}

boolean IsInSightOf(Sat &sat,Gateway &gw)
{
    // get sat current pos
    GPSPoint pSat = ComputeSatPositionAtTime(GetTime());
    // computes

    /*SerialUSB.println("======== IsInSightOf DEBUG ==========");
    SerialUSB.print(PointToInt(pSat.Latitude));
    SerialUSB.print(" , ");
    SerialUSB.println(PointToInt(pSat.Longitude));*/

    /*SerialUSB.print(PointToInt(gw.Position.Latitude));
    SerialUSB.print(" , ");
    SerialUSB.println(PointToInt(gw.Position.Longitude));*/

    return IsWithinRadius(PointToInt(pSat.Latitude), PointToInt(pSat.Longitude), PointToInt(gw.Position.Latitude), PointToInt(gw.Position.Longitude), sat.CONE_RADIUS);
}

Gateway *GetNextInsightGateway(Sat &sat,long timeOffset, RoutingTable &table)
{
    GPSPoint pSat = ComputeSatPositionAtTime(GetTime() + timeOffset);
    float pSatLat = PointToInt(pSat.Latitude);
    float pSatLon = PointToInt(pSat.Longitude);

    // to check ! and to optimize !
    Gateway* gws = new Gateway[MAXROUTINGTABLEGATEWAYSIZE * MAXNETWORKAGREGATION];

    int fi = 0;

    for(int i = 0; i < MAXNETWORKAGREGATION; i++){
        for(int j = 0; j < MAXROUTINGTABLEGATEWAYSIZE; j++){
            if (table.Gateways[i][j].GatewayID != -1)
            {
                if(IsWithinRadius(pSatLat, pSatLon, PointToInt(table.Gateways[i][j].Position.Latitude), PointToInt(table.Gateways[i][j].Position.Latitude), sat.CONE_RADIUS)){
                    gws[fi] = table.Gateways[i][j];
                    fi++;
                }
            }
        }
    }

    return gws;
}

// computes satellite position at a certain time (tsince : time since epoch)
GPSPoint ComputeSatPositionAtTime(long tsince){
    SGP4ATmega predict;
    SGP4ATmega::geodetic_t *latlon, zero= {0,0,0,0};

        predict.setElements(CurrentTLE);

        predict.setTime(tsince);
    
        latlon = &zero;
        predict.calc(predict.elements, latlon);

        GPSPoint point;
        IntToPoint(point.Latitude, (*latlon).lat);
        IntToPoint(point.Longitude, predict.toRegularLong((*latlon).lon));

        return point;
}

/*ROUTING TABLE MANAGEMENT*/
boolean IsGatewayDropped(Gateway &gw)
{
    if (gw.DropRatio < 0.5)
        return true;
    return false;
}

short GetNetworkId(Gateway &gw)
{
    return gw.Network->ID;
}
short GetNetworkId(byte DevADDR[4], RoutingTable &table)
{
    byte cp = DevADDR[0];
    bitWrite(cp, 0, 0);
    int i = 0;
    while (i < sizeof(table.Networks))
    {
        if (table.Networks[i].ID != -1 && table.Networks[i].NetworkPrefix == cp)
        {
            return table.Networks[i].ID;
        }
        i++;
    }
    return -2;
}

boolean AddGateway(Gateway &gw, LPGANNetwork &network, RoutingTable &table)
{
    
    int i = 0;
    while (i < sizeof(table.Gateways[network.StorageID]))
    {
        if (table.Gateways[network.StorageID][i].GatewayID == -1)
        {
            table.Gateways[network.StorageID][i] = gw;
            return true;
        }
        i++;
    }
    return false;
}
boolean DeleteGateway(Gateway &gw, LPGANNetwork &network, RoutingTable &table)
{
   
    boolean found = false;
    int i = 0;
    while (i < sizeof(table.Gateways[network.StorageID]) && !found)
    {
        if (table.Gateways[network.StorageID][i].GatewayID != -1 && table.Gateways[network.StorageID][i].GatewayID == gw.GatewayID)
        {
            table.Gateways[network.StorageID][i]= {};
            return true;
        }
        i++;
    }
    return false;
}

boolean AddMote(Mote &m, LPGANNetwork &network, RoutingTable &table)
{
    int i = 0;
    while (i < sizeof(table.Motes[network.StorageID]))
    {
        if (table.Motes[network.StorageID][i].MoteID == -1)
        {
            table.Motes[network.StorageID][i] = m;
            return true;
        }
        i++;
    }
    return false;
}

boolean DeleteMote(Mote &m, LPGANNetwork &network, RoutingTable &table)
{
    boolean found = false;
    int i = 0;
    while (i < sizeof(table.Motes[network.StorageID]) && !found)
    {
        if (table.Motes[network.StorageID][i].MoteID != -1 && table.Motes[network.StorageID][i].DevADDR == m.DevADDR)
        {
            table.Motes[network.StorageID][i]= {};
            return true;
        }
        i++;
    }
    return false;
}

//Getter, return nullptr if not found

Gateway &foundGatewayByEUI(byte GatewayEUI[8], RoutingTable &table, Gateway &temp)
{
    
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        for (int j = 0; j < MAXROUTINGTABLEGATEWAYSIZE; j++)
        {
            if (table.Gateways[i][j].GatewayID != -1 && table.Gateways[i][j].GatewayEUI == GatewayEUI)
            {
               temp = table.Gateways[i][j];
            }
        }
    }

    return temp;
}

Gateway &foundGatewayByID(short GatewayID, RoutingTable &table, Gateway &temp)
{
    
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        for (int j = 0; j < MAXROUTINGTABLEGATEWAYSIZE; j++)
        {
            if (table.Gateways[i][j].GatewayID != -1 && table.Gateways[i][j].GatewayID == GatewayID)
            {
                temp= table.Gateways[i][j];
            }
        }
    }

    return temp;
}

Mote &foundMoteByDEVADDR(byte DEVADDR[4], RoutingTable &table, Mote &temp)
{
    
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        for (int j = 0; j < MAXROUTINGTABLEMOTESIZE; j++)
        {
            if (table.Motes[i][j].MoteID != -1 && table.Motes[i][j].DevADDR == DEVADDR)
            {
                temp= table.Motes[i][j];
            }
        }
    }

    return temp;
}

Mote &foundMoteByID(short MoteID, RoutingTable &table,Mote &temp)
{
   
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        for (int j = 0; j < MAXROUTINGTABLEMOTESIZE; j++)
        {
            if (table.Motes[i][j].MoteID != -1 && table.Motes[i][j].MoteID == MoteID)
            {
                temp = table.Motes[i][j];
            }
        }
    }

    return temp;
}

LPGANNetwork &foundNetworkByID(short NetID, RoutingTable &table, LPGANNetwork &temp)
{
    
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        if (table.Networks[i].ID != -1 && table.Networks[i].ID == NetID)
            temp = table.Networks[i];
    }

    return temp;
}

LPGANNetwork &foundNetworkByPrefix(byte Prefix, RoutingTable &table, LPGANNetwork &temp)
{
    
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        if (table.Networks[i].ID != -1 && table.Networks[i].NetworkPrefix == Prefix)
            temp= table.Networks[i];
    }

    return temp;
}

/* MESSAGE TREATMENT*/

boolean ParseACK(Message &m, int acks[2])
{
    acks[0] = (int)(m.AckRange[0]);
    acks[1] = (int)(m.AckRange[1]);

    if ((acks[0] < 0 || acks[1] < 0) || (acks[1] < acks[0]))
    {
        return false;
    }

    return true;
}

boolean ParseType(Message &m, MessageType &t)
{
    switch (m.Type)
    {
    case 0:
        t = Standard;
        return true;
        break;
    case 1:
        t = ACK;
        return true;
        break;
    case 2:
        t = Inscription;
        return true;
        break;
    case 3:
        t = GlobalDif;
        return true;
        break;
    case 4:
        t = SatSync;
        return true;
        break;
    case 5:
        t = SatACK;
        return true;
        break;    
    default:
        return false;
    }
}
boolean DeleteStorageMessage(Message &m, Message buffer[])
{
    Message temp;
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (m.MessageID == buffer[i].MessageID)
        {
            buffer[i] = {};
            return true;
        }
    }
    return false;
}
boolean AddStorageMessage(Message &m, Message buffer[])
{
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i].MessageID == -1)
        {
            buffer[i] = m;
            return true;
        }
    }
    return false;
}

/* DIFF FUNCTIONS */
boolean ApplyDiff(Diff &d,LPGANNetwork &Network, RoutingTable &table){
    /*Check Diff number*/
    if(d.DiffNumber <= table.NetworksLocalDif[Network.StorageID])
        return false;

    return true;
}

// it's mocking time
long GetTime(Sat &sat){
    return sat.clock;
}

void SetTime(Sat &sat,long t){
    sat.clock = t;
}