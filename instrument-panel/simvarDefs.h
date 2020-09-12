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
    double tfFlapsCount = 1;
    double tfFlapsIndex = 0;
    double dcUtcSeconds = 43200;
    double dcLocalSeconds = 46800;
    double dcFlightSeconds = 0;
    double dcVolts = 23.7;
    double dcTempC = 26.2;
    double rpmEngine = 0;
    double rpmPercent = 0;
    double rpmElapsedTime = 0;
    double fuelLeft = 0;
    double fuelRight = 0;
    double vor1Obs = 0;
    double vor1RadialError = 0;
    double vor1GlideSlopeError = 0;
    double vor1ToFrom = 0;
    double vor1GlideSlopeFlag = 0;
    double vor2Obs = 0;
    double vor2RadialError = 0;
    double vor2ToFrom = 0;
    double adfRadial = 0;
    double adfCard = 0;
    double com1Freq = 119.225;
    double com1Standby = 124.850;
    double nav1Freq = 110.50;
    double nav1Standby = 113.90;
    double com2Freq = 124.850;
    double com2Standby = 124.850;
    double nav2Freq = 110.50;
    double nav2Standby = 113.90;
    double adfFreq = 394;
    double adfStandby = 368;
    char atcTailNumber[64] = "\0";
    char atcCallSign[64] = "\0";
    char atcFlightNumber[8] = "\0";
    double atcHeavy = 0;
    char aircraft[256] = "\0";
};

enum DEFINITION_ID {
    DEF_READ_ALL,
    DEF_WRITE_TRIM_FLAPS
};

struct WriteDef {
    DEFINITION_ID id;
    const char* name;
};

struct TrimFlapsData {
    double tfElevatorTrim;
    double tfFlapsIndex;
};

#endif // _SIMVARDEFS_H_
