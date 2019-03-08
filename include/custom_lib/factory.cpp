#include <Sodaq_RN2483.h>
#include "factory.h"

// GPS Point Manipulation function 
void IntToPoint(byte point[], float value){

    float integ;
    float frac = modff(value, &integ);
    int i = floor(abs(integ));
    int f = floor(abs(frac)*100);

    byte _i = lowByte(i);
    *point = _i << 1;
    if(value < 0){
        bitWrite(*point, 0, 1);
    } else {
        bitWrite(*point, 0, 0);
    }

    byte _f = lowByte(f);
    *(point+1) = _f << 1;
    bitWrite(*(point+1), 0, bitRead(_i, 7));
}

float PointToInt(byte point[]){
    SerialUSB.println();
   
    bool sign = bitRead(*point, 0);

    byte i = *point >> 1;
    byte f = *(point+1);
    //byte f = *(point+1) & 127; // not working - need to set bit manually
    bitWrite(i, 7, bitRead(f, 0));
  
    float nv = (float)i;
    nv += ((f >> 1) * 0.01);
    if(sign){
        return -nv;
    }

    return nv;
}

