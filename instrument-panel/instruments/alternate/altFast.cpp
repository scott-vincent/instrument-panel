#include <stdio.h>
#include <stdlib.h>
#include "altFast.h"
#include "knobs.h"

altFast::altFast(int xPos, int yPos, int size, const char* parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size, vars and knobs from parent
        hasParent = true;
        setName(parentName);
    }
    else {
        hasParent = false;
        setName("ALT Fast");
        addVars();

#ifndef _WIN32
        // Only have hardware knobs on Raspberry Pi
        if (globals.hardwareKnobs) {
            addKnobs();
        }
#endif
    }

    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void altFast::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("alt-fast.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* dest = al_create_bitmap(size, size);
    addBitmap(dest);

    // 2 = Graduated transparency for altitude reels
    ALLEGRO_BITMAP* trans = al_create_bitmap(78 * scaleFactor, 393 * scaleFactor);
    addBitmap(trans);

    // 3 = Graduated transparency for pressure reels
    ALLEGRO_BITMAP* trans2 = al_create_bitmap(45 * scaleFactor, 162 * scaleFactor);
    addBitmap(trans2);

    // 4 = Shadow background
    ALLEGRO_BITMAP* shadowBackground = al_create_bitmap(800 * scaleFactor, 800 * scaleFactor);
    addBitmap(shadowBackground);

    // 5 = Shadow sprite
    ALLEGRO_BITMAP* shadow = al_create_bitmap(43 * scaleFactor, 562 * scaleFactor);
    al_set_target_bitmap(shadow);
    al_draw_scaled_bitmap(orig, 192, 649, 43, 562, 0, 0, 43 * scaleFactor, 562 * scaleFactor, 0);
    addBitmap(shadow);

    // 6 = Fill with background colour (takes 1 pixel of background and scales it)
    ALLEGRO_BITMAP* bg = al_create_bitmap(size, size);
    al_set_target_bitmap(bg);
    al_draw_scaled_bitmap(orig, 75, 61, 1, 1, 0, 0, size, size, 0);
    addBitmap(bg);

    // 7 = Pointer sprite
    ALLEGRO_BITMAP* pointer = al_create_bitmap(43, 562);
    al_set_target_bitmap(pointer);
    al_draw_bitmap_region(orig, 148, 649, 43, 562, 0, 0, 0);
    addBitmap(pointer);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void altFast::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Fill with black
    al_draw_scaled_bitmap(bitmaps[0], 240, 312, 1, 1, 0, 0, size, size, 0);

    // Copy graduated pattern artwork into trans2 bitmap for the small digits
    int destWidth = 45 * scaleFactor;
    int destHeight = 162 * scaleFactor;
    al_set_target_bitmap(bitmaps[3]);
    al_draw_scaled_bitmap(bitmaps[0], 289, 0, 262, 254, 0, 0, destWidth, destHeight, 0);
    al_set_target_bitmap(bitmaps[1]);

    // Copy graduated pattern artwork into trans bitmap
    al_set_target_bitmap(bitmaps[2]);
    al_draw_scaled_bitmap(bitmaps[0], 289, 0, 262, 254, 0, 0, 78 * scaleFactor, 393 * scaleFactor, 0);
    al_set_target_bitmap(bitmaps[1]);

    // Add hPa Reels
    int val = (inhg * 33.86378746435) + 0.5;
    addSmallNumber(157 * scaleFactor, val / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10);

    // Draw InHg Reels
    val = (inhg * 100.0) + 0.5;
    addSmallNumber(465 * scaleFactor, val / 1000, (val % 1000) / 100, (val % 100) / 10, val % 10);

    // Draw Altitude reels
    int offset = (int)(10 * (altitude + 3)) % 100;
    val = (altitude + 3) / 10;

    int digit = val / 1000;
    int yPos1 = 262 + 131 * (8 - digit);
    if (val % 1000 == 999) {
        yPos1 -= offset;
    }

    digit = (val % 1000) / 100;
    int yPos2 = 262 + 131 * (8 - digit);
    if (val % 100 == 99) {
        yPos2 -= offset;
    }

    digit = (val % 100) / 10;
    int yPos3 = 262 + 131 * (8 - digit);
    if (val % 10 == 9) {
        yPos3 -= offset;
    }

    addLargeNumber(yPos1, yPos2, yPos3);

    // Set blender to multiply (shades of grey darken, white has no effect)
    al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

    // hPa Reels overlay graduated transparencies
    addSmallShadow(157 * scaleFactor);

    // IN HG Reels overlay the graduated transparencies
    addSmallShadow(465 * scaleFactor);

    // Altitutude Reels overlay the graduated transparencies
    addLargeShadow();

    // Restore normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw electrics on/off
    al_draw_scaled_bitmap(bitmaps[0], 194, 0, 95, 31, 277 * scaleFactor, 681 * scaleFactor, 98 * scaleFactor, 33 * scaleFactor, 0);

    // Display Dial
    al_draw_scaled_bitmap(bitmaps[0], 240, 312, 800, 800, 0, 0, size, size, 0);

    if (globals.aircraft == F15_EAGLE) {
        // Needle shows 1000's of feet
        angle = (altitude - (int)(altitude / 1000) * 1000.0) * 0.36 * DegreesToRadians;
    }
    else {
        // Needle shows 100's of feet
        angle = (altitude - (int)(altitude / 100) * 100.0) * 3.6 * DegreesToRadians;
    }

    if (globals.enableShadows) {
        // Fill shadow_background bitmap with white
        al_set_target_bitmap(bitmaps[4]);
        al_draw_scaled_bitmap(bitmaps[0], 192, 649, 1, 1, 0, 0, size, size, 0);

        // Write shadow sprite to shadow background
        al_draw_rotated_bitmap(bitmaps[5], 21 * scaleFactor, 322 * scaleFactor, 410 * scaleFactor, 420 * scaleFactor, angle, 0);
        al_set_target_bitmap(bitmaps[1]);

        // Draw shadow
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
        al_draw_bitmap(bitmaps[4], 0, 0, 0);
        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    //Draw needle pointer at angle
    al_draw_scaled_rotated_bitmap(bitmaps[7], 21, 322, size / 2, size / 2, scaleFactor, scaleFactor, angle, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.electrics) {
        dimInstrument();
    }
}

void altFast::addSmallNumber(int yPos, int digit1, int digit2, int digit3, int digit4)
{
    int destWidth = 45 * scaleFactor;
    int destHeight = 162 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[0], 148, 54 * (9 - digit1), 45, 162, 486 * scaleFactor, yPos, destWidth, destHeight, 0);
    al_draw_scaled_bitmap(bitmaps[0], 148, 54 * (9 - digit2), 45, 162, 532 * scaleFactor, yPos, destWidth, destHeight, 0);
    al_draw_scaled_bitmap(bitmaps[0], 148, 54 * (9 - digit3), 45, 162, 578 * scaleFactor, yPos, destWidth, destHeight, 0);
    al_draw_scaled_bitmap(bitmaps[0], 148, 54 * (9 - digit4), 45, 162, 624 * scaleFactor, yPos, destWidth, destHeight, 0);
}

