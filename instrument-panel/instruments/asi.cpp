#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "asi.h"
#include "simvars.h"
#include "knobs.h"

asi::asi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ASI");
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
void asi::resize()
{
    destroyBitmaps();

    if (globals.simVars->simVars.cruiseSpeed >= FastPlaneSpeed) {
        resizeFast();
        return;
    }

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    char origName[256] = "asi.bmp";

    if (globals.aircraft == globals.SAVAGE_CUB) {
        strcpy(origName, "asi-savage-cub.bmp");
    }

    loadedAircraft = globals.aircraft;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap(origName);
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
/// Same as resize but for faster planes so that
/// higher speeds can be displayed.
/// </summary>
void asi::resizeFast()
{
    int fsize = size * FastPlaneSizeFactor;

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = fsize / 800.0f;

    loadedAircraft = globals.aircraft;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi-fast.bmp");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(fsize, fsize);
    addBitmap(bmp);

    // 2 = Outer (mach) dial
    ALLEGRO_BITMAP* outer = al_create_bitmap(800, 800);
    al_set_target_bitmap(outer);
    al_draw_bitmap_region(orig, 801, 0, 800, 800, 0, 0, 0);
    addBitmap(outer);

    // 3 = Main dial shadow
    ALLEGRO_BITMAP* shadowBackground = al_create_bitmap(800, 800);
    al_set_target_bitmap(shadowBackground);
    al_draw_scaled_bitmap(orig, 801, 801, 800, 800, 0, 0, fsize, fsize, 0);
    addBitmap(shadowBackground);

    // 4 = Shadow sprite (to be rotated)
    ALLEGRO_BITMAP* shadowBackground2 = al_create_bitmap(800, 800);
    addBitmap(shadowBackground2);

    // 5 = Shadow sprite
    ALLEGRO_BITMAP* shadow = al_create_bitmap(149 * scaleFactor, 609 * scaleFactor);
    al_set_target_bitmap(shadow);
    al_draw_scaled_bitmap(orig, 513, 863, 149, 609, 0, 0, 149 * scaleFactor, 609 * scaleFactor, 0);
    addBitmap(shadow);

    // 6 = Pointer sprite
    ALLEGRO_BITMAP* pointer = al_create_bitmap(111, 581);
    al_set_target_bitmap(pointer);
    al_draw_bitmap_region(orig, 153, 881, 111, 581, 0, 0, 0);
    addBitmap(pointer);

    // 7 = Fill with background colour (takes 1 pixel of background and scales it)
    ALLEGRO_BITMAP* bg = al_create_bitmap(fsize, fsize);
    al_set_target_bitmap(bg);
    al_draw_scaled_bitmap(orig, 75, 61, 1, 1, 0, 0, fsize, fsize, 0);
    addBitmap(bg);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void asi::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    if (globals.simVars->simVars.cruiseSpeed >= FastPlaneSpeed) {
        renderFast();
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

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Same as render but for faster planes.
/// </summary>
void asi::renderFast()
{
    int fsize = size * FastPlaneSizeFactor;

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Draw background colour
    al_draw_bitmap(bitmaps[7], 0, 0, 0);

    // Display outer dial
    float centre = fsize / 2.0f;
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, centre, centre, scaleFactor, scaleFactor, machAngle * AngleFactor, 0);

    if (globals.enableShadows) {
        // Display main dial shadow
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
        al_draw_bitmap(bitmaps[3], 0, 0, 0);
        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Display dial (contains transparencies) over outer dial
    al_draw_scaled_bitmap(bitmaps[0], 0, 0, 800, 800, 0, 0, fsize, fsize, 0);

    if (globals.enableShadows) {
        // Fill shadow_background bitmap with white
        al_set_target_bitmap(bitmaps[4]);
        al_draw_scaled_bitmap(bitmaps[0], 513, 863, 1, 1, 0, 0, fsize, fsize, 0);

        // Write shadow sprite to shadow background
        al_draw_rotated_bitmap(bitmaps[5], 76 * scaleFactor, 367 * scaleFactor, 410 * scaleFactor, 430 * scaleFactor, airspeedAngle * AngleFactor, 0);
        al_set_target_bitmap(bitmaps[1]);

        // Draw shadow
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
        al_draw_bitmap(bitmaps[4], 0, 0, 0);
        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Draw needle pointer at angle
    al_draw_scaled_rotated_bitmap(bitmaps[6], 54, 348, centre, centre, scaleFactor, scaleFactor, airspeedAngle * AngleFactor, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);

    int fadjust = (fsize - size) / 2;
    al_draw_bitmap(bitmaps[1], xPos - fadjust, yPos - fadjust, 0);

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void asi::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2] || loadedAircraft != globals.aircraft) {
        size = settings[2];
        resize();
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    if (globals.simVars->simVars.cruiseSpeed >= FastPlaneSpeed) {
        updateFast();
        return;
    }

    // Get latest FlightSim variables
    SimVars* simVars = &globals.simVars->simVars;

    airspeedCal = -35 - (simVars->asiAirspeedCal * 2.5);

    // Calculate values - Not a linear scale!
    if (globals.aircraft == globals.SAVAGE_CUB) {
        airspeedKnots = simVars->asiAirspeed * 2;
    }
    else {
        airspeedKnots = simVars->asiAirspeed;
    }

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
/// Same as update but for faster planes.
/// </summary>
void asi::updateFast()
{
    // Get latest FlightSim variables
    SimVars* simVars = &globals.simVars->simVars;

    // Calculate airspeed angle
    double speed = simVars->asiAirspeed / 10.0f;

    if (speed > 400) {
        targetAirspeedAngle = 233.65;
    }
    else if (speed > 0) {
        targetAirspeedAngle = 0.0000052772 * pow(speed, 5) - 0.0003338205 * pow(speed, 4) - 0.0025509435 * pow(speed, 3) + 0.482328783 * pow(speed, 2) - 1.5416690771 * speed + 1.0500510789;
        // Angle is not accurate below 3
        if (targetAirspeedAngle < 3) {
            targetAirspeedAngle = 0;
        }
    }
    else {
        targetAirspeedAngle = 0;
    }

    // Smooth out airspeed adjustment at low speed
    if (targetAirspeedAngle < 5 && abs(targetAirspeedAngle - airspeedAngle) > .5f) {
        if (targetAirspeedAngle > airspeedAngle) {
            airspeedAngle += .5f;
        }
        else {
            airspeedAngle -= .5f;
        }
    }
    else {
        airspeedAngle = targetAirspeedAngle;
    }

    // Calculate mach angle
    speed = simVars->asiMachSpeed;

    if (speed > 0.3) {
        machAngle = 256 - ((((251.3 * log(speed) + 446.1) + 4.02) * 0.71111111111111) - airspeedAngle);
    }
    else {
        machAngle = 248.14444;
    }

    if (abs(machAngle - prevMachAngle) < 1)
    {
        machAngle = prevMachAngle;
    }
    else {
        prevMachAngle = machAngle;
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
        // Change calibration by knob movement amount (adjust for desired sensitivity)
        int adjust = (int)((prevVal - val) / 2);
        if (adjust != 0) {
            double newVal = globals.simVars->simVars.asiAirspeedCal + adjust;

            globals.simVars->write(KEY_TRUE_AIRSPEED_CAL_SET, newVal);
            prevVal = val;
        }
    }
}

#endif // !_WIN32
