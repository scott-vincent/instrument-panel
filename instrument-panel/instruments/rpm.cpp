#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rpm.h"
#include "alternate/rpmPercent.h"
#include "savageCub/rpmSavageCub.h"
#include "simvars.h"
#include "knobs.h"


rpm::rpm(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("RPM");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void rpm::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("rpm.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Main dial
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Pointer
    bmp = al_create_bitmap(100, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 100, 600, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Pointer shadow
    bmp = al_create_bitmap(100, 600);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 900, 0, 100, 600, 0, 0, 0);
    addBitmap(bmp);

    // 5 = White digits
    bmp = al_create_bitmap(54 * scaleFactor, 594 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1000, 0, 54, 594, 0, 0, 54 * scaleFactor, 594 * scaleFactor, 0);
    addBitmap(bmp);

    // 6 = Black digits
    bmp = al_create_bitmap(54 * scaleFactor, 594 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1054, 0, 54, 594, 0, 0, 54 * scaleFactor, 594 * scaleFactor, 0);
    addBitmap(bmp);

    // 7 = Hourglass
    bmp = al_create_bitmap(54 * scaleFactor, 156 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1108, 0, 54, 156, 0, 0, 54 * scaleFactor, 156 * scaleFactor, 0);
    addBitmap(bmp);

    // 8 = Reverse thrust indicator
    bmp = al_create_bitmap(75 * scaleFactor, 30 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1000, 604, 75, 30, 0, 0, 75 * scaleFactor, 30 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void rpm::render()
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

    // Add white digits
    al_draw_bitmap_region(bitmaps[5], 0, (499 - digit1 * 50.55) * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 238 * scaleFactor, 564 * scaleFactor, 0);
    al_draw_bitmap_region(bitmaps[5], 0, (499 - digit2 * 50.55) * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 292 * scaleFactor, 564 * scaleFactor, 0);
    al_draw_bitmap_region(bitmaps[5], 0, (499 - digit3 * 50.55) * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 346 * scaleFactor, 564 * scaleFactor, 0);
    al_draw_bitmap_region(bitmaps[5], 0, (499 - digit4 * 50.55) * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 400 * scaleFactor, 564 * scaleFactor, 0);

    // Add black digits
    al_draw_bitmap_region(bitmaps[6], 0, (499 - digit5 * 50.6) * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 454 * scaleFactor, 564 * scaleFactor, 0);

    // Add hourglass
    al_draw_bitmap_region(bitmaps[7], 0, 44 * scaleFactor, 54 * scaleFactor, 72 * scaleFactor, 508 * scaleFactor, 564 * scaleFactor, 0);

    // Add main dial
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add pointer shadow
        al_draw_scaled_rotated_bitmap(bitmaps[4], 50, 400, 415 * scaleFactor, 415 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add pointer
    al_draw_scaled_rotated_bitmap(bitmaps[3], 50, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, angle * DegreesToRadians, 0);

    // Add reverse thrust indicator
    if (simVars->throttlePosition < 0) {
        al_draw_bitmap(bitmaps[8], 362 * scaleFactor, 496 * scaleFactor, 0);
    }

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
void rpm::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;

        // Load custom instrument for this aircraft if we have one
        if (customInstrument) {
            delete customInstrument;
            customInstrument = NULL;
        }
        
        // Test for high-reving Rotax piston engine (e.g., Savage Cub, Shock Ultra, et al)
        if ((int)simVars->engineType == 0 && simVars->engineMaxRpm > 5000) {
            customInstrument = new rpmSavageCub(xPos, yPos, size, name);
        }
        // Test for Jet (1) and Turbine (5) engines
        else if ((int)simVars->engineType == 1 || (int)simVars->engineType == 5) {
            customInstrument = new rpmPercent(xPos, yPos, size, name);
        }
        // Otherwise, use this standard RPM gauge is for a piston engine with a 
        // typical red line at 2700 RPM. This gauge has a max of 3500 RPM.
        // This gauge is also used with the DA40 and and DA62 aircraft that
        // use a higher reving engine but display prop RPM in their panel.
    }

    if (customInstrument) {
        customInstrument->update();
        return;
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
    digit1 = ((int)simVars->rpmElapsedTime % 10000) / 1000;
    digit2 = ((int)simVars->rpmElapsedTime % 1000) / 100;
    digit3 = ((int)simVars->rpmElapsedTime % 100) / 10;
    digit4 = (int)simVars->rpmElapsedTime % 10;
    digit5 = (int)(simVars->rpmElapsedTime * 10) % 10;


    // The Diamond DA40 and DA62 aircraft have a higher reving engine (> 2700)
    // but display a much slower constant-speed prop RPM on their panel.  
    // On other engines with fixed, non-geared props, the prop RPM and
    // engine RPM are identical. So, we can just use propRpm for this gauge.
    double rpm = simVars->propRpm;

    // Reference angles/points on the standard RPM gauge
    // There is a difference in the gauge's scale from 0-1000 versus 1000-2700
    double origin0 = 123.0;     // angle from vertical to the 0 RPM line
    double origin1000 = 63.0;   // angle from vertical to the 1000 RPM line
    double rangeTo1000 = 60.0;      // range in degrees for arc from 0 to 1000 RPM
    double range1kTo27k = 136.0;    // range in degrees for arc from 1000 to 2700 RPM
    
    if (rpm < 1000) {
        // Indicator angle between 0 and 1000 RPM
        angle = (rangeTo1000 * rpm / 1000.0) - origin0;
    }
    else {
        // Indicator angle in the 1700 RPM span between 1000 and 2700
        angle = (range1kTo27k * (rpm - 1000) / 1700.0) - origin1000;
    }
    // TODO: Should also handle the scale difference between 2700 and 3500 RPM,
    // however excursions into to this range are exceptional for the typical engines
    // as the prop tips would go supersonic.
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void rpm::addVars()
{
    globals.simVars->addVar(name, "Engine Type", false, 1, 0);
    globals.simVars->addVar(name, "Max Rated Engine RPM", false, 1, 0);
    globals.simVars->addVar(name, "Prop RPM:1", false, 1, 0);
    globals.simVars->addVar(name, "General Eng Elapsed Time:1", false, 1, 0);
    // Values used by other RPM gauges
    globals.simVars->addVar(name, "General Eng Rpm:1", false, 1, 0);
    globals.simVars->addVar(name, "Turb Eng N1:1", false, 1, 0);

    globals.simVars->addVar(name, "Eng Rpm Animation Percent:1", false, 1, 0);
}
