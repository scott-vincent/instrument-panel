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
    double asiTrueSpeed = 0;
    double asiMachSpeed = 0;
    double asiAirspeedCal = -14;
    double asiMaxMach = 0;
    double hiHeading = 0;
    double hiHeadingTrue = 0;
    double vsiVerticalSpeed = 0;
    double altAboveGround = 0;
    double tcRate = 0;
    double tcBall = 0;
    double tfElevatorTrim = 0;
    double tfFlapsCount = 1;
    double tfFlapsIndex = 0;
    double tfSpoilersPosition = 0;
    double tfAutoBrake = 0;
    double dcUtcSeconds = 43200;
    double dcLocalSeconds = 46800;
    double dcFlightSeconds = 0;
    double dcVolts = 23.7;
    double dcTempC = 26.2;
    double batteryLoad = 0;
    double rpmEngine = 0;
    double rpmPercent = 0;
    double rpmElapsedTime = 0;
    double fuelCapacity = 0;
    double fuelQuantity = 0;
    double fuelLeftPercent = 0;
    double fuelRightPercent = 0;
    double vor1Obs = 0;
    double vor1RadialError = 0;
    double vor1GlideSlopeError = 0;
    double vor1ToFrom = 0;
    double vor1GlideSlopeFlag = 0;
    double vor2Obs = 0;
    double vor2RadialError = 0;
    double vor2ToFrom = 0;
    double navHasLocalizer = 0;
    double navLocalizer = 0;
    double gpsDrivesNav1 = 0;
    double gpsWpCrossTrk = 0;
    double adfRadial = 0;
    double adfCard = 0;
    double com1Status = 0;
    double com1Transmit = 1;
    double com1Freq = 119.225;
    double com1Standby = 124.850;
    double nav1Freq = 110.50;
    double nav1Standby = 113.90;
    double com2Status = 0;
    double com2Transmit = 0;
    double com2Freq = 124.850;
    double com2Standby = 124.850;
    double nav2Freq = 110.50;
    double nav2Standby = 113.90;
    double adfFreq = 394;
    double adfStandby = 368;
    double transponderCode = 4608;
    double autopilotAvailable = 1;
    double autopilotEngaged = 0;
    double flightDirectorActive = 0;
    double autopilotHeading = 0;
    double autopilotHeadingLock = 0;
    double autopilotLevel = 0;
    double autopilotAltitude = 0;
    double autopilotAltLock = 0;
    double autopilotPitchHold = 0;
    double autopilotVerticalSpeed = 0;
    double autopilotVerticalHold = 0;
    double autopilotAirspeed = 0;
    double autopilotMach = 0;
    double autopilotAirspeedHold = 0;
    double autopilotApproachHold = 0;
    double autopilotGlideslopeHold = 0;
    double throttlePosition = 0;
    double autothrottleActive = 0;
    double gearRetractable = 1;
    double gearLeftPos = 100;
    double gearCentrePos = 100;
    double gearRightPos = 100;
    double parkingBrakeOn = 1;
    double pushbackState = 3;
    double rudderPosition = 0;
    double cruiseSpeed = 120;
    double oilTemp = 75;
    double oilPressure = 50;
    double exhaustGasTemp = 0;
    double engineFuelFlow = 0;
    double suctionPressure = 1;
    char atcTailNumber[64] = "\0";
    char atcCallSign[64] = "\0";
    char atcFlightNumber[8] = "\0";
    double atcHeavy = 0;
    char aircraft[256] = "\0";
};

enum EVENT_ID {
    SIM_START,
    SIM_STOP,
    KEY_TRUE_AIRSPEED_CAL_SET,
    KEY_KOHLSMAN_SET,
    KEY_VOR1_SET,
    KEY_VOR2_SET,
    KEY_ELEV_TRIM_UP,
    KEY_ELEV_TRIM_DN,
    KEY_FLAPS_INCR,
    KEY_FLAPS_DECR,
    KEY_ADF_CARD_SET,
    KEY_COM1_TRANSMIT_SELECT,
    KEY_COM1_STBY_RADIO_SET,
    KEY_COM1_RADIO_FRACT_INC,
    KEY_COM1_RADIO_SWAP,
    KEY_COM2_TRANSMIT_SELECT,
    KEY_COM2_STBY_RADIO_SET,
    KEY_COM2_RADIO_FRACT_INC,
    KEY_COM2_RADIO_SWAP,
    KEY_NAV1_STBY_SET,
    KEY_NAV1_RADIO_SWAP,
    KEY_NAV2_STBY_SET,
    KEY_NAV2_RADIO_SWAP,
    KEY_ADF_COMPLETE_SET,
    KEY_ADF1_PRIMARY_SET,
    KEY_XPNDR_SET,
    KEY_AP_MASTER,
    KEY_TOGGLE_FLIGHT_DIRECTOR,
    KEY_AP_SPD_VAR_SET,
    KEY_AP_MACH_VAR_SET,
    KEY_HEADING_BUG_SET,
    KEY_AP_ALT_VAR_SET_ENGLISH,
    KEY_AP_VS_VAR_SET_ENGLISH,
    KEY_AP_AIRSPEED_ON,
    KEY_AP_AIRSPEED_OFF,
    KEY_AP_MACH_ON,
    KEY_AP_MACH_OFF,
    KEY_AP_HDG_HOLD_ON,
    KEY_AP_HDG_HOLD_OFF,
    KEY_AP_ALT_HOLD_ON,
    KEY_AP_ALT_HOLD_OFF,
    KEY_AP_LOC_HOLD,
    KEY_AP_APR_HOLD_ON,
    KEY_AP_APR_HOLD_OFF,
    KEY_AP_PANEL_ALTITUDE_ON,
    KEY_AUTO_THROTTLE_ARM,
    KEY_HEADING_SLOT_INDEX_SET,
    KEY_SPEED_SLOT_INDEX_SET,
    KEY_ALTITUDE_SLOT_INDEX_SET,
    KEY_TUG_HEADING
};

struct WriteEvent {
    EVENT_ID id;
    const char* name;
};

struct WriteData {
    EVENT_ID eventId;
    double value;
};

#endif // _SIMVARDEFS_H_
