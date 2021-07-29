#include <cstdio>
#include <cstdlib>
#include "adf.h"
#include "simvars.h"
#include "knobs.h"

adf::adf(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ADF");
    addVars();
    simVars = &globals.simVars->simVars;

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
void adf::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("adf.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Locator
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 0, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 3 = Compass
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Top
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1600, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void adf::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add locator
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, locAngle * DegreesToRadians, 0);

    // Add compass
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, compassAngle * DegreesToRadians, 0);

    // Add top
    al_draw_bitmap(bitmaps[4], 0, 0, 0);

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
void adf::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    // Calculate values
    locAngle = simVars->adfRadial;
    compassAngle = -simVars->adfCard;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void adf::addVars()
{
    globals.simVars->addVar(name, "Adf Radial:1", false, 1, 0);
    globals.simVars->addVar(name, "Adf Card", false, 1, 0);
}

#ifndef _WIN32

void adf::addKnobs()
{
    // BCM GPIO 24 and 25
    adfCardKnob = globals.hardwareKnobs->add(24, 25, -1, -1, 0);
}

void adf::updateKnobs()
{
    // Read knob for ADF Card setting
    int val = globals.hardwareKnobs->read(adfCardKnob);

    if (val != INT_MIN) {
        // Change ADF Card by knob movement amount (adjust for desired sensitivity)
        int adjust = (int)((prevVal - val) / 2) * 5;
        if (adjust != 0) {
            double newVal = globals.simVars->simVars.adfCard + adjust;

            if (newVal < 0) {
                newVal += 360;
            }
            else if (newVal >= 360) {
                newVal -= 360;
            }
            globals.simVars->write(KEY_ADF_CARD_SET, newVal);
            prevVal = val;
        }
    }
}

#endif // !_WIN32
