#include <stdio.h>
#include <stdlib.h>
#include "tc.h"
#include "simvars.h"

tc::tc(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("TC");
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
void tc::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("tc.bmp");
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Main dial
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Plane
    bmp = al_create_bitmap(800, 300);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 800, 300, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Plane shadow
    bmp = al_create_bitmap(800, 300);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 300, 800, 300, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Ball, centre of circle is off screen, only need the bottom section
    bmp = al_create_bitmap(800, 148);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 600, 800, 148, 0, 0, 0);
    addBitmap(bmp);

    // 6 = Outer case
    bmp = al_create_bitmap(800, 130);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 748, 800, 130, 0, 0, size, 130 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void tc::render()
{
    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main dial
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add ball at offscreen centre 400, -550 (-651 orig)
    al_draw_scaled_rotated_bitmap(bitmaps[5], 1100, 74, 400 * scaleFactor, -452 * scaleFactor, scaleFactor, scaleFactor, ballAngle * AngleFactor, 0);

    // Add outer case
    al_draw_bitmap(bitmaps[6], 0, 494 * scaleFactor, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add plane shadow
        al_draw_scaled_rotated_bitmap(bitmaps[4], 400, 150, 415 * scaleFactor, 415 * scaleFactor, scaleFactor, scaleFactor, planeAngle * AngleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add plane
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 150, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, planeAngle * AngleFactor, 0);

        // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void tc::update()
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

    // Calculate values
    planeAngle = turn / 10.0f;

    if (planeAngle < -23) {
        planeAngle = -23;
    }
    else if (planeAngle > 23) {
        planeAngle = 23;
    }

    // Need to turn ball by -90 degrees = -64
    targetAngle = (slip / 20.0f) - 64.0f;

    float diff = abs(targetAngle - ballAngle);

    if (diff > 10.0f) {
        if (ballAngle < targetAngle) ballAngle += 1.0f; else ballAngle -= 1.0f;
    }
    else if (diff > 5.0f) {
        if (ballAngle < targetAngle) ballAngle += 0.5f; else ballAngle -= 0.5f;
    }
    else if (diff > 2.5f) {
        if (ballAngle < targetAngle) ballAngle += 0.25f; else ballAngle -= 0.25f;
    }
    else if (diff > 1.25f) {
        if (ballAngle < targetAngle) ballAngle += 0.125f; else ballAngle -= 0.125f;
    }
    else if (diff > 0.625f) {
        if (ballAngle < targetAngle) ballAngle += 0.0625f; else ballAngle -= 0.0625f;
    }
    else if (diff > 0.03f) {
        if (ballAngle < targetAngle) ballAngle += 0.03f; else ballAngle -= 0.03f;
    }
    else {
        ballAngle = targetAngle;
    }

    // Hard stop at edge
    if (ballAngle < -73.5f) {
        ballAngle = -73.5f;
    }
    else if (ballAngle > -54.5f) {
        ballAngle = -54.5f;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void tc::addVars()
{
    globals.simVars->addVar(name, "Turn", 0xf002, false, 4, 0);
    globals.simVars->addVar(name, "Slip", 0xf003, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool tc::fetchVars()
{
    bool success = true;
    DWORD result;

    // Value from FlightSim
    if (!globals.simVars->FSUIPC_Read(0xf002, 4, &turn, &result)) {
        turn = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Read(0xf003, 4, &slip, &result)) {
        slip = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}
