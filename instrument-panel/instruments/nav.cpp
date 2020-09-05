#include <stdio.h>
#include <stdlib.h>
#include "nav.h"
#include "simvars.h"
#include "knobs.h"

nav::nav(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Nav");
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
void nav::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 1600.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("nav.bmp");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size / 4);
    addBitmap(bmp);

    // 2 = Main
    bmp = al_create_bitmap(size, size / 4);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 1600, 400, 0, 0, size, size / 4, 0);
    addBitmap(bmp);

    // 3 = Digits
    bmp = al_create_bitmap(380, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 400, 380, 80, 0, 0, 0);
    addBitmap(bmp);

    // 4 = Dot
    bmp = al_create_bitmap(20, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 380, 400, 20, 80, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Switch
    bmp = al_create_bitmap(80 * scaleFactor, 34 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 400, 400, 80, 34, 0, 0, 80 * scaleFactor, 34 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void nav::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add selected switch
    int switchX;
    int switchY;
    switch (switchSel) {
    case 0:
        switchX = 460; switchY = 104; break;
    case 1:
        switchX = 1064; switchY = 104; break;
    case 2:
        switchX = 460; switchY = 233; break;
    case 3:
        switchX = 1064; switchY = 233; break;
    default:
        switchX = 649; switchY = 363; break;
    }
    al_draw_bitmap(bitmaps[5], switchX * scaleFactor, switchY * scaleFactor, 0);

    // Add panel 1 frequencies
    addFreq3dp(com1Freq, 237, 19);
    addFreq3dp(com1Standby, 523, 19);
    addFreq2dp(nav1Freq, 837, 19);
    addFreq2dp(nav1Standby, 1153, 19);

    // Add panel 2 frequencies
    addFreq3dp(com2Freq, 237, 148);
    addFreq3dp(com2Standby, 523, 148);
    addFreq2dp(nav2Freq, 837, 148);
    addFreq2dp(nav2Standby, 1153, 148);

    // Add panel 3 frequencies
    addFreq(adfFreq, 370, 278);
    addFreq(adfStandby, 790, 278);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.active) {
        //dimInstrument();
    }
}

/// <summary>
/// Displays the specified frequency to 0 d.p.
/// </summary>
void nav::addFreq(int freq, int x, int y)
{
    int digit1 = (freq % 10000) / 1000;
    int digit2 = (freq % 1000) / 100;
    int digit3 = (freq % 100) / 10;
    int digit4 = freq % 10;

    int yPos = y * scaleFactor;
    int width = 38 * scaleFactor;
    int height = 80 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[3], 38 * digit1, 0, 38, 80, x * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit4, 0, 38, 80, (x + 114) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Displays the specified frequency to 2 d.p.
/// </summary>
void nav::addFreq2dp(int freq, int x, int y)
{
    int digit1 = freq / 10000;
    int digit2 = (freq % 10000) / 1000;
    int digit3 = (freq % 1000) / 100;
    int digit4 = (freq % 100) / 10;
    int digit5 = freq % 10;

    int yPos = y * scaleFactor;
    int width = 38 * scaleFactor;
    int height = 80 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[3], 38 * digit1, 0, 38, 80, x * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 0, 0, 20, 80, (x + 114) * scaleFactor, yPos, 20 * scaleFactor, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit4, 0, 38, 80, (x + 134) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit5, 0, 38, 80, (x + 172) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Displays the specified frequency to 3 d.p.
/// </summary>
void nav::addFreq3dp(int freq, int x, int y)
{
    int digit1 = freq / 100000;
    int digit2 = (freq % 100000) / 10000;
    int digit3 = (freq % 10000) / 1000;
    int digit4 = (freq % 1000) / 100;
    int digit5 = (freq % 100) / 10;
    int digit6 = freq % 10;

    int yPos = y * scaleFactor;
    int width = 38 * scaleFactor;
    int height = 80 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[3], 38 * digit1, 0, 38, 80, x * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 0, 0, 20, 80, (x + 114) * scaleFactor, yPos, 20 * scaleFactor, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit4, 0, 38, 80, (x + 134) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit5, 0, 38, 80, (x + 172) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 38 * digit6, 0, 38, 80, (x + 210) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void nav::update()
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

    // Calculate values - 3 d.p. for comms, 2 d.p. for nav, 0 d.p. for adf
    com1Freq = (simVars->com1Freq + 0.0000001) * 1000.0;
    com1Standby = (simVars->com1Standby + 0.0000001) * 1000.0;
    nav1Freq = (simVars->nav1Freq + 0.0000001) * 100.0;
    nav1Standby = (simVars->nav1Standby + 0.0000001) * 100.0;
    com2Freq = (simVars->com2Freq + 0.0000001) * 1000.0;
    com2Standby = (simVars->com2Standby + 0.0000001) * 1000.0;
    nav2Freq = (simVars->nav2Freq + 0.0000001) * 100.0;
    nav2Standby = (simVars->nav2Standby + 0.0000001) * 100.0;
    adfFreq = simVars->adfFreq;
    adfStandby = simVars->adfStandby;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void nav::addVars()
{
    globals.simVars->addVar(name, "Com Active Frequency:1", false, 0.01, 100);
    globals.simVars->addVar(name, "Com Standby Frequency:1", false, 0.01, 100);
    globals.simVars->addVar(name, "Nav Active Frequency:1", false, 0.01, 100);
    globals.simVars->addVar(name, "Nav Standby Frequency:1", false, 0.01, 100);
    globals.simVars->addVar(name, "Com Active Frequency:2", false, 0.01, 100);
    globals.simVars->addVar(name, "Com Standby Frequency:2", false, 0.01, 100);
    globals.simVars->addVar(name, "Nav Active Frequency:2", false, 0.01, 100);
    globals.simVars->addVar(name, "Nav Standby Frequency:2", false, 0.01, 100);
    globals.simVars->addVar(name, "Adf Active Frequency:1", false, 0.01, 100);
    globals.simVars->addVar(name, "Adf Standby Frequency:1", false, 0.01, 100);
}

#ifndef _WIN32

void nav::addKnobs()
{
    // BCM GPIO 2 and 3
    selKnob = globals.hardwareKnobs->add(2, 3, 0, 49, 0);

    // BCM GPIO 4 and 5
    freqKnob = globals.hardwareKnobs->add(2, 3, 100, 200, 0);
}

void nav::updateKnobs()
{
    // Read knob for switch selection
    int val = globals.hardwareKnobs->read(selKnob);

    if (val != INT_MIN) {
        // Convert knob value to new instrument value (adjust for desired sensitivity)
        switchSel = val / 10;

        // Update new instrument variable
        //globals.simVars->write("simvar", simVarVal);
    }

    // Read knob for frequency set
    val = globals.hardwareKnobs->read(freqKnob);

    if (val != INT_MIN) {
        // Convert knob value to new instrument value (adjust for desired sensitivity)
        double freqVal = val;

        // Update new instrument variable
        //globals.simVars->write("simvar", simVarVal);
    }
}

#endif // !_WIN32
