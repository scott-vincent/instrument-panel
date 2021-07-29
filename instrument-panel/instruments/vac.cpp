#include <cstdio>
#include "vac.h"
#include "simvars.h"

vac::vac(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("VAC");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void vac::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 400)
    scaleFactor = size / 400.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("vac.png");
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

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void vac::render()
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

    // Add left pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 60 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, vacAngle * DegreesToRadians, 0);

    // Add right pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 340 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, ampAngle * DegreesToRadians, 0);

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
void vac::update()
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
    vacAngle = 60 - (simVars->suctionPressure - 3) * 30;
    if (vacAngle < -60) {
        vacAngle = -60;
    }
    else if (vacAngle > 60) {
        vacAngle = 60;
    }
    // MSFS seems to have a bug in the battery load; the sign must be reversed.
    ampAngle = 119 + (-simVars->batteryLoad + 60);
    if (ampAngle < 119) {
        ampAngle = 119;
    }
    else if (ampAngle > 239) {
        ampAngle = 239;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void vac::addVars()
{
    globals.simVars->addVar(name, "Suction Pressure", false, 1, 1);
    globals.simVars->addVar(name, "Electrical Battery Load", false, 1, 0);
}
