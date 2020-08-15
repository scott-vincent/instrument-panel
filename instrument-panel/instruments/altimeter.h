#ifndef _ALTIMETER_H_
#define _ALTIMETER_H_

#include "instrument.h"

class altimeter : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    unsigned short pressure;
    unsigned short pressure2;
    short units;
    long altitude1;
    double altitude2;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    char hpa[5];
    char inhg[5];
    char stringAltitude[6];
    long altitude = 0;
    long altitudeTarget;
    float angle = 0;
    int var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15;
    int var16, var17, var18, var19, var20, var21, var22, var23, var24, var25, var26, var27, var28, var29;
    int var30, var31, var32, var33, var34, var35, var36, var37;

public:
    altimeter(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
};

#endif // _ALTIMETER_H
