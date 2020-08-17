#include <stdio.h>
#include <stdlib.h>
#include "adi.h"
#include "simvars.h"
#include "knobs.h"

adi::adi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ADI");
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
void adi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("adi.bmp");
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void adi::render()
{
    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Fill with ?
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, size, size, 0);

    if (globals.enableShadows) {
        // Display shadow
    }

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void adi::update()
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
    instrumentValue = instrumentVar + 42;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void adi::addVars()
{
    globals.simVars->addVar(name, "Value", 0x9997, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool adi::fetchVars()
{
    bool success = true;
    DWORD result;

    // Value from FlightSim
    if (!globals.simVars->FSUIPC_Read(0x9997, 4, &instrumentVar, &result)) {
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

void adi::addKnobs()
{
    // BCM GPIO 38 and 39
    newKnob = globals.hardwareKnobs->add(38, 39, -100, 100, 0);
}

bool adi::updateKnobs()
{
    DWORD result;

    // Read knob for new instrument calibration
    int val = globals.hardwareKnobs->read(newKnob);

    if (val != INT_MIN) {
        // Convert knob value to new instrument value (adjust for desired sensitivity)
        instrumentVar = val / 10;

        // Update new instrument variable
        if (!globals.simVars->FSUIPC_Write(0x9999, 2, &instrumentVar, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    return true;
}

#endif // !_WIN32
