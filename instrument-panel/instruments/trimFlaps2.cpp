#include <stdio.h>
#include <stdlib.h>
#include "trimFlaps2.h"

trimFlaps2::trimFlaps2(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Trim Flaps 2");
    addVars();
    simVars = &globals.simVars->simVars;

    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void trimFlaps2::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 600)
    scaleFactor = size / 600.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("trim-flaps2.png");
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
    al_draw_scaled_bitmap(orig, 0, 0, 600, 600, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Landing gear
    bmp = al_create_bitmap(200, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 400, 704, 200, 34, 0, 0, 0);
    addBitmap(bmp);

    // 4 = UNLK
    bmp = al_create_bitmap(104, 37);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 384, 600, 104, 37, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Gear down
    bmp = al_create_bitmap(88, 70);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 489, 600, 88, 70, 0, 0, 0);
    addBitmap(bmp);

    // 6 = Parking brake
    bmp = al_create_bitmap(252, 31);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 676, 252, 31, 0, 0, 0);
    addBitmap(bmp);

    // 7 = Auto brake
    bmp = al_create_bitmap(198, 31);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 708, 198, 31, 0, 0, 0);
    addBitmap(bmp);

    // 8 = Auto brake numbers
    bmp = al_create_bitmap(165, 31);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 198, 708, 165, 31, 0, 0, 0);
    addBitmap(bmp);

    // 9 = Spoilers
    bmp = al_create_bitmap(25, 253);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 600, 150, 25, 253, 0, 0, 0);
    addBitmap(bmp);

    // 10 = Pushback
    bmp = al_create_bitmap(185, 27);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 257, 678, 185, 27, 0, 0, 0);
    addBitmap(bmp);

    // 11 = Red warning
    ALLEGRO_BITMAP* warn = loadBitmap("warn.png");
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(warn, 0, 0, 1, 1, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 12 = Brake
    bmp = al_create_bitmap(131, 31);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 121, 676, 131, 31, 0, 0, 0);
    addBitmap(bmp);

    // 13 = Seatbelts On
    bmp = al_create_bitmap(384, 76);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 600, 384, 76, 0, 0, 0);
    addBitmap(bmp);

    // 14 = Trim
    bmp = al_create_bitmap(30 * scaleFactor, 24 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 600, 0, 30, 24, 0, 0, 30 * scaleFactor, 24 * scaleFactor, 0);
    addBitmap(bmp);

    // 15 = Flaps
    bmp = al_create_bitmap(30 * scaleFactor, 36 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 600, 24, 30, 36, 0, 0, 30 * scaleFactor, 36 * scaleFactor, 0);
    addBitmap(bmp);

    // 16 = Flaps target
    bmp = al_create_bitmap(30 * scaleFactor, 36 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 600, 60, 30, 36, 0, 0, 30 * scaleFactor, 36 * scaleFactor, 0);
    addBitmap(bmp);

    // 17 = Rudder trim
    bmp = al_create_bitmap(16 * scaleFactor, 28 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 600, 102, 16, 28, 0, 0, 16 * scaleFactor, 28 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void trimFlaps2::render()
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

    if (gearUpWarning > 0 && gearUpWarning % 10 < 5) {
        // Add warning flash
        al_draw_bitmap(bitmaps[11], 0, 0, 0);
    }

    // Add trim
    al_draw_bitmap(bitmaps[14], 48 * scaleFactor, (285 + trimOffset) * scaleFactor, 0);

    // Add rudder trim
    al_draw_bitmap(bitmaps[17], (228 + rudderTrimOffset) * scaleFactor, 75 * scaleFactor, 0);

    // Add flaps target
    al_draw_bitmap(bitmaps[16], 506 * scaleFactor, (146 + targetFlaps) * scaleFactor, 0);

    // Add flaps
    al_draw_bitmap(bitmaps[15], 506 * scaleFactor, (146 + flapsOffset) * scaleFactor, 0);

    if (simVars->gearRetractable) {
        // Add landing gear
        al_draw_scaled_bitmap(bitmaps[3], 0, 0, 200, 34, 111 * scaleFactor, 150 * scaleFactor, 200 * scaleFactor, 34 * scaleFactor, 0);

        if (simVars->gearLeftPos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[5], 0, 0, 88, 70, 111 * scaleFactor, 205 * scaleFactor, 88 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearLeftPos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[4], 0, 0, 104, 37, 102 * scaleFactor, 217 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }

        if (simVars->gearCentrePos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[5], 0, 0, 88, 70, 246 * scaleFactor, 205 * scaleFactor, 88 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearCentrePos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[4], 0, 0, 104, 37, 237 * scaleFactor, 217 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }

        if (simVars->gearRightPos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[5], 0, 0, 88, 70, 381 * scaleFactor, 205 * scaleFactor, 88 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearRightPos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[4], 0, 0, 104, 37, 372 * scaleFactor, 217 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }
    }

    if (simVars->parkingBrakeOn) {
        // Add parking brake
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 252, 31, 164 * scaleFactor, 318 * scaleFactor, 252 * scaleFactor, 31 * scaleFactor, 0);
    }
    else if (simVars->brakeLeftPedal > 5 || simVars->brakeRightPedal > 5) {
        // Add brake
        al_draw_scaled_bitmap(bitmaps[12], 0, 0, 131, 31, 225 * scaleFactor, 318 * scaleFactor, 131 * scaleFactor, 31 * scaleFactor, 0);
    }

    if (simVars->pushbackState < 3) {
        // Add pushback
        al_draw_scaled_bitmap(bitmaps[10], 0, 0, 185, 27, 194 * scaleFactor, 384 * scaleFactor, 185 * scaleFactor, 27 * scaleFactor, 0);
    }
    else if (simVars->tfAutoBrake > 1) {
        // Add auto brake
        al_draw_scaled_bitmap(bitmaps[7], 0, 0, 198, 31, 175 * scaleFactor, 384 * scaleFactor, 198 * scaleFactor, 31 * scaleFactor, 0);

        // Add auto brake setting
        al_draw_scaled_bitmap(bitmaps[8], 33 * (simVars->tfAutoBrake - 2), 0, 33, 31, 373 * scaleFactor, 384 * scaleFactor, 33 * scaleFactor, 31 * scaleFactor, 0);
    }

    if (simVars->tfSpoilersPosition > 0) {
        // Add spoilers
        al_draw_scaled_bitmap(bitmaps[9], 0, 0, 25, 253, 555 * scaleFactor, 204 * scaleFactor, 25 * scaleFactor, 253 * scaleFactor, 0);
    }

    if (globals.electrics && simVars->seatBeltsSwitch > 0) {
        // Add seatbelts on
        al_draw_scaled_bitmap(bitmaps[13], 0, 0, 384, 76, 97 * scaleFactor, 460 * scaleFactor, 384 * scaleFactor, 76 * scaleFactor, 0);
    }

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
void trimFlaps2::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        fastAircraft = (loadedAircraft != NO_AIRCRAFT && simVars->cruiseSpeed >= globals.FastAircraftSpeed);
    }

    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Calculate values
    trimOffset = simVars->tfElevatorTrim * 20.0;

    if (trimOffset < -150) {
        trimOffset = -150;
    }
    else if (trimOffset > 150) {
        trimOffset = 150;
    }

    rudderTrimOffset = simVars->tfRudderTrim * 0.93;
    if (loadedAircraft == FBW_A320 && simVars->tfFlapsCount == 5) {
        targetFlaps = 345.0 * simVars->tfFlapsIndex / 4.0;
    }
    else {
        targetFlaps = 345.0 * simVars->tfFlapsIndex / simVars->tfFlapsCount;
    }
    double diff = abs(targetFlaps - flapsOffset);

    if (diff > 5.0) {
        if (flapsOffset < targetFlaps) flapsOffset += 2.5; else flapsOffset -= 2.5;
    }
    else if (diff > 1.25) {
        if (flapsOffset < targetFlaps) flapsOffset += 1.25; else flapsOffset -= 1.25;
    }
    else {
        flapsOffset = targetFlaps;
    }

    // Warn if low altitude and flaps down but gear is up
    if (simVars->tfFlapsIndex > 0 && simVars->altAboveGround < 400 && simVars->altAboveGround > 30 && simVars->gearRetractable
        && simVars->gearLeftPos < 20 && simVars->vsiVerticalSpeed < 0)
    {
        if (simVars->altAboveGround > 300) {
            // Warning 1
            gearUpWarning++;
        }
        else if (simVars->altAboveGround < 100) {
            // Warning 2
            gearUpWarning++;
        }
        else {
            gearUpWarning = 0;
        }
    }
    else {
        gearUpWarning = 0;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void trimFlaps2::addVars()
{
}
