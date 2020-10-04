#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "annunciator.h"
#include "simvars.h"
#include "knobs.h"

annunciator::annunciator(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Annunciator");
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
void annunciator::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("annunciator.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size / 4);
    addBitmap(bmp);

    // 2 = No warnings
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 3 = Low fuel
    bmp = al_create_bitmap(280 * scaleFactor, 50 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 88, 237, 280, 50, 0, 0, 280 * scaleFactor, 50 * scaleFactor, 0);
    addBitmap(bmp);

    // 4 = Oil pressure
    bmp = al_create_bitmap(257 * scaleFactor, 64 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 35, 289, 257, 64, 0, 0, 257 * scaleFactor, 64 * scaleFactor, 0);
    addBitmap(bmp);

    // 5 = Volts
    bmp = al_create_bitmap(167 * scaleFactor, 62 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 610, 290, 167, 62, 0, 0, 167 * scaleFactor, 62 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = No data link
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 400, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 7 = Not connected
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 600, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 8 = ATC info background
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 600, 1, 1, 0, 0, size, size / 4, 0);
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

    int state;
    if (!globals.dataLinked)
    {
        // 'No Data Link' message
        al_draw_bitmap(bitmaps[6], 0, 0, 0);
        state = 0;
    }
    else if (!globals.connected)
    {
        // 'Not Connected' message
        al_draw_bitmap(bitmaps[7], 0, 0, 0);
        state = 1;
    }
    else {
        state = 2;

        if (selection == 0) {
            showAtcInfo();
        }
        else {
            // Draw no warnings (warnings get overlayed)
            al_draw_bitmap(bitmaps[2], 0, 0, 0);

            // No warnings if no electrics
            if (globals.electrics) {
                if (fuelWarning) {
                    al_draw_bitmap(bitmaps[3], 88 * scaleFactor, 37 * scaleFactor, 0);
                }

                if (oilWarning) {
                    al_draw_bitmap(bitmaps[4], 35 * scaleFactor, 89 * scaleFactor, 0);
                }

                if (voltsWarning) {
                    al_draw_bitmap(bitmaps[5], 610 * scaleFactor, 90 * scaleFactor, 0);
                }
            }
        }
    }

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (state != prevState) {
        dimDelay = 1000;
        prevState = state;
    }
    else if (state < 2 && dimDelay > 0) {
        dimDelay--;
    }
    else if (!globals.active && !globals.electrics) {
        dimInstrument();
    }
}

void annunciator::showAtcInfo()
{
    char callSign[256];

    if (simVars->atcFlightNumber[0] == '\0') {
        sprintf(callSign, "%s ", simVars->atcCallSign);
    }
    else {
        sprintf(callSign, "%s %s ", simVars->atcCallSign, simVars->atcFlightNumber);
    }

    if (simVars->atcHeavy == 1) {
        strcat(callSign, "Heavy");
    }

    al_draw_bitmap(bitmaps[8], 0, 0, 0);
    al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), 20, 20, 0, simVars->atcTailNumber);
    // Debug - Uncomment to show aircraft name
    //strcpy(callSign, simVars->aircraft);
    al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), 20, 40, 0, callSign);
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

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    // Calculate values
    double fuelLevel = (simVars->fuelLeft + simVars->fuelRight) / 2;

    // Warning at 5 gallons = 19%
    if (fuelLevel == 0 || fuelLevel > 19) {
        fuelWarning = false;
    }
    else if (prevFuel > 19) {
        // Start flashing for 10 seconds
        prevFuel = fuelLevel;
        flashCount = 20;
        fuelWarning = true;
#ifdef _WIN32
        lastFlash = GetTickCount64();
#else
        clock_gettime(CLOCK_MONOTONIC, &lastFlash);
#endif
    }
    else if (flashCount > 0) {
        // Alternate on/off every 1 second
        long timeDiff;
#ifdef _WIN32
        long now = GetTickCount64();
        timeDiff = now - lastFlash;
#else
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        timeDiff = (now.tv_sec - lastFlash.tv_sec) * 1000 + (now.tv_nsec - lastFlash.tv_nsec) / 1000000;
#endif
        if (timeDiff >= 500) {
            fuelWarning = !fuelWarning;
            flashCount--;
            lastFlash = now;
        }
    }
    else {
        // Steady fuel warning
        fuelWarning = true;
    }

    // Oil warning if pressure < 20 PSI
    oilWarning = (simVars->oilPressure < 20);

    // Volts warning if battery not charging, i.e. load is greater than zero
    voltsWarning = (simVars->batteryLoad > 0);
}

#ifndef _WIN32

void annunciator::addKnobs()
{
    // BCM GPIO 2
    selSwitch = globals.hardwareKnobs->add(2, 0, -1, -1, 0);
}

void annunciator::updateKnobs()
{
    // Read switch
    int val = globals.hardwareKnobs->read(selSwitch);

    if (val != INT_MIN) {
        selection = 1 - (val % 2);
    }
}

#endif // !_WIN32
