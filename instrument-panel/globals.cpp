#include "globals.h"
#include "simvars.h"

extern globalVars globals;

const int deltaDoubleSize = sizeof(DeltaDouble);
const int deltaStringSize = sizeof(DeltaString);

void identifyAircraft(char* aircraft)
{
    // Identify aircraft
    if (strcmp(aircraft, globals.lastAircraft) != 0) {
        if (strncmp(aircraft, globals.Cessna_152_Text, globals.Cessna_152_Len) == 0) {
            globals.aircraft = CESSNA_152;
        }
        else if (strncmp(aircraft, globals.Cessna_172_Text, globals.Cessna_172_Len) == 0) {
            globals.aircraft = CESSNA_172;
        }
        else if (strncmp(aircraft, globals.Cessna_CJ4_Text, globals.Cessna_CJ4_Len) == 0) {
            globals.aircraft = CESSNA_CJ4;
        }
        else if (strncmp(aircraft, globals.Savage_Cub_Text, globals.Savage_Cub_Len) == 0) {
            globals.aircraft = SAVAGE_CUB;
        }
        else if (strncmp(aircraft, globals.Shock_Ultra_Text, globals.Shock_Ultra_Len) == 0) {
            globals.aircraft = SHOCK_ULTRA;
        }
        else if (strncmp(aircraft, globals.Airbus_A310_Text, globals.Airbus_A310_Len) == 0) {
            globals.aircraft = AIRBUS_A310;
        }
        else if (strncmp(aircraft, globals.Airbus_A320_Text, globals.Airbus_A320_Len) == 0) {
            globals.aircraft = FBW_A320;
        }
        else if (strncmp(aircraft, globals.FBW_A320_Text, globals.FBW_A320_Len) == 0) {
            globals.aircraft = FBW_A320;
        }
        else if (strncmp(aircraft, globals.Boeing_747_Text, globals.Boeing_747_Len) == 0) {
            globals.aircraft = BOEING_747;
        }
        else if (strncmp(aircraft, globals.Boeing_787_Text, globals.Boeing_787_Len) == 0) {
            globals.aircraft = BOEING_787;
        }
        else if (strncmp(aircraft, globals.Supermarine_Spitfire_Text, globals.Supermarine_Spitfire_Len) == 0) {
            globals.aircraft = SUPERMARINE_SPITFIRE;
        }
        else if (strncmp(aircraft, globals.F15_Eagle_Text, globals.F15_Eagle_Len) == 0) {
            globals.aircraft = F15_EAGLE;
        }
        else if (strncmp(aircraft, globals.F18_Hornet_Text, globals.F18_Hornet_Len) == 0) {
            globals.aircraft = F18_HORNET;
        }
        else if (strncmp(aircraft, globals.JustFlight_PA28_Text, globals.JustFlight_PA28_Len) == 0) {
            globals.aircraft = JUSTFLIGHT_PA28;
        }
        else {
            // Need to flip between other aircraft so that instruments
            // can detect the aircraft has changed.
            if (globals.aircraft == OTHER_AIRCRAFT) {
                globals.aircraft = OTHER_AIRCRAFT2;
            }
            else {
                globals.aircraft = OTHER_AIRCRAFT;
            }
        }
        strcpy(globals.lastAircraft, aircraft);
    }
}

/// <summary>
/// Server can send us a delta rather than full data so we need to unpack it.
/// </summary>
void receiveDelta(char *deltaData, long deltaSize, char* simVarsPtr)
{
    char* dataPtr = deltaData;

    while (deltaSize > 0) {
        DeltaDouble* deltaDouble = (DeltaDouble*)dataPtr;
        if (deltaDouble->offset & 0x10000) {
            // Must be a string
            DeltaString* deltaString = (DeltaString*)dataPtr;
            char* stringPtr = simVarsPtr + (deltaString->offset & 0xffff);
            strncpy(stringPtr, deltaString->data, 32);
            stringPtr[31] = '\0';

            dataPtr += deltaStringSize;
            deltaSize -= deltaStringSize;
        }
        else {
            // Must be a double
            char* doublePos = simVarsPtr + deltaDouble->offset;
            double* doublePtr = (double*)doublePos;
            *doublePtr = deltaDouble->data;

            dataPtr += deltaDoubleSize;
            deltaSize -= deltaDoubleSize;
        }
    }
}