void altFast::addSmallShadow(int yPos)
{
    al_draw_bitmap(bitmaps[3], 486 * scaleFactor, yPos, 0);
    al_draw_bitmap(bitmaps[3], 532 * scaleFactor, yPos, 0);
    al_draw_bitmap(bitmaps[3], 578 * scaleFactor, yPos, 0);
    al_draw_bitmap(bitmaps[3], 624 * scaleFactor, yPos, 0);
}

void altFast::addLargeNumber(int yPos1, int yPos2, int yPos3)
{
    int destY = 202 * scaleFactor;
    int destWidth = 78 * scaleFactor;
    int destHeight = 393 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[0], 0, yPos1, 73, 393, 70 * scaleFactor, destY, destWidth, destHeight, 0);
    al_draw_scaled_bitmap(bitmaps[0], 74, yPos2, 73, 393, 151 * scaleFactor, destY, destWidth, destHeight, 0);
    al_draw_scaled_bitmap(bitmaps[0], 74, yPos3, 73, 393, 233 * scaleFactor, destY, destWidth, destHeight, 0);
}

void altFast::addLargeShadow()
{
    int destY = 202 * scaleFactor;

    al_draw_bitmap(bitmaps[2], 70 * scaleFactor, destY, 0);
    al_draw_bitmap(bitmaps[2], 151 * scaleFactor, destY, 0);
    al_draw_bitmap(bitmaps[2], 233 * scaleFactor, destY, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void altFast::update()
{
    updateCustom(inhg);
}

void altFast::updateCustom(double inhgVal)
{
    inhg = inhgVal;

    // Check for position or size change
    int *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (!hasParent && globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    if (!hasParent && lastCalAdjust == 0) {
        inhg = simVars->altKollsman;
    }

    // Calculate values
    double diff = abs(simVars->altAltitude - altitude);

    if (diff > 500.0) {
        if (altitude < simVars->altAltitude) altitude += 200.0; else altitude -= 200.0;
    }
    else if (diff > 100.0) {
        if (altitude < simVars->altAltitude) altitude += 40.0; else altitude -= 40.0;
    }
    else if (diff > 50.0) {
        if (altitude < simVars->altAltitude) altitude += 20.0; else altitude -= 20.0;
    }
    else if (diff > 10.0) {
        if (altitude < simVars->altAltitude) altitude += 5.0; else altitude -= 5.0;
    }
    else if (diff > 5.0) {
        if (altitude < simVars->altAltitude) altitude += 2.0; else altitude -= 2.0;
    }
    else if (diff > 1) {
        if (altitude < simVars->altAltitude) altitude += 1.0; else altitude -= 1.0;
    }
    else {
        altitude = simVars->altAltitude;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void altFast::addVars()
{
    globals.simVars->addVar(name, "Indicated Altitude", false, 10, 0);
    globals.simVars->addVar(name, "Kohlsman Setting Hg", false, 0.01, 29.92);
}

#ifndef _WIN32

void altFast::addKnobs()
{
    // BCM GPIO 10 and 9
    calKnob = globals.hardwareKnobs->add(10, 9, -1, -1, 0);
}

void altFast::updateKnobs()
{
    // Read knob for pressure calibration
    int val = globals.hardwareKnobs->read(calKnob);
    int diff = (val - prevVal) / 2;
    bool switchBox = false;

    if (prevValSb == 0) {
        prevValSb = simVars->sbEncoder[2];
    }
    else if (simVars->sbEncoder[2] != prevValSb) {
        val = simVars->sbEncoder[2];
        diff = val - prevValSb;
        switchBox = true;
    }

    if (val != INT_MIN) {
        // Change calibration by knob movement amount (adjust for desired sensitivity)
        double adjust = diff * 0.01;
        if (adjust != 0) {
            inhg += adjust;
            if (inhg < 28 || inhg >= 31.01) {
                inhg -= adjust;
            }
            double newVal = inhg * 33.86378746435 * 16;
            globals.simVars->write(KEY_KOHLSMAN_SET, newVal);
            if (switchBox) {
                prevValSb = val;
            }
            else {
                prevVal = val;
            }
        }
        time(&lastCalAdjust);
    }
    else if (lastCalAdjust != 0) {
        time(&now);
        if (now - lastCalAdjust > 1) {
            lastCalAdjust = 0;
        }
    }
}

#endif // !_WIN32
