#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "egt.h"
#include "simvars.h"

egt::egt(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("EGT");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void egt::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 400)
    scaleFactor = size / 400.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("egt.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Dials
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 400, 400, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Top layer
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 400, 400, 400, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 4 = Pointer
    bmp = al_create_bitmap(200, 40);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 200, 40, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Ref pointer
    bmp = al_create_bitmap(200, 12);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 840, 200, 12, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void egt::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add dials
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add ref pointer
    al_draw_scaled_rotated_bitmap(bitmaps[5], 60, 6, 60 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, egtRefAngle * DegreesToRadians, 0);

    // Add left pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 60 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, egtAngle * DegreesToRadians, 0);

    // Add right pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 340 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, flowAngle * DegreesToRadians, 0);

    // Add top layer
    al_draw_bitmap(bitmaps[3], 0, 0, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.electrics) {
        dimInstrument();
    }
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void egt::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        egtRefAngle = 62;
    }

    // Check for position or size change
    int *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Calculate values
    if (simVars->numberOfEngines == 2) {
        exhaustGasTemp = (simVars->exhaustGasTemp1 + simVars->exhaustGasTemp2) / 2;
        engineFuelFlow = (simVars->engineFuelFlow1 + simVars->engineFuelFlow2) / 2;
    }
    else if (simVars->numberOfEngines > 3) {
        exhaustGasTemp = (simVars->exhaustGasTemp1 + simVars->exhaustGasTemp2 + simVars->exhaustGasTemp3 + simVars->exhaustGasTemp4) / 4;
        engineFuelFlow = (simVars->engineFuelFlow1 + simVars->engineFuelFlow2 + simVars->engineFuelFlow3 + simVars->engineFuelFlow4) / 4;
    }
    else if (simVars->numberOfEngines == 3) {
        exhaustGasTemp = (simVars->exhaustGasTemp1 + simVars->exhaustGasTemp2 + simVars->exhaustGasTemp3) / 3;
        engineFuelFlow = (simVars->engineFuelFlow1 + simVars->engineFuelFlow2 + simVars->engineFuelFlow3) / 3;
    }
    else {
        exhaustGasTemp = simVars->exhaustGasTemp1;
        engineFuelFlow = simVars->engineFuelFlow1;
    }

    egtAngle = 62 - (exhaustGasTemp - 680) * 0.527885;
    if (egtAngle < -60) {
        egtAngle = -60;
    }
    else if (egtAngle > 62) {
        egtAngle = 62;
    }

    if (egtAngle < egtRefAngle) {
        egtRefAngle = egtAngle;
    }

    if (loadedAircraft == FBW) {
        if (engineFuelFlow > 300) {
            flowAngle = 126 + (engineFuelFlow - 300) * 0.039;
        }
        else {
            flowAngle = 118 + engineFuelFlow * 0.0267;
        }
    }
    else {
        if (engineFuelFlow > 5) {
            flowAngle = 126 + (engineFuelFlow - 5) * 8;
        }
        else {
            flowAngle = 118 + engineFuelFlow * 1.6;
        }
    }

    if (flowAngle < 118) {
        flowAngle = 118;
    }
    else if (flowAngle >238) {
        flowAngle = 238;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void egt::addVars()
{
    globals.simVars->addVar(name, "General Eng Exhaust Gas Temperature:1", false, 1, 0);
    globals.simVars->addVar(name, "Eng Fuel Flow GPH:1", false, 1, 0);
}
