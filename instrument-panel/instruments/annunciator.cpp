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

    // 3 = L VAC R
    bmp = al_create_bitmap(245, 62);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 34, 230, 245, 62, 0, 0, 0);
    addBitmap(bmp);

    // 4 = L LOWFUEL R
    bmp = al_create_bitmap(374, 62);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 34, 290, 374, 62, 0, 0, 0);
    addBitmap(bmp);

    // 5 = OIL PRESS
    bmp = al_create_bitmap(260 * scaleFactor, 64 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 496, 229, 260, 64, 0, 0, 260 * scaleFactor, 64 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = VOLTS
    bmp = al_create_bitmap(170 * scaleFactor, 62 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 590, 290, 170, 62, 0, 0, 170 * scaleFactor, 62 * scaleFactor, 0);
    addBitmap(bmp);

    // 7 = No data link
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 400, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 8 = Not connected
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 600, 800, 200, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 9 = ATC info background
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
        al_draw_bitmap(bitmaps[7], 0, 0, 0);
        state = 0;
    }
    else if (!globals.connected)
    {
        // 'Not Connected' message
        al_draw_bitmap(bitmaps[8], 0, 0, 0);
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
                if (vacWarningL || vacWarningR) {

                    if (vacWarningL) {
                        al_draw_scaled_bitmap(bitmaps[3], 0, 0, 46, 62, 34 * scaleFactor, 30 * scaleFactor, 46 * scaleFactor, 62 * scaleFactor, 0);
                    }
                    al_draw_scaled_bitmap(bitmaps[3], 46, 0, 153, 62, 80 * scaleFactor, 30 * scaleFactor, 153 * scaleFactor, 62 * scaleFactor, 0);
                    if (vacWarningR) {
                        al_draw_scaled_bitmap(bitmaps[3], 199, 0, 46, 62, 233 * scaleFactor, 30 * scaleFactor, 46 * scaleFactor, 62 * scaleFactor, 0);
                    }
                }

                if (fuelWarningL || fuelWarningR) {
                    if (flashCount % 2 == 0) {
                        if (fuelWarningL) {
                            al_draw_scaled_bitmap(bitmaps[4], 0, 0, 46, 62, 34 * scaleFactor, 90 * scaleFactor, 46 * scaleFactor, 62 * scaleFactor, 0);
                        }
                        al_draw_scaled_bitmap(bitmaps[4], 46, 0, 282, 62, 80 * scaleFactor, 90 * scaleFactor, 282 * scaleFactor, 62 * scaleFactor, 0);
                        if (fuelWarningR) {
                            al_draw_scaled_bitmap(bitmaps[4], 328, 0, 46, 62, 362 * scaleFactor, 90 * scaleFactor, 46 * scaleFactor, 62 * scaleFactor, 0);
                        }
                    }
                }

                if (oilWarning) {
                    al_draw_bitmap(bitmaps[5], 496 * scaleFactor, 29 * scaleFactor, 0);
                }

                if (voltsWarning) {
                    al_draw_bitmap(bitmaps[6], 590 * scaleFactor, 90 * scaleFactor, 0);
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
    else if (!globals.electrics) {
        dimInstrument();
    }
}

void annunciator::showAtcInfo()
{
    char tailNumber[64];
    snprintf(tailNumber, 63, "%s", simVars->atcTailNumber);
    tailNumber[63] = '\0';

    char callSign[64];
    if (simVars->atcFlightNumber[0] == '\0') {
        snprintf(callSign, 63, "%s", simVars->atcCallSign);
    }
    else {
        snprintf(callSign, 63, "%s %s", simVars->atcCallSign, simVars->atcFlightNumber);
    }
    callSign[63] = '\0';

    if (simVars->atcHeavy == 1 && strlen(callSign) < 26) {
        strcat(callSign, " Heavy");
    }

    al_draw_bitmap(bitmaps[9], 0, 0, 0);
    al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), 20, 20, 0, tailNumber);
    al_draw_text(globals.font, al_map_rgb(0x80, 0x80, 0x80), 20, 40, 0, callSign);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void annunciator::update()
{
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        fastAircraft = (loadedAircraft != NO_AIRCRAFT && simVars->cruiseSpeed >= globals.FastAircraftSpeed);
        prevFuelL = 100;
        prevFuelR = 100;
        vacWarningL = false;
        vacWarningR = false;
        fuelWarningL = false;
        fuelWarningR = false;
        oilWarning = false;
        voltsWarning = false;
    }

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

    if (simVars->numberOfEngines == 2) {
        oilPressure = (simVars->oilPressure1 + simVars->oilPressure2) / 2;
    }
    else if (simVars->numberOfEngines > 3) {
        oilPressure = (simVars->oilPressure1 + simVars->oilPressure2 + simVars->oilPressure3 + simVars->oilPressure4) / 4;
    }
    else if (simVars->numberOfEngines == 3) {
        oilPressure = (simVars->oilPressure1 + simVars->oilPressure2 + simVars->oilPressure3) / 3;
    }
    else {
        oilPressure = simVars->oilPressure1;
    }

    if (loadedAircraft != GLIDER) {
        checkFuel(leftPercent, &fuelWarningL, &prevFuelL);
        checkFuel(rightPercent, &fuelWarningR, &prevFuelR);

        // VAC warning if < 1 inHG
        vacWarningL = (simVars->suctionPressure < 1);
        vacWarningR = (simVars->suctionPressure < 1);

        // Oil warning if pressure < 20 PSI
        oilWarning = (oilPressure < 20);

        // Volts warning if battery load too high, i.e. alternator off or not charging quick enough
        voltsWarning = (simVars->batteryLoad > 18);
    }

    // Fuel warning flashes for 10 seconds
    if (flashCount > 0) {
        // Alternate on/off every 500 ms
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
            flashCount--;
            lastFlash = now;
        }
    }
}

void annunciator::checkFuel(double fuelLevel, bool *fuelWarning, double *prevFuel)
{
    double warnPercent;

    if (loadedAircraft == AIRBUS_A310 || loadedAircraft == BOEING_747) {
        // Long haul - Fuel warning at 3%
        warnPercent = 3;
    }
    else if (fastAircraft) {
        // Fuel warning at 6%
        warnPercent = 6;
    }
    else {
        // Fuel warning at 12%
        warnPercent = 12;
    }

    if (fuelLevel == 0 || fuelLevel > warnPercent) {
        *fuelWarning = false;
    }
    else {
        *fuelWarning = true;
        if (*prevFuel > warnPercent) {
            // Flash for 10 seconds
            if (flashCount == 0) {
#ifdef _WIN32
                lastFlash = GetTickCount64();
#else
                clock_gettime(CLOCK_MONOTONIC, &lastFlash);
#endif
            }
            flashCount = 20;
        }
    }

    *prevFuel = fuelLevel;
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

    // This switch has a short so ignore for now
    val = INT_MIN;

    if (val != INT_MIN) {
        selection = 1 - (val % 2);
    }
}

#endif // !_WIN32
