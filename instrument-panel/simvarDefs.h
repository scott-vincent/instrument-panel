#ifndef _SIMVARDEFS_H_
#define _SIMVARDEFS_H_

#include <stdio.h>

struct SimVars
{
    double connected = 0;

    // All Jetbridge vars must come first
    double apuMasterSw = 0;
    double apuBleed = 0;
    double elecBat1 = 0;
    double elecBat2 = 0;
    double jbManagedSpeed = 0;
    double jbManagedHeading = 0;
    double jbManagedAltitude = 0;
    double jbLateralMode = 0;
    double jbVerticalMode = 0;
    double jbLocMode = 0;
    double jbApprMode = 0;
    double jbAutothrustMode = 0;
    double jbShowMach = 0;
    double jbAutobrake = 0;
    double jbPitchTrim = 0;
    double jbTcasMode = 0;
    double sbEncoder[4];    // SwitchBox
    double sbButton[7];     // SwitchBox
    double sbMode;          // SwitchBox

    // Vars required for all panels (screensaver, aircraft identification etc.)
    char aircraft[32] = "\0";
    double cruiseSpeed = 120;
    double dcVolts = 23.7;
    double batteryLoad = 0;

    // Vars for Power/Lights panel
    double lightStates = 0;
    double tfFlapsCount = 1;
    double tfFlapsIndex = 0;
    double parkingBrakeOn = 1;
    double pushbackState = 3;
    double apuStartSwitch = 0;
    double apuPercentRpm = 0;

    // Vars for Radio panel
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
    double com1Receive = 1;
    double com2Receive = 0;
    double adfFreq = 394;
    double adfStandby = 368;
    double com1Volume = 0;
    double com2Volume = 0;
    double seatBeltsSwitch = 0;
    double transponderState = 0;
    double transponderCode = 4608;
    // No vars after here required by Radio panel

    // Vars for Autopilot panel
    double altAltitude = 0;
    double asiAirspeed = 0;
    double asiMachSpeed = 0;
    double hiHeading = 0;
    double vsiVerticalSpeed = 0;
    double autopilotAvailable = 1;
    double autopilotEngaged = 0;
    double flightDirectorActive = 0;
    double autopilotHeading = 0;
    double autopilotHeadingLock = 0;
    double autopilotHeadingSlotIndex = 1;
    double autopilotLevel = 0;
    double autopilotAltitude = 0;
    double autopilotAltitude3 = 0;
    double autopilotAltLock = 0;
    double autopilotPitchHold = 0;
    double autopilotVerticalSpeed = 0;
    double autopilotVerticalHold = 0;
    double autopilotVsSlotIndex = 1;
    double autopilotAirspeed = 0;
    double autopilotMach = 0;
    double autopilotAirspeedHold = 0;
    double autopilotApproachHold = 0;
    double autopilotGlideslopeHold = 0;
    double throttlePosition = 0;
    double autothrottleActive = 0;
    // No vars after here required by Autopilot panel

    double altKollsman = 29.92;
    double adiPitch = 0;
    double adiBank = 0;
    double asiTrueSpeed = 0;
    double asiAirspeedCal = -14;
    double hiHeadingTrue = 0;
    double altAboveGround = 0;
    double tcRate = 0;
    double tcBall = 0;
    double tfElevatorTrim = 0;
    double tfRudderTrim = 0;
    double tfSpoilersPosition = 0;
    double tfAutoBrake = 0;
    double dcUtcSeconds = 43200;
    double dcLocalSeconds = 46800;
    double dcFlightSeconds = 0;
    double dcTempC = 26.2;
    double numberOfEngines = 1;
    double rpmEngine = 0;
    double rpmPercent = 0;
    double rpmElapsedTime = 0;
    double fuelCapacity = 50;
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
    double gearRetractable = 1;
    double gearLeftPos = 100;
    double gearCentrePos = 100;
    double gearRightPos = 100;
    double rudderPosition = 0;
    double brakeLeftPedal = 0;
    double brakeRightPedal = 0;
    double oilTemp1 = 0;
    double oilTemp2 = 0;
    double oilTemp3 = 0;
    double oilTemp4 = 0;
    double oilPressure1 = 0;
    double oilPressure2 = 0;
    double oilPressure3 = 0;
    double oilPressure4 = 0;
    double exhaustGasTemp1 = 0;
    double exhaustGasTemp2 = 0;
    double exhaustGasTemp3 = 0;
    double exhaustGasTemp4 = 0;
    double engineType = 0;
    double engineMaxRpm = 0;
    double turbineEngine1N1 = 0;
    double turbineEngine2N1 = 0;
    double turbineEngine3N1 = 0;
    double turbineEngine4N1 = 0;
    double propRpm = 0;
    double engineManifoldPressure = 0;
    double engineFuelFlow1 = 0;
    double engineFuelFlow2 = 0;
    double engineFuelFlow3 = 0;
    double engineFuelFlow4 = 0;
    double suctionPressure = 1;
    double onGround = 0;
    double gForce = 0;
    char atcTailNumber[32] = "\0";
    char atcCallSign[32] = "\0";
    char atcFlightNumber[32] = "\0";
    double atcHeavy = 0;
    double landingRate = -999;
    double skytrackState = 0;
};

