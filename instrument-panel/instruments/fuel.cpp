#include <stdio.h>
#include <stdlib.h>
#include "fuel.h"
#include "simvars.h"
#include "knobs.h"

fuel::fuel(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Fuel");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void fuel::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 400)
    scaleFactor = size / 400.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("fuel.png");
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
void fuel::render()
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
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 60 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, angleLeft * DegreesToRadians, 0);

    // Add right pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 60, 20, 340 * scaleFactor, 200 * scaleFactor, scaleFactor, scaleFactor, angleRight * DegreesToRadians, 0);

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
void fuel::update()
{
    // Check for position or size change
    int *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Split fuel from all tanks between left/right
    // by using main tank percentages as a ratio.
    double fuelPercent = 100 * simVars->fuelQuantity / simVars->fuelCapacity;

    if (simVars->fuelLeftPercent == simVars->fuelRightPercent) {
        leftPercent = fuelPercent;
        rightPercent = fuelPercent;
    }
    else {
        fuelPercent *= 2;
        totalPercent = simVars->fuelLeftPercent + simVars->fuelRightPercent;
        leftPercent = fuelPercent * simVars->fuelLeftPercent / totalPercent;
        rightPercent = fuelPercent * simVars->fuelRightPercent / totalPercent;

        // Normalise percentages (maintain overall percent)
        if (leftPercent > 100) {
            rightPercent += leftPercent - 100;
            leftPercent = 100;
        }

        if (rightPercent > 100) {
            leftPercent += rightPercent - 100;
            rightPercent = 100;
        }
    }

    angleLeft = 55 - leftPercent * 1.144;
    angleRight = 124 + rightPercent * 1.144;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void fuel::addVars()
{
    globals.simVars->addVar(name, "Fuel Tank Left Main Level", false, 1, 0);
    globals.simVars->addVar(name, "Fuel Tank Right Main Level", false, 1, 0);
}
