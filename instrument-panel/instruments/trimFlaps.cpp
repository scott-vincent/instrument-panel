#include <stdio.h>
#include <stdlib.h>
#include "trimFlaps.h"
#include "simvars.h"
#include "knobs.h"

trimFlaps::trimFlaps(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Trim Flaps");
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
void trimFlaps::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("trim-flaps.bmp");
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

    // 3 = Trim
    bmp = al_create_bitmap(40 * scaleFactor, 24 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 0, 40, 24, 0, 0, 40 * scaleFactor, 24 * scaleFactor, 0);
    addBitmap(bmp);

    // 4 = Flaps
    bmp = al_create_bitmap(36 * scaleFactor, 36 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 24, 36, 36, 0, 0, 36 * scaleFactor, 36 * scaleFactor, 0);
    addBitmap(bmp);

    // 5 = Not connected
    bmp = al_create_bitmap(372 * scaleFactor, 50 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 800, 372, 50, 0, 0, 372 * scaleFactor, 50 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void trimFlaps::render()
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

    // Add trim
    al_draw_bitmap(bitmaps[3], 262 * scaleFactor, (388 + trimOffset) * scaleFactor, 0);

    // Add flaps
    al_draw_bitmap(bitmaps[4], 501 * scaleFactor, (211 + flapsOffset) * scaleFactor, 0);

    // Display 'Not Connected message'
    if (!globals.connected)
    {
        al_draw_bitmap(bitmaps[5], 214 * scaleFactor, 650 * scaleFactor, 0);
    }

    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void trimFlaps::update()
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
    trimOffset = trim * 2.0f;

    if (trimOffset < -150) {
        trimOffset = -150;
    }
    else if (trimOffset > 150) {
        trimOffset = 150;
    }

    targetFlaps = flaps * 86.0f;

    if (targetFlaps < 0) {
        targetFlaps = 0;
    }
    else if (targetFlaps > 344) {
        targetFlaps = 344;
    }

    float diff = abs(targetFlaps - flapsOffset);

    if (diff > 4.0f) {
        if (flapsOffset < targetFlaps) {
            flapsOffset += 4.0f;
        }
        else {
            flapsOffset -= 4.0f;
        }
    }
    else {
        flapsOffset = targetFlaps;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void trimFlaps::addVars()
{
    globals.simVars->addVar(name, "Trim", 0xf010, false, 1, 0);
    globals.simVars->addVar(name, "Flaps", 0xf011, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool trimFlaps::fetchVars()
{
    bool success = true;
    DWORD result;

    // Value from FlightSim
    if (!globals.simVars->FSUIPC_Read(0xf010, 4, &trim, &result)) {
        trim = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Read(0xf011, 4, &flaps, &result)) {
        flaps = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}

#ifndef _WIN32

void trimFlaps::addKnobs()
{
    // BCM GPIO 2 and 3
    trimKnob = globals.hardwareKnobs->add(2, 3, -50, 50, 0);

    // BCM GPIO 4 and 5
    flapsKnob = globals.hardwareKnobs->add(4, 5, 0, 4, 0);
}

bool trimFlaps::updateKnobs()
{
    DWORD result;

    // Read knob for trim adjustment
    int val = globals.hardwareKnobs->read(trimKnob);

    if (val != INT_MIN) {
        // Convert knob value to trim (adjust for desired sensitivity)
        trim = val;

        // Update trim
        if (!globals.simVars->FSUIPC_Write(0xf010, 4, &trim, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    // Read knob for flaps
    int val = globals.hardwareKnobs->read(flapsKnob);

    if (val != INT_MIN) {
        // Convert knob value to flaps (adjust for desired sensitivity)
        flaps = val;

        // Update trim
        if (!globals.simVars->FSUIPC_Write(0xf011, 4, &flaps, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    return true;
}

#endif // !_WIN32
