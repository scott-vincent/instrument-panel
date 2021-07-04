#include <stdio.h>
#include "simvarDefs.h"

const char* versionString = "v1.5.4";

const char* SimVarDefs[][2] = {
    // Vars for Jetbridge (must come first)
    { "Apu Master Sw", "jetbridge" },
    { "Apu Start", "jetbridge" },
    { "Apu Start Avail", "jetbridge" },
    { "Apu Bleed", "jetbridge" },
    { "Elec Bat1", "jetbridge" },
    { "Elec Bat2", "jetbridge" },

    // Vars required for all panels (screensaver, aircraft identification etc.)
    { "Title", "string32" },
    { "Estimated Cruise Speed", "knots" },
    { "Electrical Main Bus Voltage", "volts" },

    // Vars for Power/Lights panel
    { "Apu Pct Rpm", "percent" },

    // Vars for Radio panel
    { "Com Status:1", "enum" },
    { "Com Transmit:1", "bool" },
    { "Com Active Frequency:1", "mhz" },
    { "Com Standby Frequency:1", "mhz" },
    { "Nav Active Frequency:1", "mhz" },
    { "Nav Standby Frequency:1", "mhz" },
    { "Com Status:2", "enum" },
    { "Com Transmit:2", "bool" },
    { "Com Active Frequency:2", "mhz" },
    { "Com Standby Frequency:2", "mhz" },
    { "Nav Active Frequency:2", "mhz" },
    { "Nav Standby Frequency:2", "mhz" },
    { "Adf Active Frequency:1", "khz" },
    { "Adf Standby Frequency:1", "khz" },
    { "Cabin Seatbelts Alert Switch", "bool" },
    { "Transponder State:1", "enum" },
    { "Transponder Code:1", "bco16" },

    // Vars for Autopilot panel
    { "Indicated Altitude", "feet" },
    { "Airspeed Indicated", "knots" },
    { "Airspeed Mach", "mach" },
    { "Plane Heading Degrees Magnetic", "degrees" },
    { "Vertical Speed", "feet per second" },
    { "Autopilot Available", "bool" },
    { "Autopilot Master", "bool" },
    { "Autopilot Flight Director Active", "bool" },
    { "Autopilot Heading Lock Dir", "degrees" },
    { "Autopilot Heading Lock", "bool" },
    { "Autopilot Wing Leveler", "bool" },
    { "Autopilot Altitude Lock Var", "feet" },
    { "Autopilot Altitude Lock", "bool" },
    { "Autopilot Pitch Hold", "bool" },
    { "Autopilot Vertical Hold Var", "feet/minute" },
    { "Autopilot Vertical Hold", "bool" },
    { "Autopilot Airspeed Hold Var", "knots" },
    { "Autopilot Mach Hold Var", "number" },
    { "Autopilot Airspeed Hold", "bool" },
    { "Autopilot Approach Hold", "bool" },
    { "Autopilot Glideslope Hold", "bool" },
    { "General Eng Throttle Lever Position:1", "percent" },
    { "Autothrottle Active", "bool" },

    // Remaining vars for Instrument panel
    { "Kohlsman Setting Hg", "inHg" },
    { "Attitude Indicator Pitch Degrees", "degrees" },
    { "Attitude Indicator Bank Degrees", "degrees" },
    { "Airspeed True", "knots" },
    { "Airspeed True Calibrate", "degrees" },
    { "Plane Heading Degrees True", "degrees" },
    { "Plane Alt Above Ground", "feet" },
    { "Turn Indicator Rate", "radians per second" },
    { "Turn Coordinator Ball", "position" },
    { "Elevator Trim Position", "degrees" },
    { "Rudder Trim Pct", "percent" },
    { "Flaps Num Handle Positions", "number" },
    { "Flaps Handle Index", "number" },
    { "Spoilers Handle Position", "percent" },
    { "Auto Brake Switch Cb", "number" },
    { "Zulu Time", "seconds" },
    { "Local Time", "seconds" },
    { "Absolute Time", "seconds" },
    { "Ambient Temperature", "celsius" },
    { "Electrical Battery Load", "amperes" },
    { "General Eng Rpm:1", "rpm" },
    { "Eng Rpm Animation Percent:1", "percent" },
    { "General Eng Elapsed Time:1", "hours" },
    { "Fuel Total Capacity", "gallons" },
    { "Fuel Total Quantity", "gallons" },
    { "Fuel Tank Left Main Level", "percent" },
    { "Fuel Tank Right Main Level", "percent" },
    { "Nav Obs:1", "degrees" },
    { "Nav Radial Error:1", "degrees" },
    { "Nav Glide Slope Error:1", "degrees" },
    { "Nav ToFrom:1", "enum" },
    { "Nav Gs Flag:1", "bool" },
    { "Nav Obs:2", "degrees" },
    { "Nav Radial Error:2", "degrees" },
    { "Nav ToFrom:2", "enum" },
    { "Nav Has Localizer:1", "bool" },
    { "Nav Localizer:1", "degrees" },
    { "Gps Drives Nav1", "bool" },
    { "Gps Wp Cross Trk", "meters" },
    { "Adf Radial:1", "degrees" },
    { "Adf Card", "degrees" },
    { "Is Gear Retractable", "bool" },
    { "Gear Left Position", "percent" },
    { "Gear Center Position", "percent" },
    { "Gear Right Position", "percent" },
    { "Brake Parking Position", "bool" },
    { "Pushback State", "enum" },
    { "Rudder Position", "position" },
    { "General Eng Oil Temperature:1", "fahrenheit" },
    { "General Eng Oil Pressure:1", "psi" },
    { "General Eng Exhaust Gas Temperature:1", "celsius" },
    { "Engine Type", "enum" },
    { "Max Rated Engine RPM", "rpm" },
    { "Turb Eng N1:1", "percent" },
    { "Prop RPM:1", "rpm" },
    { "Eng Manifold Pressure:1", "inches of mercury" },
    { "Eng Fuel Flow GPH:1", "gallons per hour" },
    { "Suction Pressure", "inches of mercury" },
    { "G Force", "gforce"},
    { "Atc Id", "string32" },
    { "Atc Airline", "string32" },
    { "Atc Flight Number", "string32" },
    { "Atc Heavy", "bool" },
    { NULL, NULL }
};

