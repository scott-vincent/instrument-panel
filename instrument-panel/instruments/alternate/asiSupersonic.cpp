#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "asiSupersonic.h"

asiSupersonic::asiSupersonic(int xPos, int yPos, int size, const char *parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size and vars from parent
        setName(parentName);
    }
    else {
        setName("ASI Supersonic");
        addVars();
    }

    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void asiSupersonic::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi-supersonic.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Main dial
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Pointer
    bmp = al_create_bitmap(50, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 50, 600, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void asiSupersonic::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main dial
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[3], 25, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

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
void asiSupersonic::update()
{
    // Check for position or size change
    int *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Calculate values
    if (simVars->asiAirspeed < 50) {
        // 0 to 20 degrees = 0 to 50 knots
        angle = simVars->asiAirspeed * 20.0 / 50.0;
    }
    else if (simVars->asiAirspeed < 200) {
        // 20 to 140 degrees = 50 to 200 knots
        angle = 20 + (simVars->asiAirspeed - 50) * 120.0 / 150.0;
    }
    else if (simVars->asiAirspeed < 300) {
        // 140 to 180 degrees = 200 to 300 knots
        angle = 140 + (simVars->asiAirspeed - 200) * 40.0 / 100.0;
    }
    else if (simVars->asiAirspeed < 600) {
        // 180 to 270 degrees = 300 to 600 knots
        angle = 180 + (simVars->asiAirspeed - 300) * 90.0 / 300.0;
    }
    else if (simVars->asiAirspeed < 1000) {
        // 270 to 340 degrees = 600 to 1000 knots
        angle = 270 + (simVars->asiAirspeed - 600) * 70.0 / 400.0;
    }
    else {
        angle = 340;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void asiSupersonic::addVars()
{
    globals.simVars->addVar(name, "Airspeed Indicated", false, 1, 0);
}
