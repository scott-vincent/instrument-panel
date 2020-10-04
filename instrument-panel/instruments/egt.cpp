#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "egt.h"
#include "simvars.h"

egt::egt(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("EGT");
    addVars();
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void egt::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
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

    // 4 = Flow Pointer
    bmp = al_create_bitmap(200, 40);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 200, 40, 0, 0, 0);
    addBitmap(bmp);

    // 5 = EGT Pointer
    bmp = al_create_bitmap(200, 40);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 14, 840, 139, 40, 0, 0, 0);
    addBitmap(bmp);

    // 6 = EGT Ref Pointer
    bmp = al_create_bitmap(200, 40);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 7, 880, 155, 40, 0, 0, 0);
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

    // Add Flow pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 186, 20, 386 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, flowAngle * DegreesToRadians, 0);

    // Add EGT pointer
    al_draw_scaled_rotated_bitmap(bitmaps[5], 0, 20, 38 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, egtAngle * DegreesToRadians, 0);

    // Add EGT Ref pointer
    al_draw_scaled_rotated_bitmap(bitmaps[6], 0, 20, 38 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, egtRefAngle * DegreesToRadians, 0);

    // Add top layer
    al_draw_bitmap(bitmaps[3], 0, 0, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void egt::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Get latest FlightSim variables
    SimVars* simVars = &globals.simVars->simVars;

    // Calculate values
    if (simVars->exhaustGasTemp < 680)
        egtAngle = 52;
    else
        egtAngle = -0.4496*simVars->exhaustGasTemp+357.74;
    float tmp = simVars->exhaustGasTempGES/16384;
    egtRefAngle = 101.718*pow(tmp,3) - 104.843 * pow(tmp, 2) - 96.894 * tmp + 49.022;
    tmp = simVars->engineFuelFlow;
    flowAngle = -.02 * pow(tmp, 3) + .6759 * pow(tmp, 2) - 1.6837 * tmp - 41.4675;

    // Gauge not working so just zero out for now
    egtAngle = 52;
    egtRefAngle = 49;
    flowAngle = -43;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void egt::addVars()
{
    globals.simVars->addVar(name, "General Eng Exhaust Gas Temperature:1", false, 1, 0);
    globals.simVars->addVar(name, "Eng Exhaust Gas Temperature GES:1", false, 1, 0);
    globals.simVars->addVar(name, "Eng Fuel Flow GPH:1", false, 1, 0);

}
