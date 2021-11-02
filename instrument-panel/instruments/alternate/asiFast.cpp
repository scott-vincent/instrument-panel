#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "asiFast.h"
#include "knobs.h"

asiFast::asiFast(int xPos, int yPos, int size, const char* parentName) : instrument(xPos, yPos, size)
{
    if (parentName) {
        // Use position, size, vars and knobs from parent
        hasParent = true;
        setName(parentName);
    }
    else {
        hasParent = false;
        setName("ASI Fast");
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
void asiFast::resize()
{
    destroyBitmaps();

    int fsize = size * FastPlaneSizeFactor;

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = fsize / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("asi-fast.png");
    addBitmap(orig);

    if (bitmaps[0] == nullptr) {
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
void asiFast::render()
{
    if (bitmaps[0] == nullptr) {
        return;
    }

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

    if (!globals.electrics) {
        dimInstrument();
    }
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void asiFast::update()
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
    if (!hasParent && globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

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
void asiFast::addVars()
{
    globals.simVars->addVar(name, "Airspeed Indicated", false, 1, 0);
    globals.simVars->addVar(name, "Airspeed True Calibrate", false, 1, 0);
    globals.simVars->addVar(name, "Airspeed Mach", false, 100, 0);
}

#ifndef _WIN32

void asiFast::addKnobs()
{
    // BCM GPIO 27 and 22
    calKnob = globals.hardwareKnobs->add(27, 22, -1, -1, 0);
}

void asiFast::updateKnobs()
{
    // Read knob for airspeed calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Change calibration by knob movement amount (adjust for desired sensitivity)
        int adjust = (int)((prevVal - val) / 2);
        if (adjust != 0) {
            double newVal = simVars->asiAirspeedCal + adjust;

            globals.simVars->write(KEY_TRUE_AIRSPEED_CAL_SET, newVal);
            prevVal = val;
        }
    }
}

#endif // !_WIN32
