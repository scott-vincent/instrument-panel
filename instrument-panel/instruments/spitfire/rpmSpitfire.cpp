#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rpmSpitfire.h"

rpmSpitfire::rpmSpitfire(int xPos, int yPos, int size, const char* parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size and vars from parent
        setName(parentName);
    }
    else {
        setName("RPM Spitfire");
        addVars();
    }

    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void rpmSpitfire::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("rpm-spitfire.png");
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
    bmp = al_create_bitmap(80, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 80, 600, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void rpmSpitfire::render()
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
    al_draw_scaled_rotated_bitmap(bitmaps[3], 40, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

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
void rpmSpitfire::update()
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
    rpm = simVars->rpmEngine;

    if (rpm == 0) {
        targetAngle = -180;
    }
    else {
        if (rpm < 900) rpm = 900;
        targetAngle = rpm * 0.075 - 225;
    }

    double diff = abs(angle - targetAngle);

    if (diff < 2) {
        angle = targetAngle;
    }
    else if (rpm <= 900) {
        if (targetAngle > angle) angle += 0.4;
        else if (targetAngle < angle) angle -= 0.4;
    }
    else if (diff < 25) {
        if (targetAngle > angle) angle++;
        else if (targetAngle < angle) angle--;
    }
    else {
        if (targetAngle > angle) angle += 2;
        else if (targetAngle < angle) angle -= 2;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void rpmSpitfire::addVars()
{
    globals.simVars->addVar(name, "General Eng Rpm:1", false, 1, 0);
}
