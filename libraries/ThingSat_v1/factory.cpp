#include <Sodaq_RN2483.h>
#include "Arduino.h"
#include "factory.h"

/* GPS POINT MANIPULATION FUNCTION */
void IntToPoint(byte point[2], float value)
{

    float integ;
    float frac = modff(value, &integ);
    int i = floor(abs(integ));
    int f = floor(abs(frac) * 100);

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

    byte _f = lowByte(f);
    *(point + 1) = _f << 1;
    bitWrite(*(point + 1), 0, bitRead(_i, 7));
}

float PointToInt(byte point[2])
{
    SerialUSB.println();

    bool sign = bitRead(*point, 0);

    byte i = *point >> 1;
    byte f = *(point + 1);
    //byte f = *(point+1) & 127; // not working - need to set bit manually
    bitWrite(i, 7, bitRead(f, 0));

    float nv = (float)i;
    nv += ((f >> 1) * 0.01);
    if (sign)
    {
        return -nv;
    }

    return nv;
}

/* GPS FUNCTION */

boolean IsInSightOf(Gateway gw)
{
    return false;
}
Gateway *GetNextInsightGateway()
{
    return nullptr;
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
            table.Gateways[network.StorageID][i].GatewayID = -1;
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
            table.Motes[network.StorageID][i].MoteID = -1;
            return true;
        }
        i++;
    }
    return false;
}

//Getter, return nullptr if not found

Gateway &foundGatewayByEUI(byte GatewayEUI[8], RoutingTable &table)
{
    Gateway temp;
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

Gateway &foundGatewayByID(short GatewayID, RoutingTable &table)
{
    Gateway temp;
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

Mote &foundMoteByDEVADDR(byte DEVADDR[4], RoutingTable &table)
{
    Mote temp;
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

Mote &foundMoteByID(short MoteID, RoutingTable &table)
{
    Mote temp;
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

LPGANNetwork &foundNetworkByID(short NetID, RoutingTable &table)
{
    LPGANNetwork temp;
    for (int i = 0; i < MAXNETWORKAGREGATION; i++)
    {
        if (table.Networks[i].ID != -1 && table.Networks[i].ID == NetID)
            temp = table.Networks[i];
    }

    return temp;
}

LPGANNetwork &foundNetworkByPrefix(byte Prefix, RoutingTable &table)
{
    LPGANNetwork temp;
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