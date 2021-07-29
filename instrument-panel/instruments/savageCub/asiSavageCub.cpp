#include <cstdio>
#include <cmath>
#include "asiSavageCub.h"

asiSavageCub::asiSavageCub(int xPos, int yPos, int size, const char *parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size and vars from parent
        setName(parentName);
    }
    else {
        setName("ASI Savage Cub");
        addVars();
    }

    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void asiSavageCub::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi-savage-cub.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Pointer
    bmp = al_create_bitmap(80, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 80, 800, 0, 0, 0);
    addBitmap(bmp);

    // 3 = Pointer shadow
    bmp = al_create_bitmap(80, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 880, 0, 80, 800, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void asiSavageCub::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main dial
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, size, size, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[3], 40, 400, 410 * scaleFactor, 410 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[2], 40, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

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
void asiSavageCub::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Calculate values
    airspeedKnots = simVars->asiAirspeed * 2;

    // Not a linear scale!
    if (airspeedKnots < 40) {
        airspeedAngle = airspeedKnots * 0.013;
    }
    else if (airspeedKnots < 90) {
        airspeedAngle = -0.03 + pow(airspeedKnots - 12.1, 1.439) * 0.0047;
    }
    else if (airspeedKnots < 120) {
        airspeedAngle = 0.4 + pow(airspeedKnots - 12.1, 1.4) * 0.0046;
    }
    else if (airspeedKnots < 160) {
        airspeedAngle = 1.53 + pow(airspeedKnots - 12.0, 1.320) * 0.0043;
    }
    else {
        airspeedAngle = 2.27 + pow(airspeedKnots - 12.0, 1.28) * 0.0040;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void asiSavageCub::addVars()
{
    globals.simVars->addVar(name, "Airspeed Indicated", false, 1, 0);
}
