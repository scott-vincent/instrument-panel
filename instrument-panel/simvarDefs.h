#ifndef _SIMVARDEFS_H_
#define _SIMVARDEFS_H_

#include <stdio.h>

struct SimVars
{
    double connected = 0;
    double kohlsmann = 29.92;
    double altitude = 0;
    double latitude = 123.45;
    double longitude = 234.56;
    double adiPitch = 0;
    double adiBank = 0;
    double adiCal = 0;
    double altPressure1 = 16208;
    double altPressure2 = 16208;
    double altUnits = 2;
    double altAltitude1 = 0;
    double altAltitude2 = 0;
    double asiAirspeed = 0;
    double asiAirspeedCal = 0;
    double asiMachSpeed = 0;
    double hiHeading = 0;
    double hiHeadingBug = 0;
    double tcTurn = 0;
    double tcSlip = 0;
    double tfTrim = 0;
    double tfFlaps = 0;
    double vsiVerticalSpeed = 0;
    char title[256] = "A title";
};

#endif // _SIMVARDEFS_H_
