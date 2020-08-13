#include <stdio.h>
#include <stdlib.h>
#include "newInstrument.h"
#include "simvars.h"

newInstrument::newInstrument(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("New Instrument");
    addVars();
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
    //ALLEGRO_BITMAP* orig = loadBitmap("new-instrument.bmp");
    ALLEGRO_BITMAP* orig = al_create_bitmap(size, size);
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

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

    // Get latest FlightSim variables
    globals.connected = fetchVars();

    // Calculate values
    instrumentValue = instrumentVar + 42;
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
    DWORD dwResult;

    // Value from FlightSim
    if (!globals.simVars->FSUIPC_Read(0x9999, 4, &instrumentVar, &dwResult)) {
        instrumentVar = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&dwResult))
    {
        success = false;
    }

    return success;
}
