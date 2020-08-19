#include <stdio.h>
#include <stdlib.h>
#include "asi.h"
#include "simvars.h"
#include "knobs.h"

asi::asi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ASI");
    addVars();

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        addKnobs();
    }
#endif

    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void asi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi.bmp");
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

    // 2 = Outer scale
    ALLEGRO_BITMAP* outer = al_create_bitmap(800, 800);
    al_set_target_bitmap(outer);
    al_draw_bitmap_region(orig, 0, 800, 800, 800, 0, 0, 0);
    addBitmap(outer);

    // 3 = Outer scale shadow
    ALLEGRO_BITMAP* outerShadow = al_create_bitmap(size, 180 * scaleFactor);
    al_set_target_bitmap(outerShadow);
    al_draw_scaled_bitmap(orig, 0, 1600, 800, 180, 0, 0, size, 180 * scaleFactor, 0);
    addBitmap(outerShadow);

    // 4 = Pointer
    ALLEGRO_BITMAP* pointer3 = al_create_bitmap(80, 800);
    al_set_target_bitmap(pointer3);
    al_draw_bitmap_region(orig, 800, 0, 80, 800, 0, 0, 0);
    addBitmap(pointer3);

    // 5 = Pointer shadow
    ALLEGRO_BITMAP* shadow3 = al_create_bitmap(80, 800);
    al_set_target_bitmap(shadow3);
    al_draw_bitmap_region(orig, 800, 800, 80, 800, 0, 0, 0);
    addBitmap(shadow3);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void asi::render()
{
    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add outer scale (adjusted airspeed) and rotate
    // 0 = 0 radians
    angle =  airspeedCal * 0.01f;
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add outer hole Shadow
        al_draw_bitmap_region(bitmaps[3], 0, 0, size, 180 * scaleFactor, 10 * scaleFactor, 630 * scaleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add main dial
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, size, size, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[5], 40, 400, 410 * scaleFactor, 410 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 40, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void asi::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    // Get latest FlightSim variables
    globals.connected = fetchVars();

    // Calculate values - Not a linear scale!
    airspeedKnots = airspeed / 128;
    if (airspeedKnots < 40) {
        airspeedAngle = airspeedKnots * 0.013f;
    }
    else if (airspeedKnots < 90) {
        airspeedAngle = -0.03f + pow(airspeedKnots - 12.1, 1.439) * 0.0047f;
    }
    else if (airspeedKnots < 120) {
        airspeedAngle = 0.4f + pow(airspeedKnots - 12.1, 1.4) * 0.0046f;
    }
    else if (airspeedKnots < 160) {
        airspeedAngle = 1.53f + pow(airspeedKnots - 12.0, 1.320) * 0.0043f;
    }
    else {
        airspeedAngle = 2.27f + pow(airspeedKnots - 12.0, 1.28) * 0.0040f;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void asi::addVars()
{
    // Add 0x8000 to all vars for now so that Learjet asi can be displayed at the same time
    globals.simVars->addVar(name, "Airspeed", 0x02BC + 0x8000, false, 128, 0);
    globals.simVars->addVar(name, "Airspeed Calibration", 0xF000, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool asi::fetchVars()
{
    bool success = true;
    DWORD result;

    // Values from FlightSim
    if (!globals.simVars->FSUIPC_Read(0x02BC + 0x8000, 4, &airspeed, &result)) {
        airspeed = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Read(0xF000, 2, &airspeedCal, &result)) {
        airspeedCal = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}

#ifndef _WIN32

void asi::addKnobs()
{
    // BCM GPIO 2 and 3
    calKnob = globals.hardwareKnobs->add(2, 3, -500, 500, 0);
}

bool asi::updateKnobs()
{
    DWORD result;

    // Read knob for airspeed calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Convert knob value to adjusted airspeed (adjust for desired sensitivity)
        airspeedCal = val / 10;

        // Update airspeed calibration
        if (!globals.simVars->FSUIPC_Write(0xF000, 2, &airspeedCal, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    return true;
}

#endif // !_WIN32