WriteEvent WriteEvents[] = {
    { KEY_TRUE_AIRSPEED_CAL_SET, "TRUE_AIRSPEED_CAL_SET" },
    { KEY_KOHLSMAN_SET, "KOHLSMAN_SET" },
    { KEY_VOR1_SET, "VOR1_SET" },
    { KEY_VOR2_SET, "VOR2_SET" },
    { KEY_ELEV_TRIM_UP, "ELEV_TRIM_UP" },
    { KEY_ELEV_TRIM_DN, "ELEV_TRIM_DN" },
    { KEY_FLAPS_INCR, "FLAPS_INCR" },
    { KEY_FLAPS_DECR, "FLAPS_DECR" },
    { KEY_FLAPS_UP, "FLAPS_UP" },
    { KEY_FLAPS_1, "FLAPS_1" },
    { KEY_FLAPS_2, "FLAPS_2" },
    { KEY_FLAPS_3, "FLAPS_3" },
    { KEY_FLAPS_DOWN, "FLAPS_DOWN" },
    { KEY_SPOILERS_ARM_SET, "SPOILERS_ARM_SET" },
    { KEY_SPOILERS_OFF, "SPOILERS_OFF" },
    { KEY_SPOILERS_SET, "SPOILERS_SET" },
    { KEY_SPOILERS_ON, "SPOILERS_ON" },
    { KEY_GEAR_SET, "GEAR_SET" },
    { KEY_ADF_CARD_SET, "ADF_CARD_SET" },
    { KEY_COM1_TRANSMIT_SELECT, "COM1_TRANSMIT_SELECT" },
    { KEY_COM1_STBY_RADIO_SET, "COM_STBY_RADIO_SET" },
    { KEY_COM1_RADIO_FRACT_INC, "COM_RADIO_FRACT_INC" },
    { KEY_COM1_RADIO_SWAP, "COM1_RADIO_SWAP" },
    { KEY_COM2_TRANSMIT_SELECT, "COM2_TRANSMIT_SELECT" },
    { KEY_COM2_STBY_RADIO_SET, "COM2_STBY_RADIO_SET" },
    { KEY_COM2_RADIO_FRACT_INC, "COM2_RADIO_FRACT_INC" },
    { KEY_COM2_RADIO_SWAP, "COM2_RADIO_SWAP" },
    { KEY_NAV1_STBY_SET, "NAV1_STBY_SET" },
    { KEY_NAV1_RADIO_SWAP, "NAV1_RADIO_SWAP" },
    { KEY_NAV2_STBY_SET, "NAV2_STBY_SET" },
    { KEY_NAV2_RADIO_SWAP, "NAV2_RADIO_SWAP" },
    { KEY_ADF_COMPLETE_SET, "ADF_COMPLETE_SET" },
    { KEY_ADF_SET, "ADF_SET" },
    { KEY_XPNDR_SET, "XPNDR_SET" },
    { KEY_AP_MASTER, "AP_MASTER" },
    { KEY_TOGGLE_FLIGHT_DIRECTOR, "TOGGLE_FLIGHT_DIRECTOR" },
    { KEY_AP_SPD_VAR_SET, "AP_SPD_VAR_SET" },
    { KEY_AP_MACH_VAR_SET, "AP_MACH_VAR_SET" },
    { KEY_HEADING_BUG_SET, "HEADING_BUG_SET" },
    { KEY_AP_ALT_VAR_SET_ENGLISH, "AP_ALT_VAR_SET_ENGLISH" },
    { KEY_AP_VS_VAR_SET_ENGLISH, "AP_VS_VAR_SET_ENGLISH" },
    { KEY_AP_AIRSPEED_ON, "AP_AIRSPEED_ON" },
    { KEY_AP_AIRSPEED_OFF, "AP_AIRSPEED_OFF" },
    { KEY_AP_MACH_ON, "AP_MACH_ON" },
    { KEY_AP_MACH_OFF, "AP_MACH_OFF" },
    { KEY_AP_HDG_HOLD_ON, "AP_HDG_HOLD_ON" },
    { KEY_AP_HDG_HOLD_OFF, "AP_HDG_HOLD_OFF" },
    { KEY_AP_ALT_HOLD_ON, "AP_ALT_HOLD_ON" },
    { KEY_AP_ALT_HOLD_OFF, "AP_ALT_HOLD_OFF" },
    { KEY_AP_LOC_HOLD, "AP_LOC_HOLD" },
    { KEY_AP_APR_HOLD_ON, "AP_APR_HOLD_ON" },
    { KEY_AP_APR_HOLD_OFF, "AP_APR_HOLD_OFF" },
    { KEY_AP_PANEL_ALTITUDE_ON, "AP_PANEL_ALTITUDE_ON" },
    { KEY_AUTO_THROTTLE_ARM, "AUTO_THROTTLE_ARM" },
    { KEY_HEADING_SLOT_INDEX_SET, "HEADING_SLOT_INDEX_SET" },
    { KEY_SPEED_SLOT_INDEX_SET, "SPEED_SLOT_INDEX_SET" },
    { KEY_ALTITUDE_SLOT_INDEX_SET, "ALTITUDE_SLOT_INDEX_SET" },
    { KEY_TUG_HEADING, "KEY_TUG_HEADING" },
    { KEY_ELEC_BAT1, "ELEC BAT1" },
    { KEY_ELEC_BAT2, "ELEC BAT2" },
    { KEY_TOGGLE_MASTER_BATTERY, "TOGGLE_MASTER_BATTERY" },
    { KEY_TOGGLE_MASTER_ALTERNATOR, "TOGGLE_MASTER_ALTERNATOR" },
    { KEY_TOGGLE_JETWAY, "TOGGLE_JETWAY" },
    { KEY_FUEL_PUMP, "FUEL_PUMP" },
    { KEY_BEACON_LIGHTS_SET, "BEACON_LIGHTS_SET" },
    { KEY_LANDING_LIGHTS_SET, "LANDING_LIGHTS_SET" },
    { KEY_TAXI_LIGHTS_SET, "TAXI_LIGHTS_SET" },
    { KEY_NAV_LIGHTS_SET, "NAV_LIGHTS_SET" },
    { KEY_STROBES_SET, "STROBES_SET" },
    { KEY_PITOT_HEAT_SET, "PITOT_HEAT_SET" },
    { KEY_ANTI_ICE_SET, "ANTI_ICE_SET" },
    { KEY_AVIONICS_MASTER_SET, "AVIONICS_MASTER_SET" },
    { KEY_APU_OFF_SWITCH, "APU_OFF_SWITCH" },
    { KEY_APU_STARTER, "APU_STARTER" },
    { KEY_BLEED_AIR_SOURCE_CONTROL_SET, "BLEED_AIR_SOURCE_CONTROL_SET" },
    { KEY_CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE, "CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE" },
    { KEY_TOGGLE_PUSHBACK, "TOGGLE_PUSHBACK" },
    { KEY_CHECK_EVENT, "CHECK_EVENT" },
    { EVENT_NONE, "EVENT_NONE" },
    { EVENT_DOORS_TO_MANUAL, "EVENT_DOORS_TO_MANUAL" },
    { EVENT_DOORS_FOR_DISEMBARK, "EVENT_DOORS_FOR_DISEMBARK" },
    { EVENT_DOORS_TO_AUTO, "EVENT_DOORS_TO_AUTO" },
    { EVENT_DOORS_FOR_BOARDING, "EVENT_DOORS_FOR_BOARDING" },
    { EVENT_WELCOME_ON_BOARD, "EVENT_WELCOME_ON_BOARD" },
    { EVENT_BOARDING_COMPLETE, "EVENT_BOARDING_COMPLETE" },
    { EVENT_PUSHBACK_START, "EVENT_PUSHBACK_START" },
    { EVENT_PUSHBACK_STOP, "EVENT_PUSHBACK_STOP" },
    { EVENT_SEATS_FOR_TAKEOFF, "EVENT_SEATS_FOR_TAKEOFF" },
    { EVENT_START_SERVING, "EVENT_START_SERVING" },
    { EVENT_FINAL_DESCENT, "EVENT_FINAL_DESCENT" },
    { EVENT_REMAIN_SEATED, "EVENT_REMAIN_SEATED" },
    { EVENT_DISEMBARK, "EVENT_DISEMBARK" },
    { EVENT_TURBULENCE, "EVENT_TURBULENCE" },
    { EVENT_REACHED_CRUISE, "EVENT_REACHED_CRUISE" },
    { EVENT_REACHED_TOD, "EVENT_REACHED_TOD" },
    { EVENT_LANDING_PREPARE_CABIN, "EVENT_LANDING_PREPARE_CABIN" },
    { EVENT_SEATS_FOR_LANDING, "EVENT_SEATS_FOR_LANDING" },
    { EVENT_GO_AROUND, "EVENT_GO_AROUND" },
    { VJOY_BUTTONS, "VJOY_BUTTONS" },
    { VJOY_BUTTON_1, "VJOY_BUTTON_1" },
    { VJOY_BUTTON_2, "VJOY_BUTTON_2" },
    { VJOY_BUTTON_3, "VJOY_BUTTON_3" },
    { VJOY_BUTTON_4, "VJOY_BUTTON_4" },
    { VJOY_BUTTON_5, "VJOY_BUTTON_5" },
    { VJOY_BUTTON_6, "VJOY_BUTTON_6" },
    { VJOY_BUTTON_7, "VJOY_BUTTON_7" },
    { VJOY_BUTTON_8, "VJOY_BUTTON_8" },
    { VJOY_BUTTON_9, "VJOY_BUTTON_9" },
    { VJOY_BUTTON_10, "VJOY_BUTTON_10" },
    { VJOY_BUTTON_11, "VJOY_BUTTON_11" },
    { VJOY_BUTTON_12, "VJOY_BUTTON_12" },
    { VJOY_BUTTON_13, "VJOY_BUTTON_13" },
    { VJOY_BUTTON_14, "VJOY_BUTTON_14" },
    { VJOY_BUTTON_15, "VJOY_BUTTON_15" },
    { VJOY_BUTTON_16, "VJOY_BUTTON_16" },
    { VJOY_BUTTONS_END, "VJOY_BUTTONS_END" },
    { SIM_STOP, NULL }
};
