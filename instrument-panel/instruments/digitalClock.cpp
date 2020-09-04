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

    // 3 = Selector arrow
    bmp = al_create_bitmap(52 * scaleFactor, 30 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 0, 52, 30, 0, 0, 52 * scaleFactor, 30 * scaleFactor, 0);
    addBitmap(bmp);

    // 4 = Digit 0
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 5 = Digit 1
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 94, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = Digit 2
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 188, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 7 = Digit 3
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 282, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 8 = Digit 4
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 376, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 9 = Digit 5
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 470, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 10 = Digit 6
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 564, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 11 = Digit 7
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 658, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 12 = Digit 8
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 752, 800, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 13 = Digit 9
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 666, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 14 = Letter E
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 264, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 15 = Letter F
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 398, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 16 = Letter C
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 532, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
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

    // Add selector arrow
    int arrowX;
    int arrowY;

    if (bottomView == 0 || bottomView == 2) {
        arrowX = 111;
    }
    else {
        arrowX = 196;
    }

    if (bottomView < 2) {
        arrowY = 464;
    }
    else {
        arrowY = 550;
    }

    al_draw_bitmap(bitmaps[3], arrowX * scaleFactor, arrowY * scaleFactor, 0);

    switch (topView) {
    case 0:
        // Bus volts
        drawTop(voltsx10 / 100, (voltsx10 / 10) % 10, voltsx10 % 10, 0);
        break;

    case 1:
        // Temp Fahrenheit
        drawTop(tempFx10 / 100, (tempFx10 / 10) % 10, tempFx10 % 10, 1);
        break;

    case 2:
        // Temp Celsius
        drawTop(tempCx10 / 100, (tempCx10 / 10) % 10, tempCx10 % 10, 2);
        break;
    }

    switch (bottomView) {
    case 0:
        // UTC
        drawBottom(utcHours / 10, utcHours % 10, utcMins / 10, utcMins % 10);
        break;

    case 1:
        // Local time
        drawBottom(localHours / 10, localHours % 10, localMins / 10, localMins % 10);
        break;

    case 2:
        // Flight time
        drawBottom(flightHours / 10, flightHours % 10, flightMins / 10, flightMins % 10);
        break;

    case 3:
        // Elapsed time (stopwatch)
        drawBottom(elapsedHours / 10, elapsedHours % 10, elapsedMins / 10, elapsedMins % 10);
        break;
    }

    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Draw top row of digits + letter
/// </summary>
void digitalClock::drawTop(int digit1, int digit2, int digit3, int letter)
{
    int y = 255 * scaleFactor;

    al_draw_bitmap(bitmaps[4 + digit1], 187 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit2], 289 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit3], 417 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[14 + letter], 519 * scaleFactor, y, 0);
}

/// <summary>
/// Draw bottom row of digits
/// </summary>
void digitalClock::drawBottom(int digit1, int digit2, int digit3, int digit4)
{
    int y = 430 * scaleFactor;

    al_draw_bitmap(bitmaps[4 + digit1], 277 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit2], 379 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit3], 507 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit4], 609 * scaleFactor, y, 0);
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
    SimVars* simVars = &globals.simVars->simVars;

    // Calculate values
    voltsx10 = simVars->dcVolts * 10;
    tempFx10 = simVars->dcTempC * 338.0;
    tempCx10 = simVars->dcTempC * 10.0;
    utcHours = simVars->dcUtcSeconds / 3600;
    utcMins = (simVars->dcUtcSeconds - utcHours * 3600.0) / 60;
    localHours = simVars->dcLocalSeconds / 3600;
    localMins = (simVars->dcLocalSeconds - localHours * 3600.0) / 60;
    flightHours = simVars->dcFlightSeconds / 3600;
    flightMins = (simVars->dcFlightSeconds - flightHours * 3600.0) / 60;
    elapsedHours = 0;
    elapsedMins = 0;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void digitalClock::addVars()
{
    globals.simVars->addVar(name, "Zulu Time", false, 60, 43200);
    globals.simVars->addVar(name, "Local Time", false, 60, 46800);
    globals.simVars->addVar(name, "Absolute Time", false, 60, 0);
    globals.simVars->addVar(name, "Electrical Battery Bus Voltage", false, 0.1, 23.7);
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

void digitalClock::updateKnobs()
{
    // Read buttons for clock adjustment
    int val = globals.hardwareKnobs->read(clockButton1);
    if (val != INT_MIN) {
        button1 = val;
    }

    val = globals.hardwareKnobs->read(clockButton2);
    if (val != INT_MIN) {
        button2 = val;
    }

    val = globals.hardwareKnobs->read(clockButton3);
    if (val != INT_MIN) {
        button3 = val;
    }
}

#endif // !_WIN32
