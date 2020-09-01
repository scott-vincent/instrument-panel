#include <stdio.h>
#include <stdlib.h>
#include "alt.h"
#include "simvars.h"
#include "knobs.h"

alt::alt(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ALT");
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
void alt::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("alt.bmp");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Outer scale
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 3 = Inner scale
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 800, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 4 = 1000ft pointer
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 5 = 100ft pointer
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1600, 0, 100, 800, 0, 0, 0);
    addBitmap(bmp);

    // 6 = 1ft pointer
    bmp = al_create_bitmap(100, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1600, 800, 100, 800, 0, 0, 0);
    addBitmap(bmp);

    // 7 = Inner hole shadow
    bmp = al_create_bitmap(200 * scaleFactor, 200 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1700, 0, 200, 200, 0, 0, 200 * scaleFactor, 200 * scaleFactor, 0);
    addBitmap(bmp);

    // 8 = Outer hole shadow
    bmp = al_create_bitmap(200 * scaleFactor, 200 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1700, 200, 200, 200, 0, 0, 200 * scaleFactor, 200 * scaleFactor, 0);
    addBitmap(bmp);

    // 9 = Pointer hole shadow
    bmp = al_create_bitmap(200, 200);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1700, 400, 200, 200, 0, 0, 0);
    addBitmap(bmp);

    // 10 = Main dial
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void alt::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add outer scale (inches of mercury) and rotate
    // 29.5 = 0 radians
    angle = (29.5f - inhg) * 1.8f;
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle, 0);

    // Add inner scale (millibars) and rotate
    // 1000 = 0 radians
    angle = (1000.0f - mb) * 0.0525f;
    al_set_blender(ALLEGRO_ADD, ALLEGRO_INVERSE_DEST_COLOR, ALLEGRO_ONE);
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle, 0);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add inner hole shadow
        al_draw_bitmap_region(bitmaps[7], 0, 0, 200, 200, 60 * scaleFactor, 310 * scaleFactor, 0);

        // Add outer hole Shadow
        al_draw_bitmap_region(bitmaps[8], 0, 0, 200, 200, 610 * scaleFactor, 310 * scaleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add main dial
    al_draw_bitmap(bitmaps[10], 0, 0, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer hole shadow
        al_draw_scaled_bitmap(bitmaps[9], 0, 0, 200, 200, 310 * scaleFactor, 410 * scaleFactor, 200 * scaleFactor, 200 * scaleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add 1000ft pointer
    // 0 = 0 radians
    angle = altitude * 0.0062832f;
    al_draw_scaled_rotated_bitmap(bitmaps[4], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle / 100, 0);

    // Add 100ft pointer
    // 0 = 0 radians
    al_draw_scaled_rotated_bitmap(bitmaps[5], 50, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle / 10, 0);

    // Add 1ft pointer
    al_draw_scaled_rotated_bitmap(bitmaps[6], 50, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle, 0);

    // Position dest bitmap on screen
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
void alt::update()
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

    // Add altitude correction
    double altitudeTarget = simVars->altAltitude + 1000 * (29.92 - simVars->altKollsman);

    mb = simVars->altKollsman * 33.86389;
    inhg = simVars->altKollsman;

    if (altitudeTarget < 0) {
        altitudeTarget = 0;
    }

    if (altitude - altitudeTarget > 500) altitude -= 200;
    else if (altitudeTarget - altitude > 500) altitude += 200;
    else if (altitude - altitudeTarget > 100) altitude -= 40;
    else if (altitudeTarget - altitude > 100) altitude += 40;
    else if (altitude - altitudeTarget > 50) altitude -= 20;
    else if (altitudeTarget - altitude > 50) altitude += 20;
    else if (altitude - altitudeTarget > 10) altitude -= 5;
    else if (altitudeTarget - altitude > 10) altitude += 5;
    else if (altitude - altitudeTarget > 5) altitude -= 2;
    else if (altitudeTarget - altitude > 5) altitude += 2;
    else if (altitude - altitudeTarget > 0) altitude -= 1;
    else if (altitudeTarget - altitude > 0) altitude += 1;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void alt::addVars()
{
    globals.simVars->addVar(name, "Plane Altitude", false, 10, 0);
    globals.simVars->addVar(name, "Kohlsman Setting Hg", false, 0.01, 29.92f);
}

#ifndef _WIN32

void alt::addKnobs()
{
    // BCM GPIO 2 and 3
    calKnob = globals.hardwareKnobs->add(2, 3, 15168, 17344, 16208);
}

void alt::updateKnobs()
{
    // Read knob for pressure calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Convert knob value to pressure (adjust for desired sensitivity)
        double pressure2 = val;

        // Update manual pressure adjust
        //globals.simVars->write("pressure2", pressure2);
    }
}

#endif // !_WIN32
