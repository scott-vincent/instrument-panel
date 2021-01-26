#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "hi.h"
#include "simvars.h"
#include "knobs.h"

hi::hi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("HI");
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
void hi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("hi.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Dial
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 0, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 3 = Plane
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 4 = Heading bug
    bmp = al_create_bitmap(80, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1600, 0, 80, 80, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Heading bug shadow
    bmp = al_create_bitmap(80, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1600, 400, 80, 80, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void hi::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add dial
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

    // Add plane
    al_draw_bitmap(bitmaps[3], 0, 0, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add heading bug shadow
        al_draw_scaled_rotated_bitmap(bitmaps[5], 40, 400, 410 * scaleFactor, 408 * scaleFactor, scaleFactor, scaleFactor, bugAngle * DegreesToRadians, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add heading bug
    al_draw_scaled_rotated_bitmap(bitmaps[4], 40, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, bugAngle * DegreesToRadians, 0);

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
void hi::update()
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
    angle = -simVars->hiHeading;

    // Only update local value from sim if it is not currently being
    // adjusted by the rotary encoder. This stops the displayed value
    // from jumping around due to lag of fetch/update cycle.
    if (lastBugAdjust == 0) {
        headingBug = simVars->autopilotHeading;
    }

    // Show bug setting to nearest 5 degrees
    int bugSetting = (int)((headingBug + 2.5) / 5) * 5;
    bugAngle = bugSetting - simVars->hiHeading;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void hi::addVars()
{
    globals.simVars->addVar(name, "Plane Heading Degrees Magnetic", false, 1, 0);
}

#ifndef _WIN32

void hi::addKnobs()
{
    // BCM GPIO 13 and 6
    hdgKnob = globals.hardwareKnobs->add(13, 6, -1, -1, 0);
}

void hi::updateKnobs()
{
    // Read knob for new instrument calibration
    int val = globals.hardwareKnobs->read(hdgKnob);

    if (val != INT_MIN) {
        // Convert knob value to heading (adjust for desired sensitivity)
        int adjust = ((int)(val - prevVal) / 2) * 5;
        if (adjust != 0) {
            int newHeading = headingBug += adjust;
            if (newHeading > 359) {
                newHeading -= 360;
            }
            else if (newHeading < 0) {
                newHeading += 360;
            }
            globals.simVars->write(KEY_HEADING_BUG_SET, newHeading);
            prevVal = val;
        }
        time(&lastBugAdjust);
    }
    else if (lastBugAdjust != 0) {
        time(&now);
        if (now - lastBugAdjust > 1) {
            lastBugAdjust = 0;
        }
    }
}

#endif // !_WIN32
