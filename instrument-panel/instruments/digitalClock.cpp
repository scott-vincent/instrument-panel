#include <stdio.h>
#include <stdlib.h>
#include "digitalClock.h"
#include "simvars.h"
#include "knobs.h"

digitalClock::digitalClock(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Digital Clock");
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
void digitalClock::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("digital-clock.bmp");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Main panel
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void digitalClock::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main panel
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void digitalClock::update()
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
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void digitalClock::addVars()
{
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool digitalClock::fetchVars()
{
    bool success = true;

    return success;
}

#ifndef _WIN32

void digitalClock::addKnobs()
{
    // BCM GPIO 2 and 3
    clockButton1 = globals.hardwareKnobs->add(2, 3, 0, 1, 0);

    // BCM GPIO 4 and 5
    clockButton2 = globals.hardwareKnobs->add(4, 5, 0, 1, 0);

    // BCM GPIO 6 and 7
    clockButton3 = globals.hardwareKnobs->add(6, 7, 0, 1, 0);
}

bool digitalClock::updateKnobs()
{
    DWORD result;

    // Read buttons for clock adjustment
    int val = globals.hardwareKnobs->read(clockButton1);
    if (val != INT_MIN) {
        button1 = val;
    }

    int val = globals.hardwareKnobs->read(clockButton2);
    if (val != INT_MIN) {
        button2 = val;
    }

    int val = globals.hardwareKnobs->read(clockButton3);
    if (val != INT_MIN) {
        button3 = val;
    }

    return true;
}

#endif // !_WIN32
