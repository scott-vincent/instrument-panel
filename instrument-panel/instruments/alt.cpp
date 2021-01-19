#include <stdio.h>
#include <stdlib.h>
#include "alt.h"
#include "genericFast/altFast.h"
#include "simvars.h"
#include "knobs.h"

alt::alt(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ALT");
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
void alt::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("alt.png");
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
    if (bitmaps[0] == NULL || loadedAircraft != globals.aircraft) {
        return;
    }

    if (customInstrument) {
        customInstrument->render();
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
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        fastAircraft = (loadedAircraft != NO_AIRCRAFT && simVars->cruiseSpeed >= globals.FastAircraftSpeed);

        // Load custom instrument for this aircraft if we have one
        if (customInstrument) {
            delete customInstrument;
            customInstrument = NULL;
        }

        if (fastAircraft) {
            customInstrument = new altFast(xPos, yPos, size, name);
        }
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    if (customInstrument) {
        customInstrument->update();
        return;
    }

    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2] || aircraftChanged) {
        size = settings[2];
        resize();
    }

    // Only update local value from sim if it is not currently being
    // adjusted by the rotary encoder. This stops the displayed value
    // from jumping around due to lag of fetch/update cycle.
    if (lastCalAdjust == 0) {
        inhg = simVars->altKollsman;
    }

    // Calculate values
    mb = inhg * 33.86389;

    double diff = abs(simVars->altAltitude - altitude);

    if (diff > 4000.0) {
        altitude = simVars->altAltitude;
    }
    else if (diff > 500.0) {
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
void alt::addVars()
{
    globals.simVars->addVar(name, "Indicated Altitude", false, 10, 0);
    globals.simVars->addVar(name, "Kohlsman Setting Hg", false, 0.01, 29.92f);
}

#ifndef _WIN32

void alt::addKnobs()
{
    // BCM GPIO 10 and 9
    calKnob = globals.hardwareKnobs->add(10, 9, -1, -1, 0);
}

void alt::updateKnobs()
{
    // Read knob for pressure calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Change calibration by knob movement amount (adjust for desired sensitivity)
        double adjust = (int)((val - prevVal) / 2) * 0.01;
        if (adjust != 0) {
            inhg += adjust;
            double newVal = inhg * 541.82224;

            globals.simVars->write(KEY_KOHLSMAN_SET, newVal);
            prevVal = val;
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