enum EVENT_ID {
    SIM_START,
    SIM_STOP,
    KEY_CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE,
    KEY_TRUE_AIRSPEED_CAL_SET,
    KEY_KOHLSMAN_SET,
    KEY_HEADING_GYRO_SET,
    KEY_VOR1_SET,
    KEY_VOR2_SET,
    KEY_ELEV_TRIM_UP,
    KEY_ELEV_TRIM_DN,
    KEY_FLAPS_INCR,
    KEY_FLAPS_DECR,
    KEY_FLAPS_UP,
    KEY_FLAPS_SET,
    KEY_FLAPS_DOWN,
    KEY_SPOILERS_ARM_SET,
    KEY_SPOILERS_OFF,
    KEY_SPOILERS_SET,
    KEY_SPOILERS_ON,
    KEY_GEAR_SET,
    KEY_ADF_CARD_SET,
    KEY_COM1_VOLUME_SET,
    KEY_COM1_TRANSMIT_SELECT,
    KEY_COM1_RECEIVE_SELECT,
    KEY_COM1_STBY_RADIO_SET,
    KEY_COM1_RADIO_FRACT_INC,
    KEY_COM1_RADIO_SWAP,
    KEY_COM2_VOLUME_SET,
    KEY_COM2_TRANSMIT_SELECT,
    KEY_COM2_RECEIVE_SELECT,
    KEY_COM2_STBY_RADIO_SET,
    KEY_COM2_RADIO_FRACT_INC,
    KEY_COM2_RADIO_SWAP,
    KEY_NAV1_STBY_SET,
    KEY_NAV1_RADIO_SWAP,
    KEY_NAV2_STBY_SET,
    KEY_NAV2_RADIO_SWAP,
    KEY_ADF_STBY_SET,
    KEY_ADF1_RADIO_SWAP,
    KEY_RADIO_VOR1_IDENT_SET,
    KEY_RADIO_VOR2_IDENT_SET,
    KEY_RADIO_ADF_IDENT_SET,
    KEY_XPNDR_SET,
    KEY_XPNDR_HIGH_SET,
    KEY_XPNDR_LOW_SET,
    KEY_XPNDR_STATE,
    KEY_AP_MASTER,
    KEY_TOGGLE_FLIGHT_DIRECTOR,
    KEY_AP_SPD_VAR_SET,
    KEY_AP_MACH_VAR_SET,
    KEY_HEADING_BUG_SET,            // 50
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
    KEY_AP_HEADING_SLOT_INDEX_SET,
    KEY_AP_SPEED_SLOT_INDEX_SET,
    KEY_AP_VS_SLOT_INDEX_SET,
    KEY_AP_VS_SET,
    KEY_AP_PANEL_HEADING_SET,
    KEY_TUG_HEADING,
    KEY_ELEC_BAT1,
    KEY_ELEC_BAT2,
    KEY_TOGGLE_MASTER_BATTERY,
    KEY_TOGGLE_MASTER_ALTERNATOR,
    KEY_TOGGLE_JETWAY,
    KEY_TOGGLE_RAMPTRUCK,
    KEY_FUEL_PUMP,
    KEY_CABIN_LIGHTS_SET,
    KEY_BEACON_LIGHTS_SET,
    KEY_LANDING_LIGHTS_SET,
    KEY_TAXI_LIGHTS_SET,
    KEY_NAV_LIGHTS_SET,
    KEY_STROBES_SET,
    KEY_PITOT_HEAT_SET,
    KEY_ANTI_ICE_SET,
    KEY_WINDSHIELD_DEICE_SET,
    KEY_AVIONICS_MASTER_SET,
    KEY_APU_STARTER,
    KEY_APU_OFF_SWITCH,
    KEY_BLEED_AIR_SOURCE_CONTROL_SET,
    KEY_TOGGLE_PUSHBACK,
    KEY_PARKING_BRAKE_SET,
    KEY_AUTOBRAKE,
    KEY_TANK_SELECT_1,
    KEY_TANK_SELECT_2,
    KEY_ENG_CRANK,
    KEY_SKYTRACK_STATE,             // 98
    KEY_G1000_PFD_SOFTKEY_1,
    KEY_G1000_PFD_SOFTKEY_2,
    KEY_G1000_PFD_SOFTKEY_3,
    KEY_G1000_PFD_SOFTKEY_4,
    KEY_G1000_PFD_SOFTKEY_5,
    KEY_G1000_PFD_SOFTKEY_6,
    KEY_G1000_PFD_SOFTKEY_7,
    KEY_G1000_PFD_SOFTKEY_8,
    KEY_G1000_PFD_SOFTKEY_9,
    KEY_G1000_PFD_SOFTKEY_10,
    KEY_G1000_PFD_SOFTKEY_11,
    KEY_G1000_PFD_SOFTKEY_12,
    KEY_G1000_PFD_DIRECTTO,
    KEY_G1000_PFD_ENT,
    KEY_G1000_PFD_CLR_LONG,
    KEY_G1000_PFD_CLR,
    KEY_G1000_PFD_MENU,
    KEY_G1000_PFD_FPL,
    KEY_G1000_PFD_PROC,
    KEY_G1000_PFD_UPPER_INC,
    KEY_G1000_PFD_UPPER_DEC,
    KEY_G1000_PFD_PUSH,
    KEY_G1000_PFD_LOWER_INC,
    KEY_G1000_PFD_LOWER_DEC,
    KEY_G1000_MFD_SOFTKEY_1,
    KEY_G1000_MFD_SOFTKEY_2,
    KEY_G1000_MFD_SOFTKEY_3,
    KEY_G1000_MFD_SOFTKEY_4,
    KEY_G1000_MFD_SOFTKEY_5,
    KEY_G1000_MFD_SOFTKEY_6,
    KEY_G1000_MFD_SOFTKEY_7,
    KEY_G1000_MFD_SOFTKEY_8,
    KEY_G1000_MFD_SOFTKEY_9,
    KEY_G1000_MFD_SOFTKEY_10,
    KEY_G1000_MFD_SOFTKEY_11,
    KEY_G1000_MFD_SOFTKEY_12,
    KEY_G1000_MFD_DIRECTTO,
    KEY_G1000_MFD_ENT,
    KEY_G1000_MFD_CLR_LONG,
    KEY_G1000_MFD_CLR,
    KEY_G1000_MFD_MENU,
    KEY_G1000_MFD_FPL,
    KEY_G1000_MFD_PROC,
    KEY_G1000_MFD_UPPER_INC,
    KEY_G1000_MFD_UPPER_DEC,
    KEY_G1000_MFD_PUSH,
    KEY_G1000_MFD_LOWER_INC,
    KEY_G1000_MFD_LOWER_DEC,
    KEY_G1000_MFD_RANGE_INC,
    KEY_G1000_MFD_RANGE_DEC,
    KEY_G1000_END,      // Must be last G1000 key
    A32NX_FCU_SPD_PUSH,
    A32NX_FCU_SPD_PULL,
    A32NX_FCU_SPD_SET,
    A32NX_FCU_HDG_PUSH,
    A32NX_FCU_HDG_PULL,
    A32NX_FCU_HDG_SET,
    A32NX_FCU_ALT_PUSH,
    A32NX_FCU_ALT_PULL,
    A32NX_FCU_VS_PUSH,
    A32NX_FCU_VS_PULL,
    A32NX_FCU_VS_SET,
    A32NX_FCU_SPD_MACH_TOGGLE_PUSH,
    A32NX_FCU_APPR_PUSH,
    A32NX_FCU_TRK_FPA_TOGGLE_PUSH,
    KEY_CHECK_EVENT,
    EVENT_NONE,
    EVENT_DOORS_TO_MANUAL,
    EVENT_DOORS_FOR_DISEMBARK,
    EVENT_DOORS_TO_AUTO,
    EVENT_DOORS_FOR_BOARDING,
    EVENT_WELCOME_ON_BOARD,
    EVENT_BOARDING_COMPLETE,
    EVENT_PUSHBACK_START,
    EVENT_PUSHBACK_STOP,
    EVENT_SEATS_FOR_TAKEOFF,
    EVENT_START_SERVING,
    EVENT_FINAL_DESCENT,
    EVENT_REMAIN_SEATED,
    EVENT_DISEMBARK,
    EVENT_TURBULENCE,
    EVENT_REACHED_CRUISE,
    EVENT_REACHED_TOD,
    EVENT_LANDING_PREPARE_CABIN,
    EVENT_SEATS_FOR_LANDING,
    EVENT_GO_AROUND,
    EVENT_RESET_DRONE_FOV,
    VJOY_BUTTONS,
    // Buttons must start from 1 and must be sequential until VJOY_BUTTONS_END
    VJOY_BUTTON_1,
    VJOY_BUTTON_2,
    VJOY_BUTTON_3,
    VJOY_BUTTON_4,
    VJOY_BUTTON_5,
    VJOY_BUTTON_6,
    VJOY_BUTTON_7,
    VJOY_BUTTON_8,
    VJOY_BUTTON_9,
    VJOY_BUTTON_10,
    VJOY_BUTTON_11,
    VJOY_BUTTON_12,
    VJOY_BUTTON_13,
    VJOY_BUTTON_14,
    VJOY_BUTTON_15,
    VJOY_BUTTON_16,
    VJOY_BUTTONS_END,
};

struct WriteEvent {
    EVENT_ID id;
    const char* name;
};

struct WriteData {
    EVENT_ID eventId;
    double value;
};

struct PosData {
    double lat;
    double lon;
    double heading;
};

struct Request {
    int requestedSize;
    int wantFullData;
    WriteData writeData;
};

struct DeltaDouble {
    int offset;
    double data;
};

struct DeltaString {
    int offset;
    char data[32];
};

#endif // _SIMVARDEFS_H_
