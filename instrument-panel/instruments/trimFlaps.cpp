#include <stdio.h>
#include <stdlib.h>
#include "trimFlaps.h"
#include "knobs.h"

trimFlaps::trimFlaps(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Trim Flaps");
    addVars();
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
void trimFlaps::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("trim-flaps.png");
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

    // 5 = Flaps target
    bmp = al_create_bitmap(36 * scaleFactor, 36 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 60, 36, 36, 0, 0, 36 * scaleFactor, 36 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = Landing gear
    bmp = al_create_bitmap(198, 44);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 198, 44, 0, 0, 0);
    addBitmap(bmp);

    // 7 = UNLK
    bmp = al_create_bitmap(104, 37);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 198, 800, 104, 37, 0, 0, 0);
    addBitmap(bmp);

    // 8 = Gear down
    bmp = al_create_bitmap(90, 70);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 302, 800, 900, 70, 0, 0, 0);
    addBitmap(bmp);

    // 9 = Parking brake
    bmp = al_create_bitmap(252, 33);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 392, 807, 252, 33, 0, 0, 0);
    addBitmap(bmp);

    // 10 = Auto brake
    bmp = al_create_bitmap(198, 30);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 392, 840, 198, 30, 0, 0, 0);
    addBitmap(bmp);

    // 11 = Auto brake numbers
    bmp = al_create_bitmap(165, 30);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 590, 840, 165, 30, 0, 0, 0);
    addBitmap(bmp);

    // 12 = Spoilers
    bmp = al_create_bitmap(28, 260);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 804, 545, 28, 260, 0, 0, 0);
    addBitmap(bmp);

    // 13 = Pushback
    bmp = al_create_bitmap(192, 33);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 644, 807, 192, 33, 0, 0, 0);
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
    al_draw_bitmap(bitmaps[3], 262 * scaleFactor, (301 + trimOffset) * scaleFactor, 0);

    // Add flaps target
    al_draw_bitmap(bitmaps[5], 501 * scaleFactor, (161 + targetFlaps) * scaleFactor, 0);

    // Add flaps
    al_draw_bitmap(bitmaps[4], 501 * scaleFactor, (161 + flapsOffset) * scaleFactor, 0);

    if (simVars->gearRetractable) {
        // Add landing gear
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 198, 44, 227 * scaleFactor, 535 * scaleFactor, 198 * scaleFactor, 44 * scaleFactor, 0);

        if (simVars->gearLeftPos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[8], 0, 0, 90, 70, 223 * scaleFactor, 590 * scaleFactor, 90 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearLeftPos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[7], 0, 0, 104, 37, 214 * scaleFactor, 602 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }

        if (simVars->gearCentrePos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[8], 0, 0, 90, 70, 358 * scaleFactor, 590 * scaleFactor, 90 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearCentrePos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[7], 0, 0, 104, 37, 350 * scaleFactor, 602 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }

        if (simVars->gearRightPos > 99.99) {
            // Add gear down
            al_draw_scaled_bitmap(bitmaps[8], 0, 0, 90, 70, 493 * scaleFactor, 590 * scaleFactor, 90 * scaleFactor, 70 * scaleFactor, 0);
        }
        else if (simVars->gearRightPos > 0) {
            // Add UNLK
            al_draw_scaled_bitmap(bitmaps[7], 0, 0, 104, 37, 486 * scaleFactor, 602 * scaleFactor, 104 * scaleFactor, 37 * scaleFactor, 0);
        }
    }

    if (simVars->parkingBrakeOn) {
        // Add parking brake
        al_draw_scaled_bitmap(bitmaps[9], 0, 0, 252, 33, 277 * scaleFactor, 703 * scaleFactor, 252 * scaleFactor, 33 * scaleFactor, 0);
    }
    else if (simVars->pushbackState < 3) {
        // Add pushback
        al_draw_scaled_bitmap(bitmaps[13], 0, 0, 192, 33, 307 * scaleFactor, 703 * scaleFactor, 192 * scaleFactor, 33 * scaleFactor, 0);
    }
    else if (simVars->tfAutoBrake > 1) {
        // Add auto brake
        al_draw_scaled_bitmap(bitmaps[10], 0, 0, 198, 30, 289 * scaleFactor, 705 * scaleFactor, 198 * scaleFactor, 30 * scaleFactor, 0);

        // Add auto brake setting
        al_draw_scaled_bitmap(bitmaps[11], 33 * (simVars->tfAutoBrake - 2), 0, 33, 30, 487 * scaleFactor, 705 * scaleFactor, 33 * scaleFactor, 30 * scaleFactor, 0);
    }

    if (simVars->tfSpoilersPosition > 0) {
        // Add spoilers
        al_draw_scaled_bitmap(bitmaps[12], 0, 0, 28, 260, 412 * scaleFactor, 221 * scaleFactor, 28 * scaleFactor, 260 * scaleFactor, 0);
    }

    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void trimFlaps::update()
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

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    // Calculate values
    trimOffset = simVars->tfElevatorTrim * 20.0;

    if (trimOffset < -150) {
        trimOffset = -150;
    }
    else if (trimOffset > 150) {
        trimOffset = 150;
    }

    targetFlaps = 345.0 * simVars->tfFlapsIndex / simVars->tfFlapsCount;

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

    // If pushing back use rudder to steer
    if (simVars->pushbackState < 3) {
        double tugHeading = simVars->hiHeadingTrue;

        if (simVars->rudderPosition < -0.1) {
            tugHeading += 90;
        }
        else if (simVars->rudderPosition > 0.1) {
            tugHeading -= 90;
        }

        globals.simVars->write(KEY_TUG_HEADING, tugHeading * 11930464);
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void trimFlaps::addVars()
{
    globals.simVars->addVar(name, "Elevator Trim Position", false, 1, 0);
    globals.simVars->addVar(name, "Flaps Num Handle Positions", false, 1, 0);
    globals.simVars->addVar(name, "Flaps Handle Index", false, 1, 0);
    globals.simVars->addVar(name, "Is Gear Retractable", true, 1, 0);
    globals.simVars->addVar(name, "Gear Left Position", false, 1, 0);
    globals.simVars->addVar(name, "Gear Center Position", false, 1, 0);
    globals.simVars->addVar(name, "Gear Right Position", false, 1, 0);
    globals.simVars->addVar(name, "Brake Parking Position", true, 1, 0);
    globals.simVars->addVar(name, "Spoilers Handle Position", false, 1, 0);
    globals.simVars->addVar(name, "Auto Brake Switch Cb", false, 1, 0);
}

#ifndef _WIN32

void trimFlaps::addKnobs()
{
    // BCM GPIO 14 and 15
    trimKnob = globals.hardwareKnobs->add(14, 15, -1, -1, 0);

    // BCM GPIO 18 and 23
    flapsKnob = globals.hardwareKnobs->add(18, 23, -1, -1, 0);
}

void trimFlaps::updateKnobs()
{
    // Read knob for trim adjustment
    int val = globals.hardwareKnobs->read(trimKnob);
    if (val != INT_MIN) {
        if (val > lastTrimVal) {
            globals.simVars->write(KEY_ELEV_TRIM_DN);
            if (fastAircraft) {
                globals.simVars->write(KEY_ELEV_TRIM_DN);
            }
        }
        else if (val < lastTrimVal) {
            globals.simVars->write(KEY_ELEV_TRIM_UP);
            if (fastAircraft) {
                globals.simVars->write(KEY_ELEV_TRIM_UP);
            }
        }
        lastTrimVal = val;
    }

    // Read knob for flaps
    val = globals.hardwareKnobs->read(flapsKnob);
    if (val != INT_MIN) {
        if (val - lastFlapsVal > 1) {
            // Flaps up one notch
            globals.simVars->write(KEY_FLAPS_DECR);
            lastFlapsVal = val;
        }
        else if (lastFlapsVal - val > 1 ) {
            // Flaps down one notch
            globals.simVars->write(KEY_FLAPS_INCR);
            lastFlapsVal = val;
        }
    }
}

#endif // !_WIN32
