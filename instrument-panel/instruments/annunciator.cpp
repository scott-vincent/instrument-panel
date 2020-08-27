#include <stdio.h>
#include <stdlib.h>
#include "annunciator.h"
#include "simvars.h"

annunciator::annunciator(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Annunciator");
    addVars();
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void annunciator::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("annunciator.bmp");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size / 4);
    addBitmap(bmp);

    // 2 = Main panel
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 3 = No data link
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 200, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 4 = Not connected
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 400, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void annunciator::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    if (!globals.dataLinked)
    {
        // 'No Data Link' message
        al_draw_bitmap(bitmaps[3], 0, 0, 0);
    }
    else if (!globals.connected)
    {
        // 'Not Connected' message
        al_draw_bitmap(bitmaps[4], 0, 0, 0);
    }
    else {
        // Main display
        al_draw_bitmap(bitmaps[2], 0, 0, 0);
    }

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void annunciator::update()
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
    SimVars* simVars = &globals.simVars->simVars;

    // Calculate values
    angle = simVars->adiBank / 100.0f;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void annunciator::addVars()
{
    // globals.simVars->addVar(name, "Value", false, 1, 0);
}
