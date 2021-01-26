#include <stdio.h>
#include <stdlib.h>
#include "comSavageCub.h"
#include "knobs.h"

comSavageCub::comSavageCub(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("COM Savage Cub");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void comSavageCub::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 400.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("com-savage-cub.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, size);
    addBitmap(bmp);

    // 2 = Main
    bmp = al_create_bitmap(size, size);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 400, 400, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Digits
    bmp = al_create_bitmap(320, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 400, 320, 50, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void comSavageCub::render()
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

    // Add com1 frequencies
    addFreq3dp(com1Freq, 105, 120);
    addFreq3dp(com1Standby, 105, 170);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.avionics) {
        dimInstrument();
    }
}

/// <summary>
/// Displays the specified frequency to 3 d.p.
/// </summary>
void comSavageCub::addFreq3dp(int freq, int x, int y)
{
    int digit1 = freq / 100000;
    int digit2 = (freq % 100000) / 10000;
    int digit3 = (freq % 10000) / 1000;
    int digit4 = (freq % 1000) / 100;
    int digit5 = (freq % 100) / 10;
    int digit6 = freq % 10;

    int yPos = y * scaleFactor;
    int width = 32 * scaleFactor;
    int height = 50 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[3], 32 * digit1, 0, 32, 50, x * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit2, 0, 32, 50, (x + 32) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit3, 0, 32, 50, (x + 64) * scaleFactor, yPos, width, height, 0);
    // Decimal point already drawn
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit4, 0, 32, 50, (x + 128) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit5, 0, 32, 50, (x + 160) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit6, 0, 32, 50, (x + 192) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void comSavageCub::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

    // Calculate values - 3 d.p. for comms
    com1Freq = (simVars->com1Freq + 0.0000001) * 1000.0;
    com1Standby = (simVars->com1Standby + 0.0000001) * 1000.0;
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void comSavageCub::addVars()
{
    globals.simVars->addVar(name, "Com Active Frequency:1", false, 0.005, 100);
    globals.simVars->addVar(name, "Com Standby Frequency:1", false, 0.005, 100);
}

#ifndef _WIN32

void comSavageCub::addKnobs()
{
    // BCM GPIO 12
    selPush = globals.hardwareKnobs->add(12, 0, -1, -1, 0);

    // BCM GPIO 20 and 21
    adjustKnob = globals.hardwareKnobs->add(20, 21, -1, -1, 0);

    // BCM GPIO 16
    adjustPush = globals.hardwareKnobs->add(16, 0, -1, -1, 0);
}

void comSavageCub::updateKnobs()
{
    // Read switch push
    int val = globals.hardwareKnobs->read(selPush);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevSelPush % 2 == 1) {
            // Com switch pressed
            globals.simVars->write(KEY_COM1_RADIO_SWAP);
        }
        prevSelPush = val;
        adjustSetSel = 0;
    }

    // Read knob for digits set
    val = globals.hardwareKnobs->read(adjustKnob);
    if (val != INT_MIN) {
        int diff = (val - prevAdjustVal) / 2;
        int adjust = 0;
        if (diff > 0) {
            adjust = 1;
        }
        else if (diff < 0) {
            adjust = -1;
        }

        if (adjust != 0) {
            double newVal = adjustCom(simVars->com1Standby, adjust);
            globals.simVars->write(KEY_COM1_STBY_RADIO_SET, newVal);
            prevAdjustVal = val;
        }
        time(&lastAdjust);
    }
    else if (lastAdjust != 0) {
        // Reset digit set selection if more than 5 seconds since last adjustment
        time(&now);
        if (now - lastAdjust > 5) {
            adjustSetSel = 0;
            lastAdjust = 0;
        }
    }

    // Read digits set push
    val = globals.hardwareKnobs->read(adjustPush);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevAdjustPush % 2 == 1) {
            adjustSetSel++;
            if (adjustSetSel >= 3) {
                adjustSetSel = 0;
            }
        }
        prevAdjustPush = val;
    }
}

double comSavageCub::adjustCom(double val, int adjust)
{
    int whole = val;
    val -= whole;
    int thousandths = (val + 0.0001) * 1000;
    int frac1 = thousandths / 100;
    int frac2 = thousandths % 100;

    if (adjustSetSel == 0) {
        // Adjust whole - Range 118 to 136
        whole += adjust;
        if (whole > 136) {
            whole -= 19;
        }
        else if (whole < 118) {
            whole += 19;
        }
    }
    else if (adjustSetSel == 1) {
        // Adjust 10ths
        frac1 += adjust;
        if (frac1 > 9) {
            frac1 -= 10;
        }
        else if (frac1 < 0) {
            frac1 += 10;
        }
    }
    else {
        // Adjust 100ths and 1000ths
        frac2 += adjust * 5;

        if (frac2 >= 100) {
            frac2 -= 100;
        }
        else if (frac2 < 0) {
            frac2 += 100;
        }

        // Skip .020, .045, .070 and .095
        if (frac2 == 95) {
            frac2 += adjust * 5;
            if (frac2 >= 100) {
                frac2 -= 100;
            }
        }
        else if (frac2 == 20 || frac2 == 45 || frac2 == 70) {
            frac2 += adjust * 5;
        }

        // .020 shows as .025 and .070 shows as .075
        if (frac2 == 25 || frac2 == 75) {
            frac2 -= 5;
        }
    }

    // Convert to BCD
    int digit1 = whole / 100;
    int digit2 = (whole % 100) / 10;
    int digit3 = whole % 10;
    int digit4 = frac1;
    int digit5 = frac2 / 10;
    int digit6 = frac2 % 10;

    // Return digit6 as fraction
    return 65536 * digit1 + 4096 * digit2 + 256 * digit3 + 16 * digit4 + digit5 + digit6 * 0.1;
}

#endif // !_WIN32
