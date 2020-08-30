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
    double asiMachSpeed = 0;
    double asiAirspeedCal = 0;
    double hiHeading = 0;
    double vsiVerticalSpeed = 0;
    double tcRate = 0;
    double tcBall = 0;
    double tfElevatorTrim = 0;
    double tfFlapsCount = 0;
    double tfFlapsIndex = 0;
    double utcSeconds = 43200;
    double localSeconds = 46800;
    double flightSeconds = 0;
    double busVoltage = 23.7;
    double tempC = 26.2;
    double adiCal = 0;
    double hiHeadingBug = 0;
};

#endif // _SIMVARDEFS_H_
