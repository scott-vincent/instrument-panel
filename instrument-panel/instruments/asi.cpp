#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "asi.h"
#include "alternate/asiFast.h"
#include "alternate/asiSupersonic.h"
#include "savageCub/asiSavageCub.h"
#include "spitfire/asiSpitfire.h"
#include "glider/asiGlider.h"
#include "simvars.h"
#include "knobs.h"

asi::asi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ASI");
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
void asi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi.png");
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

    // 3 = Outer scale shadow
    bmp = al_create_bitmap(size, 180 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 1600, 800, 180, 0, 0, size, 180 * scaleFactor, 0);
    addBitmap(bmp);

    // 4 = Pointer
    bmp = al_create_bitmap(80, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 80, 800, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Pointer shadow
    bmp = al_create_bitmap(80, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 800, 80, 800, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void asi::render()
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

    // Add outer scale (adjusted airspeed) and rotate
    // 0 = 0 radians
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, airspeedCal * DegreesToRadians, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add outer hole Shadow
        al_draw_bitmap_region(bitmaps[3], 0, 0, size, 180 * scaleFactor, 10 * scaleFactor, 630 * scaleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add main dial
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, size, size, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[5], 40, 400, 410 * scaleFactor, 410 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[4], 40, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, airspeedAngle, 0);

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
void asi::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        fastAircraft = (loadedAircraft != NO_AIRCRAFT && loadedAircraft != JUSTFLIGHT_PA28
            && simVars->cruiseSpeed >= globals.FastAircraftSpeed);
        supersonicAircraft = (loadedAircraft == F15_EAGLE || loadedAircraft == F18_HORNET || loadedAircraft == HAWKER_HUNTER);
        cabinLights = 0;

        // Load custom instrument for this aircraft if we have one
        if (customInstrument) {
            delete customInstrument;
            customInstrument = NULL;
        }

        if (loadedAircraft == SAVAGE_CUB) {
            customInstrument = new asiSavageCub(xPos, yPos, size, name);
        }
        else if (loadedAircraft == SUPERMARINE_SPITFIRE) {
            customInstrument = new asiSpitfire(xPos, yPos, size, name);
        }
        else if (loadedAircraft == GLIDER) {
            customInstrument = new asiGlider(xPos, yPos, size, name);
        }
        else if (supersonicAircraft) {
            customInstrument = new asiSupersonic(xPos, yPos, size, name);
        }
        else if (fastAircraft) {
            customInstrument = new asiFast(xPos, yPos, size, name);
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

    // Calculate values
    airspeedCal = -35 - (simVars->asiAirspeedCal * 2.5);
    airspeedKnots = simVars->asiAirspeed;

    // Not a linear scale!
    if (airspeedKnots < 40) {
        airspeedAngle = airspeedKnots * 0.013;
    }
    else if (airspeedKnots < 90) {
        airspeedAngle = -0.03 + pow(airspeedKnots - 12.1, 1.439) * 0.0047;
    }
    else if (airspeedKnots < 120) {
        airspeedAngle = 0.4 + pow(airspeedKnots - 12.1, 1.4) * 0.0046;
    }
    else if (airspeedKnots < 160) {
        airspeedAngle = 1.53 + pow(airspeedKnots - 12.0, 1.320) * 0.0043;
    }
    else {
        airspeedAngle = 2.27 + pow(airspeedKnots - 12.0, 1.28) * 0.0040;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void asi::addVars()
{
    globals.simVars->addVar(name, "Airspeed Indicated", false, 1, 0);
    globals.simVars->addVar(name, "Airspeed True Calibrate", false, 1, 0);
    globals.simVars->addVar(name, "Airspeed Mach", false, 100, 0);
}

#ifndef _WIN32

void asi::addKnobs()
{
    // BCM GPIO 27 and 22
    calKnob = globals.hardwareKnobs->add(27, 22, -1, -1, 0);
}

void asi::updateKnobs()
{
    // Read knob for airspeed calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        if (loadedAircraft == FBW_A320) {
            // Use knob for interior lighting instead
            double adjust = val - prevVal;
            if (adjust != 0) {
                cabinLights += adjust;
                if (cabinLights < 0) {
                    cabinLights = 0;
                }
                else if (cabinLights > 100) {
                    cabinLights = 100;
                }
                globals.simVars->write(KEY_CABIN_LIGHTS_SET, cabinLights);
                prevVal = val;
            }
        }
        else {
            // Change calibration by knob movement amount (adjust for desired sensitivity)
            int adjust = (int)((prevVal - val) / 2);
            if (adjust != 0) {
                double newVal = simVars->asiAirspeedCal + adjust;

                globals.simVars->write(KEY_TRUE_AIRSPEED_CAL_SET, newVal);
                prevVal = val;
            }
        }
    }
}

#endif // !_WIN32
