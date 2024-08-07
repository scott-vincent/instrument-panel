#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vsi.h"
#include "spitfire/vsiSpitfire.h"
#include "alternate/vsiExtreme.h"
#include "simvars.h"

vsi::vsi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("VSI");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void vsi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("vsi.png");
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
    bmp = al_create_bitmap(800, 100);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 800, 100, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Pointer shadow
    bmp = al_create_bitmap(800, 100);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 900, 800, 100, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void vsi::render()
{
    if (bitmaps[0] == NULL || loadedAircraft != globals.aircraft) {
        return;
    }

    if (customInstrument) {
        customInstrument->render();
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main dial
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[4], 400,50, 415 * scaleFactor, 415 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 50, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

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
void vsi::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;

        // Load custom instrument for this aircraft if we have one
        if (customInstrument) {
            delete customInstrument;
            customInstrument = NULL;
        }

        if (loadedAircraft == SUPERMARINE_SPITFIRE) {
            customInstrument = new vsiSpitfire(xPos, yPos, size, name);
        }
        else if (loadedAircraft == F15_EAGLE || loadedAircraft == ORNITHOPTER) {
            customInstrument = new vsiExtreme(xPos, yPos, size, name);
        }
    }

    if (customInstrument) {
        customInstrument->update();
        return;
    }

    // Check for position or size change
    int *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2] || aircraftChanged) {
        size = settings[2];
        resize();
    }

    // Convert feet per second to 100 feet per minute
    vertSpeed = abs(simVars->vsiVerticalSpeed * 0.6);

    // Different scale after 10
    if (vertSpeed > 10) {
        targetAngle = 79.5 + (96.0 * (vertSpeed - 10.0)) / 20.0;

        if (targetAngle > 175.5) {
            targetAngle = 175.5;
        }
    }
    else {
        targetAngle = (79.5 * vertSpeed) / 10.0;
    }

    if (simVars->vsiVerticalSpeed < 0) {
        targetAngle = -targetAngle;
    }

    double diff = abs(targetAngle - angle);

    if (diff > 40.0) {
        if (angle < targetAngle) angle += 20.0; else angle -= 20.0;
    }
    else if (diff > 20.0) {
        if (angle < targetAngle) angle += 10.0; else angle -= 10.0;
    }
    else if (diff > 10.0) {
        if (angle < targetAngle) angle += 5.0; else angle -= 5.0;
    }
    else if (diff > 5.0) {
        if (angle < targetAngle) angle += 2.5; else angle -= 2.5;
    }
    else if (diff > 2.5) {
        if (angle < targetAngle) angle += 1.25; else angle -= 1.25;
    }
    else if (diff > 0.625) {
        if (angle < targetAngle) angle += 0.625; else angle -= 0.625;
    }
    else {
        angle = targetAngle;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void vsi::addVars()
{
    globals.simVars->addVar(name, "Vertical Speed", false, 4, 0);
}
