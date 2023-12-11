#include <stdio.h>
#include <stdlib.h>
#include "adiLearjet.h"
#include "knobs.h"

adiLearjet::adiLearjet(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ADI Learjet");
    addVars();
    simVars = &globals.simVars->simVars;

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        addKnobs();
    }
#endif

    resize();
    time(&lastPowerTime);
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void adiLearjet::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("adi-learjet.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

    // 2 = Wheel
    ALLEGRO_BITMAP* wheel = al_create_bitmap(506 * scaleFactor, 600 * scaleFactor);
    addBitmap(wheel);

    // 3 = Graduated transparency for wheel
    ALLEGRO_BITMAP* trans = al_create_bitmap(506 * scaleFactor, 600 * scaleFactor);
    al_set_target_bitmap(trans);
    al_draw_scaled_bitmap(orig, 1311, 100, 506, 600, 0, 0, 506 * scaleFactor, 600 * scaleFactor, 0);
    addBitmap(trans);

    // 4 = Horizon shadow
    ALLEGRO_BITMAP* trans2 = al_create_bitmap(573 * scaleFactor, 110 * scaleFactor);
    al_set_target_bitmap(trans2);
    al_draw_scaled_bitmap(orig, 618, 1629, 573, 110, 0, 0, 573 * scaleFactor, 110 * scaleFactor, 0);
    addBitmap(trans2);

    // 5 = Bezel shadow
    ALLEGRO_BITMAP* trans3 = al_create_bitmap(652 * scaleFactor, 324 * scaleFactor);
    al_set_target_bitmap(trans3);
    al_draw_scaled_bitmap(orig, 509, 1794, 652, 324, 0, 0, 652 * scaleFactor, 324 * scaleFactor, 0);
    addBitmap(trans3);

    // 6 = OFF indicator
    ALLEGRO_BITMAP* trans4 = al_create_bitmap(229 * scaleFactor, 389 * scaleFactor);
    al_set_target_bitmap(trans4);
    al_draw_scaled_bitmap(orig, 1524, 858, 229, 389, 0, 0, 229 * scaleFactor, 389 * scaleFactor, 0);
    addBitmap(trans4);

    // 7 = Roll pointer shadow
    ALLEGRO_BITMAP* trans5 = al_create_bitmap(size, size);
    addBitmap(trans5);

    // 8 = Roll pointer sprite
    ALLEGRO_BITMAP* pointer = al_create_bitmap(100 * scaleFactor, 600 * scaleFactor);
    al_set_target_bitmap(pointer);
    al_draw_scaled_bitmap(orig, 1297, 1388, 99, 599, 0, 0, 100 * scaleFactor, 600 * scaleFactor, 0);
    addBitmap(pointer);

    // 9 = Roll pointer shadow sprite
    ALLEGRO_BITMAP* pointerShadow = al_create_bitmap(100 * scaleFactor, 600 * scaleFactor);
    al_set_target_bitmap(pointerShadow);
    al_draw_scaled_bitmap(orig, 1498, 1386, 99, 599, 0, 0, 100 * scaleFactor, 600 * scaleFactor, 0);
    addBitmap(pointerShadow);

    // 10 = Fill with background colour (takes 1 pixel of background and scales it)
    ALLEGRO_BITMAP* bg = al_create_bitmap(size, size);
    al_set_target_bitmap(bg);
    al_draw_scaled_bitmap(orig, 1267, 20, 1, 1, 0, 0, size, size, 0);
    addBitmap(bg);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void adiLearjet::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Fill with black
    al_draw_scaled_bitmap(bitmaps[0], 628, 850, 1, 1, 0, 0, size, size, 0);

    // Blit wheel into wheel sized bitmap
    al_set_target_bitmap(bitmaps[2]);
    al_draw_scaled_bitmap(bitmaps[0], 0, 900 + (pitchAngle * 5), 506, 600, 0, 0, 506 * scaleFactor, 600 * scaleFactor, 0);

    // Set blender to multiply (shades of grey darken, white has no effect)
    al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
    // Draw graduation to make wheel look round
    al_draw_bitmap(bitmaps[3], 0, 0, 0);
    // Restore normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw bits above and below the wheel
    al_draw_scaled_bitmap(bitmaps[0], 507, 972, 506, 600, 0, 0, 506 * scaleFactor, 600 * scaleFactor, 0);
    al_set_target_bitmap(bitmaps[1]);

    // Draw wheel
    al_draw_rotated_bitmap(bitmaps[2], 253 * scaleFactor, 300 * scaleFactor, 400 * scaleFactor, 400 * scaleFactor, bankAngle * 0.7111111 * DegreesToRadians, 0);

    if (globals.enableShadows)
    {
        // Fill roll pointer shadow sprite with white
        al_set_target_bitmap(bitmaps[7]);
        al_draw_scaled_bitmap(bitmaps[0], 1484, 1387, 1, 1, 0, 0, size, size, 0);

        // Draw roll_pointer shadow into trans5
        al_draw_scaled_rotated_bitmap(bitmaps[9], 50 * scaleFactor, 300 * scaleFactor, 400 * scaleFactor, 415 * scaleFactor, .94, .94, bankAngle * 0.7111111 * DegreesToRadians, 0);
        al_set_target_bitmap(bitmaps[1]);

        // Draw shadows
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Wing pointer shadow
        al_draw_bitmap(bitmaps[4], 186 * scaleFactor, (390 - (currentAdiCal * 10)) * scaleFactor, 0);

        // Bezel shadow
        al_draw_bitmap(bitmaps[5], 70 * scaleFactor, 90 * scaleFactor, 0);

        if (!globals.electrics)
        {
            // OFF indicator shadow
            al_draw_bitmap(bitmaps[6], 18 * scaleFactor, 200 * scaleFactor, 0);
        }

        // Roll pointer shadow
        al_draw_bitmap(bitmaps[7], 0, 0, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Draw Bezel
    al_draw_scaled_bitmap(bitmaps[0], 507, 0, 798, 798, 0, 0, size, size, 0);

    if (!globals.electrics)
    {
        // Draw OFF Indicator
        al_draw_scaled_bitmap(bitmaps[0], 1236, 874, 213, 362, 15 * scaleFactor, 200 * scaleFactor, 213 * scaleFactor, 362 * scaleFactor, 0);
    }

    // Draw wing pointer Indicator
    al_draw_scaled_bitmap(bitmaps[0], 523, 820, 544, 92, 200 * scaleFactor, (380 - (currentAdiCal * 10)) * scaleFactor, 544 * scaleFactor, 92 * scaleFactor, 0);

    // Draw roll pointer
    al_draw_scaled_rotated_bitmap(bitmaps[8], 50 * scaleFactor, 300 * scaleFactor, 400 * scaleFactor, 400 * scaleFactor, .94, .94, bankAngle * 0.7111111 * DegreesToRadians, 0);

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
void adiLearjet::update()
{
    // Check for position or size change
    int* settings = globals.simVars->readSettings(name, xPos, yPos, size);

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

    // If power is off for a random amount of time (between 5 - 7 mins) then simulate gyro stopping
    double timeDifference = 0;
    if (globals.electrics)
    {
        time(&lastPowerTime);

        // Power to go off after 5 - 7 mins
        gyroSpinTime = rand() % 120 + 300;
        failCount = 0;
    }
    else
    {
        time_t now;
        time(&now);

        // Secs power is off
        timeDifference = (double)difftime(now, lastPowerTime);
    }

    if (timeDifference >= gyroSpinTime)
    {
        if (failCount++ <= 200)
        {
            if (pitchAngle < 90)
            {
                pitchAngle += 0.03;
            }

            if (bankAngle < 180)
            {
                bankAngle += 0.3;
            }
        }
        else
        {
            failCount = 201;
        }
    }
    else
    {
        double targetPitch = simVars->adiPitch;
        double diff = abs(targetPitch - pitchAngle);

        if (diff > 80.0) {
            if (pitchAngle < targetPitch) pitchAngle += 20.0; else pitchAngle -= 20.0;
        }
        else if (diff > 40.0) {
            if (pitchAngle < targetPitch) pitchAngle += 10.0; else pitchAngle -= 10.0;
        }
        else if (diff > 20.0) {
            if (pitchAngle < targetPitch) pitchAngle += 5.0; else pitchAngle -= 5.0;
        }
        else if (diff > 10.0) {
            if (pitchAngle < targetPitch) pitchAngle += 2.5; else pitchAngle -= 2.5;
        }
        else if (diff > 5.0) {
            if (pitchAngle < targetPitch) pitchAngle += 1.25; else pitchAngle -= 1.25;
        }
        else if (diff > 0.625) {
            if (pitchAngle < targetPitch) pitchAngle += 0.625; else pitchAngle -= 0.625;
        }
        else {
            pitchAngle = targetPitch;
        }

        double targetBank = simVars->adiBank;
        diff = abs(targetBank - bankAngle);

        // Stop instrument flipping by 360 degrees
        if (diff > 300.0) {
            if (bankAngle < targetBank) bankAngle += 360.0; else bankAngle -= 360.0;
            diff = abs(targetBank - bankAngle);
        }

        if (diff > 80.0) {
            if (bankAngle < targetBank) bankAngle += 20.0; else bankAngle -= 20.0;
        }
        else if (diff > 40.0) {
            if (bankAngle < targetBank) bankAngle += 10.0; else bankAngle -= 10.0;
        }
        else if (diff > 20.0) {
            if (bankAngle < targetBank) bankAngle += 5.0; else bankAngle -= 5.0;
        }
        else if (diff > 10.0) {
            if (bankAngle < targetBank) bankAngle += 2.5; else bankAngle -= 2.5;
        }
        else if (diff > 5.0) {
            if (bankAngle < targetBank) bankAngle += 1.25; else bankAngle -= 1.25;
        }
        else if (diff > 0.625) {
            if (bankAngle < targetBank) bankAngle += 0.625; else bankAngle -= 0.625;
        }
        else {
            bankAngle = targetBank;
        }

        if (currentAdiCal > adiCal && currentAdiCal > -10)
        {
            currentAdiCal -= 1;
        }
        else if (currentAdiCal < adiCal && currentAdiCal < 10)
        {
            currentAdiCal += 1;
        }
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void adiLearjet::addVars()
{
    globals.simVars->addVar(name, "Attitude Indicator Pitch Degrees", false, 1, 0);
    globals.simVars->addVar(name, "Attitude Indicator Bank Degrees", false, 1, 0);
}

#ifndef _WIN32

void adiLearjet::addKnobs()
{
    // BCM GPIO 2 and 3
    //calKnob = globals.hardwareKnobs->add(2, 3, -40, 40, 0);
}

void adiLearjet::updateKnobs()
{
    // Read knob for ADI calibration
    //int val = globals.hardwareKnobs->read(calKnob);

    //if (val != INT_MIN) {
        // Convert knob value to variable (adjust for sensitivity)
        //adiCal = val / 2;
    //}
}

#endif // !_WIN32
