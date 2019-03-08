#include <Sodaq_RN2483.h>
#include "factory.h"

// GPS Point Manipulation function
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

/*ROUTING TABLE MANAGEMENT*/
boolean IsGatewayDropped(Gateway *gw)
{
    if (gw->DropRatio < 0.5)
        return true;
    return false;
}

int GetNetworkId(Gateway *gw)
{
    if (gw == nullptr)
        return -1;
    return gw->Network->ID;
}
int GetNetworkId(byte DevADDR[4], RoutingTable *table)
{
    byte cp = DevADDR[0];
    bitWrite(cp, 0, 0);
    if (table == nullptr)
        return -1;
    int i = 0;
    while (i < sizeof(table->Networks))
    {
        if (table->Networks[i] != nullptr && table->Networks[i]->NetworkPrefix == cp)
        {
            return table->Networks[i]->ID;
        }
        i++;
    }
    return -2;
}

boolean IsInSightOf(Gateway gw)
{
    return false;
}
Gateway *GetNextInsightGateway()
{
    return nullptr;
}

boolean AddGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table)
{
    if (gw == nullptr || table == nullptr)
        return false;
    int i = 0;
    while (i < sizeof(table->Gateways[network->ID]))
    {
        if (table->Gateways[network->ID][i] == nullptr)
        {
            table->Gateways[network->ID][i] = gw;
            return true;
        }
        i++;
    }
    return false;
}
boolean DeleteGateway(Gateway *gw, LPGANNetwork *network, RoutingTable *table)
{
    if (gw == nullptr || table == nullptr)
        return false;
    boolean found = false;
    int i = 0;
    while (i < sizeof(table->Gateways[network->ID]) && !found)
    {
        if (table->Gateways[network->ID][i] != nullptr && table->Gateways[network->ID][i]->GatewayEUI == gw->GatewayEUI)
        {
            table->Gateways[network->ID][i] = nullptr;
            return true;
        }
        i++;
    }
    return false;
}
