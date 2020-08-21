#include <stdio.h>
#include <stdlib.h>
#include "adi.h"
#include "simvars.h"
#include "knobs.h"

adi::adi(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("ADI");
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
void adi::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("adi.bmp");
    addBitmap(orig);

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Back horizon (can rotate)
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 0, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 3 = Horizon
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Horizon shadow
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 2400, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Rim (can rotate)
    bmp = al_create_bitmap(800, 800);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 800, 800, 800, 0, 0, 0);
    addBitmap(bmp);

    // 6 = Rim shadow
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 1600, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 7 = Background
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 8 = Outer casing
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 1600, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 9 = Outer pointer shadow
    bmp = al_create_bitmap(200 * scaleFactor, 200 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 1100, 2400, 200, 200, 0, 0, 200 * scaleFactor, 200 * scaleFactor, 0);
    addBitmap(bmp);

    // 10 = Middle pointer
    bmp = al_create_bitmap(size, 400 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 3200, 800, 400, 0, 0, size, 400 * scaleFactor, 0);
    addBitmap(bmp);

    // 11 = Middle pointer shadow
    bmp = al_create_bitmap(size, 400 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 800, 3200, 800, 400, 0, 0, size, 400 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void adi::render()
{
    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add back horizon and rotate
    al_draw_scaled_rotated_bitmap(bitmaps[2], 400, 400, 400 * scaleFactor, (400 + pitchAngle * 10) * scaleFactor, scaleFactor, scaleFactor, bankAngle * AngleFactor, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add horizon shadow
        al_draw_scaled_rotated_bitmap(bitmaps[4], 400, 400, 415 * scaleFactor, (415 + pitchAngle * 10) * scaleFactor, scaleFactor, scaleFactor, bankAngle * AngleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add horizon
    al_draw_scaled_rotated_bitmap(bitmaps[3], 400, 400, 400 * scaleFactor, (400 + pitchAngle * 10) * scaleFactor, scaleFactor, scaleFactor, bankAngle * AngleFactor, 0);

    if (globals.enableShadows) {
        // Set blender to multiply (shades of grey darken, white has no effect)
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);

        // Add rim shadow
        al_draw_bitmap(bitmaps[6], 15 * scaleFactor, 15 * scaleFactor, 0);

        // Add outer pointer shadow
        al_draw_bitmap(bitmaps[9], 315 * scaleFactor, 15 * scaleFactor, 0);

        // Add middle pointer shadow
        al_draw_bitmap(bitmaps[11], 15 * scaleFactor, (355 - currentAdiCal * 10) * scaleFactor, 0);

        // Restore normal blender
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    }

    // Add middle pointer
    al_draw_bitmap(bitmaps[10], 0, (340 - currentAdiCal * 10) * scaleFactor, 0);

    // Add background
    al_draw_bitmap(bitmaps[7], 0, 0, 0);

    // Add rim
    al_draw_scaled_rotated_bitmap(bitmaps[5], 400, 400, 400 * scaleFactor, 400 * scaleFactor, scaleFactor, scaleFactor, bankAngle * AngleFactor, 0);

    // Add outer casing
    al_draw_bitmap(bitmaps[8], 0, 0, 0);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void adi::update()
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
    globals.connected = fetchVars();

    // Calculate values
    float pitchTest = (float)((float)pitch * 3.6 / (42949672.96));
    if (pitchTest - pitchAngle > 6)
    {
        pitchAngle += 5;
    }
    else if (pitchAngle - pitchTest > 6)
    {
        pitchAngle -= 5;
    }
    else
    {
        pitchAngle = pitchTest;
    }

    if (pitchAngle > 11.5f) {
        pitchAngle = 11.5;
    }
    else if (pitchAngle < -11.5f) {
        pitchAngle = -11.5f;
    }

    float bankTest = (float)((float)bank * 3.6 / (42949672.96));
    if (bankTest - bankAngle > 6 && bankTest - bankAngle < 180)
    {
        bankAngle += 5;
    }
    else if (bankAngle - bankTest > 6 && bankAngle - bankTest < 180)
    {
        bankAngle -= 5;
    }
    else
    {
        bankAngle = bankTest;
    }

    if (!globals.externalControls)
    {
        adiCal = 0;
    }

    if (currentAdiCal > adiCal && currentAdiCal > -10)
    {
        currentAdiCal -= 1;
    }
    else if (currentAdiCal < adiCal && currentAdiCal < 10)
    {
        currentAdiCal += 1;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void adi::addVars()
{
    globals.simVars->addVar(name, "Pitch", 0x0578 + 0x8000, false, 65536L * 64L, 0);
    globals.simVars->addVar(name, "Bank", 0x057C + 0x8000, false, 65536L * 64L, 0);
    globals.simVars->addVar(name, "ADI Cal", 0x73E4 + 0x8000, false, 1, 0);
}

/// <summary>
/// Use SDK to obtain latest values of all flightsim variables
/// that affect this instrument.
/// 
/// Returns false if flightsim is not connected.
/// </summary>
bool adi::fetchVars()
{
    bool success = true;
    DWORD result;

    // Pitch
    if (!globals.simVars->FSUIPC_Read(0x0578 + 0x8000, 4, &pitch, &result)) {
        pitch = 0;
        success = false;
    }

    // Bank
    if (!globals.simVars->FSUIPC_Read(0x057C + 0x8000, 4, &bank, &result)) {
        bank = 0;
        success = false;
    }

    // ADI Cal (for manual calibration)
    if (!globals.simVars->FSUIPC_Read(0x73E4 + 0x8000, 2, &adiCal, &result)) {
        adiCal = 0;
        success = false;
    }

    if (!globals.simVars->FSUIPC_Process(&result))
    {
        success = false;
    }

    return success;
}

#ifndef _WIN32

void adi::addKnobs()
{
    // BCM GPIO 2 and 3
    calKnob = globals.hardwareKnobs->add(2, 3, -20, 20, 0);
}

bool adi::updateKnobs()
{
    DWORD result;

    // Read knob for ADI calibration
    int val = globals.hardwareKnobs->read(calKnob);

    if (val != INT_MIN) {
        // Convert knob value to variable (adjust for sensitivity)
        adiCal = val / 2;

        // Update ADI calibration variable
        if (!globals.simVars->FSUIPC_Write(0x73E4 + 0x8000, 2, &adiCal, &result) || !globals.simVars->FSUIPC_Process(&result)) {
            return false;
        }
    }

    return true;
}

#endif // !_WIN32