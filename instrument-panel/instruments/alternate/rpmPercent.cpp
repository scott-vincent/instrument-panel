#include <cstdio>
#include <math.h>
#include "rpmPercent.h"

rpmPercent::rpmPercent(int xPos, int yPos, int size, const char* parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size and vars from parent
        setName(parentName);
    }
    else {
        setName("RPM Percent");
        addVars();
    }

    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void rpmPercent::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("rpm-percent.png");
    addBitmap(orig);

    if (bitmaps[0] == nullptr) {
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
    bmp = al_create_bitmap(100, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 100, 600, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Pointer shadow
    bmp = al_create_bitmap(100, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 900, 0, 100, 600, 0, 0, 0);
    addBitmap(bmp);

    // 5 = White digits
    bmp = al_create_bitmap(54 * scaleFactor, 594 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1000, 0, 54, 594, 0, 0, 54 * scaleFactor, 594 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = Black digits
    bmp = al_create_bitmap(54 * scaleFactor, 594 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1054, 0, 54, 594, 0, 0, 54 * scaleFactor, 594 * scaleFactor, 0);
    addBitmap(bmp);

    // 7 = Hourglass
    bmp = al_create_bitmap(54 * scaleFactor, 156 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1108, 0, 54, 156, 0, 0, 54 * scaleFactor, 156 * scaleFactor, 0);
    addBitmap(bmp);

    // 8 = Reverse thrust indicator
    bmp = al_create_bitmap(75 * scaleFactor, 30 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1000, 604, 75, 30, 0, 0, 75 * scaleFactor, 30 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void rpmPercent::render()
{
    if (bitmaps[0] == nullptr) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main dial
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add reverse thrust indicator
    if (simVars->throttlePosition < 0) {
        al_draw_bitmap(bitmaps[8], 362 * scaleFactor, 496 * scaleFactor, 0);
    }

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[4], 50, 400, 415 * scaleFactor, 415 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[3], 50, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

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
void rpmPercent::update()
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
    digit1 = ((int)simVars->rpmElapsedTime % 10000) / 1000;
    digit2 = ((int)simVars->rpmElapsedTime % 1000) / 100;
    digit3 = ((int)simVars->rpmElapsedTime % 100) / 10;
    digit4 = (int)simVars->rpmElapsedTime % 10;
    digit5 = (int)(simVars->rpmElapsedTime * 10) % 10;

    // RPM percent dial begins at vertical and spans 315 degrees
    // The scale changes at 60 percent
    if (simVars->turbineEngineN1 < 60) {
        // The 0-60% arc spans 135 degrees
        angle = 135 * (simVars->turbineEngineN1 / 60 );
    }
    else {
        // The remaining 40% arc (60-100%) starts at 135 degs and spans 180 degrees
        angle = 135 + (181 * (simVars->turbineEngineN1 - 60) / 40.0);    
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void rpmPercent::addVars()
{
    globals.simVars->addVar(name, "General Eng Elapsed Time:1", false, 1, 0);
    globals.simVars->addVar(name, "Turb Eng N1:1", false, 1, 0);
}
