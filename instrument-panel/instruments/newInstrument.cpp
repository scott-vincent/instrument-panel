#include <stdio.h>
#include <stdlib.h>
#include "newInstrument.h"
#include "simvars.h"
#include "knobs.h"

newInstrument::newInstrument(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("New Instrument");
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
void newInstrument::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("new-instrument.bmp");
    addBitmap(orig);

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
void newInstrument::render()
{
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
        al_draw_scaled_rotated_bitmap(bitmaps[4], 400, 50, 415 * scaleFactor, 415 * scaleFactor, scaleFactor, scaleFactor, angle * AngleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 50, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * AngleFactor, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void newInstrument::update()
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

    // Get latest FlightSim variables
    globals.connected = fetchVars();

    // Calculate values
    angle = instrumentVar / 100.0f;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void newInstrument ::addVars()
{
    globals.simVars->addVar(name, "Value", 0x9999, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool newInstrument::fetchVars()
{
    bool success = true;
    DWORD result;

    // Value from FlightSim
    if (!globals.simVars->FSUIPC_Read(0xf000, 4, &instrumentVar, &result)) {
        instrumentVar = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}

#ifndef _WIN32

void newInstrument::addKnobs()
{
    // BCM GPIO 2 and 3
    calKnob = globals.hardwareKnobs->add(2, 3, -100, 100, 0);
}

bool newInstrument::updateKnobs()
{
    DWORD result;

    // Read knob for new instrument calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Convert knob value to new instrument value (adjust for desired sensitivity)
        instrumentVar = val / 10;

        // Update new instrument variable
        if (!globals.simVars->FSUIPC_Write(0xf000, 2, &instrumentVar, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    return true;
}

#endif // !_WIN32
