#include <stdio.h>
#include <stdlib.h>
#include "digitalClock.h"
#include "knobs.h"

digitalClock::digitalClock(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Digital Clock");
    addVars();
    simVars = &globals.simVars->simVars;

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        addKnobs();
    }
#endif

    resize();
    time(&flightStartTime);
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
    ALLEGRO_BITMAP* orig = loadBitmap("digital-clock.png");
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

    // 14 = Minus
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 130, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 15 = Letter V
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 264, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 16 = Letter F
    bmp = al_create_bitmap(94 * scaleFactor, 134 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 398, 94, 134, 0, 0, 94 * scaleFactor, 134 * scaleFactor, 0);
    addBitmap(bmp);

    // 17 = Letter C
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

    if (clockView == UtcTime || clockView == FlightTime) {
        arrowX = 111;
    }
    else {
        arrowX = 196;
    }

    if (clockView == UtcTime || clockView == LocalTime) {
        arrowY = 464;
    }
    else {
        arrowY = 550;
    }

    al_draw_bitmap(bitmaps[3], arrowX * scaleFactor, arrowY * scaleFactor, 0);

    switch (displayView) {
    case Voltage:
        drawDisplay(voltsx10 / 100, (voltsx10 / 10) % 10, voltsx10 % 10, 0);
        break;

    case Farenheit:
        if (tempFx10 < 0) {
            drawDisplay(-tempFx10 / 100, (-tempFx10 / 10) % 10, -tempFx10 % 10, 1, true);
        }
        else {
            drawDisplay(tempFx10 / 100, (tempFx10 / 10) % 10, tempFx10 % 10, 1);
        }
        break;

    case Celsius:
        if (tempCx10 < 0) {
            drawDisplay(-tempCx10 / 100, (-tempCx10 / 10) % 10, -tempCx10 % 10, 2, true);
        }
        else {
            drawDisplay(tempCx10 / 100, (tempCx10 / 10) % 10, tempCx10 % 10, 2);
        }
        break;
    }

    switch (clockView) {
    case UtcTime:
        drawClock(utcHours / 10, utcHours % 10, utcMins / 10, utcMins % 10);
        break;

    case LocalTime:
        drawClock(localHours / 10, localHours % 10, localMins / 10, localMins % 10);
        break;

    case FlightTime:
        drawClock(flightHours / 10, flightHours % 10, flightMins / 10, flightMins % 10);
        break;

    case ElapsedTime:
        drawClock(elapsedMins / 10, elapsedMins % 10, elapsedSecs / 10, elapsedSecs % 10);
        break;
    }

    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.electrics) {
        dimInstrument();
    }
}

/// <summary>
/// Draw top row of digits + letter
/// </summary>
void digitalClock::drawDisplay(int digit1, int digit2, int digit3, int letter, bool isMinus)
{
    int y = 255 * scaleFactor;

    // Temps can be negative
    if (isMinus) {
        if (digit1 > 0) {
            al_draw_bitmap(bitmaps[14], 110 * scaleFactor, y, 0);
        }
        else {
            al_draw_bitmap(bitmaps[14], 212 * scaleFactor, y, 0);
        }
    }

    // Farenheit can be > 99
    if (digit1 > 9) {
        al_draw_bitmap(bitmaps[5], 85 * scaleFactor, y, 0);
    }

    if (digit1 > 0) {
        al_draw_bitmap(bitmaps[4 + (digit1 % 10)], 187 * scaleFactor, y, 0);
    }

    al_draw_bitmap(bitmaps[4 + digit2], 289 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[4 + digit3], 417 * scaleFactor, y, 0);
    al_draw_bitmap(bitmaps[15 + letter], 519 * scaleFactor, y, 0);
}

/// <summary>
/// Draw bottom row of digits
/// </summary>
void digitalClock::drawClock(int digit1, int digit2, int digit3, int digit4)
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

    // Calculate values
    voltsx10 = simVars->dcVolts * 10 + 0.5;

    // Smooth out temp change display
    time(&now);
    if (now - lastTempChange > 4) {
        lastTempChange = now;
        tempCx10 = simVars->dcTempC * 10 + .5;
        tempFx10 = 320 + tempCx10 * 1.8;
    }

    int mins = simVars->dcUtcSeconds / 60 + .5;
    utcHours = (mins / 60) % 24;
    utcMins = mins % 60;

    mins = simVars->dcLocalSeconds / 60 + .5;
    localHours = (mins / 60) % 24;
    localMins = mins % 60;

    time(&now);

    // Absoulte time doesn't work so just use panel start time
    mins = (now - flightStartTime) / 60;
    flightHours = (mins / 60) % 24;
    flightMins = mins % 60;

    int seconds = stopWatchSeconds;
    if (stopWatchRunning) {
        seconds += now - stopWatchStarted;
    }

    elapsedMins = (seconds / 60) % 99;
    elapsedSecs = seconds % 60;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void digitalClock::addVars()
{
    globals.simVars->addVar(name, "Zulu Time", false, 60, 43200);
    globals.simVars->addVar(name, "Local Time", false, 60, 46800);
    globals.simVars->addVar(name, "Absolute Time", false, 60, 0);
    globals.simVars->addVar(name, "Electrical Main Bus Voltage", false, 0.1, 23.7);
}

#ifndef _WIN32

void digitalClock::addKnobs()
{
    // BCM GPIO 3
    topButton = globals.hardwareKnobs->add(3, 0, -1, -1, 0);

    // BCM GPIO 4
    leftButton = globals.hardwareKnobs->add(4, 0, -1, -1, 0);

    // BCM GPIO 17
    rightButton = globals.hardwareKnobs->add(17, 0, -1, -1, 0);
}

void digitalClock::updateKnobs()
{
    // Read buttons for clock adjustment
    int val = globals.hardwareKnobs->read(topButton);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevTopVal % 2 == 1) {
            if (displayView == Celsius) {
                displayView = Voltage;
            }
            else {
                displayView = (DisplayView)((int)displayView + 1);
            }
        }
        prevTopVal = val;
    }

    val = globals.hardwareKnobs->read(leftButton);
    if (val != INT_MIN) {
        if (prevLeftVal % 2 == 1) {
            if (clockView == ElapsedTime) {
                clockView = UtcTime;
            }
            else {
                clockView = (ClockView)((int)clockView + 1);
            }
        }
        prevLeftVal = val;
    }

    val = globals.hardwareKnobs->read(rightButton);
    if (val != INT_MIN) {
        if (prevRightVal % 2 == 1) {
            time(&stopWatchPressed);

            if (clockView == ElapsedTime) {
                if (!stopWatchRunning) {
                    // Start
                    stopWatchRunning = true;
                    time(&stopWatchStarted);
                }
                else {
                    // Stop
                    stopWatchRunning = false;
                    time(&now);
                    stopWatchSeconds += now - stopWatchStarted;
                }
            }
        }

        if (val % 2 == 1) {
            stopWatchPressed = 0;
        }
    
        prevRightVal = val;
    }
    else if (stopWatchPressed != 0) {
        // Reset if button held for more than 1 second
        time(&now);
        if (now - stopWatchPressed > 1) {
            if (clockView == FlightTime) {
                // Reset flight time
                time(&flightStartTime);
            }
            else if (clockView == ElapsedTime) {
                // Reset stopwatch
                stopWatchSeconds = 0;
                stopWatchRunning = false;
            }

            stopWatchPressed = 0;
        }
    }
}

#endif // !_WIN32
