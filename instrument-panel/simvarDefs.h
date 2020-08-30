#ifndef _SIMVARDEFS_H_
#define _SIMVARDEFS_H_

#include <stdio.h>

struct SimVars
{
    double connected = 0;
    double altAltitude = 0;
    double altKollsman = 29.92;
    double adiPitch = 0;
    double adiBank = 0;
    double asiAirspeed = 0;
    double adiCal = 0;
    double asiAirspeedCal = 0;
    double asiMachSpeed = 0;
    double hiHeading = 0;
    double hiHeadingBug = 0;
    double tcTurn = 0;
    double tcSlip = 0;
    double tfTrim = 0;
    double tfFlaps = 0;
    double vsiVerticalSpeed = 0;
    //char title[256] = "A title";
};

#endif // _SIMVARDEFS_H_
