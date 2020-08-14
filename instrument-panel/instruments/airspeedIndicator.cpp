#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "airspeedIndicator.h"
#include "simvars.h"

airspeedIndicator::airspeedIndicator(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Airspeed Indicator");
    addVars();
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void airspeedIndicator::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("airspeed-indicator.bmp");
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

    // 2 = Outer (mach) dial
    ALLEGRO_BITMAP* outer = al_create_bitmap(800, 800);
    al_set_target_bitmap(outer);
    al_draw_bitmap_region(orig, 801, 0, 800, 800, 0, 0, 0);
    addBitmap(outer);

    // 3 = Main dial shadow
    ALLEGRO_BITMAP* shadowBackground = al_create_bitmap(800, 800);
    al_set_target_bitmap(shadowBackground);
    al_draw_scaled_bitmap(orig, 801, 801, 800, 800, 0, 0, size, size, 0);
    addBitmap(shadowBackground);

    // 4 = Shadow sprite (to be rotated)
    ALLEGRO_BITMAP* shadowBackground2 = al_create_bitmap(800, 800);
    addBitmap(shadowBackground2);

    // 5 = Shadow sprite
    ALLEGRO_BITMAP* shadow = al_create_bitmap(149 * scaleFactor, 609 * scaleFactor);
    al_set_target_bitmap(shadow);
    al_draw_scaled_bitmap(orig, 513, 863, 149, 609, 0, 0, 149 * scaleFactor, 609 * scaleFactor, 0);
    addBitmap(shadow);

    // 6 = Pointer sprite
    ALLEGRO_BITMAP* pointer = al_create_bitmap(111, 581);
    al_set_target_bitmap(pointer);
    al_draw_bitmap_region(orig, 153, 881, 111, 581, 0, 0, 0);
    addBitmap(pointer);

    // 7 = Fill with background colour (takes 1 pixel of background and scales it)
    ALLEGRO_BITMAP* bg = al_create_bitmap(size, size);
    al_set_target_bitmap(bg);
    al_draw_scaled_bitmap(orig, 75, 61, 1, 1, 0, 0, size, size, 0);
    addBitmap(bg);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void airspeedIndicator::render()
{
    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Draw background colour
    al_draw_bitmap(bitmaps[7], 0, 0, 0);

    // Display outer dial
    float centre = size / 2.0f;
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, centre, centre, scaleFactor, scaleFactor, machAngle * AngleFactor, 0);

    if (globals.enableShadows) {
        // Display main dial shadow
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
        al_draw_bitmap(bitmaps[3], 0, 0, 0);
        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Display dial (contains transparencies) over outer dial
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, size, size, 0);


    if (globals.enableShadows) {
        // Fill shadow_background bitmap with white
        al_set_target_bitmap(bitmaps[4]);
        al_draw_scaled_bitmap(bitmaps[0], 513, 863, 1, 1, 0, 0, size, size, 0);

        // Write shadow sprite to shadow background
        al_draw_rotated_bitmap(bitmaps[5], 76 * scaleFactor, 367 * scaleFactor, 410 * scaleFactor, 430 * scaleFactor, airspeedAngle * AngleFactor, 0);
        al_set_target_bitmap(bitmaps[1]);

        // Draw shadow
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
        al_draw_bitmap(bitmaps[4], 0, 0, 0);
        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Draw needle pointer at angle
    al_draw_scaled_rotated_bitmap(bitmaps[6], 54, 348, centre, centre, scaleFactor, scaleFactor, airspeedAngle * AngleFactor, 0);

    if (!globals.connected) {
        // Display 'Not Connected message'
        al_draw_scaled_bitmap(bitmaps[0], 1245, 879, 230, 73, 162 * scaleFactor, 318 * scaleFactor, 480 * scaleFactor, 168 * scaleFactor, 0);
    }

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void airspeedIndicator::update()
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
    globals.connected = fetchVars();

    // Calculate airspeed angle
    float speed = airspeed / 1280.0f;

    if (speed > 40) {
        targetAirspeedAngle = 233.65;
    }
    else if (speed > 0) {
        targetAirspeedAngle = 0.0000052772 * pow(speed, 5) - 0.0003338205 * pow(speed, 4) - 0.0025509435 * pow(speed, 3) + 0.482328783 * pow(speed, 2) - 1.5416690771 * speed + 1.0500510789;
        // Angle is not accurate below 3
        if (targetAirspeedAngle < 3) {
            targetAirspeedAngle = 0;
        }
    }
    else {
        targetAirspeedAngle = 0;
    }

    // Smooth out airspeed adjustment at low speed
    if (targetAirspeedAngle < 5 && abs(targetAirspeedAngle - airspeedAngle) > .5f) {
        if (targetAirspeedAngle > airspeedAngle) {
            airspeedAngle += .5f;
        }
        else {
            airspeedAngle -= .5f;
        }
    }
    else {
        airspeedAngle = targetAirspeedAngle;
    }

    // Calculate mach angle
    speed = machSpeed / 20480.0f;

    if (speed > 0.3) {
        machAngle = 256 - ((((251.3 * log(speed) + 446.1) + 4.02) * 0.71111111111111) - airspeedAngle);
    }
    else {
        machAngle = 248.144440;
    }

    if (abs(machAngle - prevMachAngle) < 1)
    {
        machAngle = prevMachAngle;
    }
    else {
        prevMachAngle = machAngle;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void airspeedIndicator::addVars()
{
    globals.simVars->addVar(name, "Airspeed", 0x02BC, false, 100, 0);
    globals.simVars->addVar(name, "Mach Speed", 0x11C6, false, 100, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool airspeedIndicator::fetchVars()
{
    bool success = true;
    DWORD result;

    // Indicated Airspeed Value
    if (!globals.simVars->FSUIPC_Read(0x02BC, 4, &airspeed, &result)) {
        airspeed = 0;
        success = false;
    }

    // Mach speed Value
    if (!globals.simVars->FSUIPC_Read(0x11C6, 2, &machSpeed, &result)) {
        machSpeed = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}
