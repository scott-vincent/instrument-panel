#include <cstdio>
#include <cstdlib>
#include "tc.h"
#include "simvars.h"

tc::tc(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("TC");
    addVars();
    simVars = &globals.simVars->simVars;
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
    ALLEGRO_BITMAP* orig = loadBitmap("tc.png");
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
    if (bitmaps[0] == nullptr) {
        return;
    }

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

    if (!globals.electrics) {
        dimInstrument();
    }
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

    // Calculate values
    planeAngle = simVars->tcRate * 200;

    if (planeAngle < -23) {
        planeAngle = -23;
    }
    else if (planeAngle > 23) {
        planeAngle = 23;
    }

    // Need to turn ball by -90 degrees = -64
    targetAngle = (-simVars->tcBall * 9) - 64.0;

    double diff = abs(targetAngle - ballAngle);

    if (diff > 20.0) {
        if (ballAngle < targetAngle) ballAngle += 4.0; else ballAngle -= 4.0;
    }
    else if (diff > 10.0) {
        if (ballAngle < targetAngle) ballAngle += 2.0; else ballAngle -= 2.0;
    }
    else if (diff > 5.0) {
        if (ballAngle < targetAngle) ballAngle += 1.0; else ballAngle -= 1.0;
    }
    else if (diff > 2.5) {
        if (ballAngle < targetAngle) ballAngle += 0.5; else ballAngle -= 0.5;
    }
    else if (diff > 0.25) {
        if (ballAngle < targetAngle) ballAngle += 0.25; else ballAngle -= 0.25;
    }
    else {
        ballAngle = targetAngle;
    }

    // Hard stop at edge
    if (ballAngle < -73.5) {
        ballAngle = -73.5;
    }
    else if (ballAngle > -54.5) {
        ballAngle = -54.5;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void tc::addVars()
{
    globals.simVars->addVar(name, "Turn Indicator Rate", false, 0.01, 0);
    globals.simVars->addVar(name, "Turn Coordinator Ball", false, 1, 0);
}
