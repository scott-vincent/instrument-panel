#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "xpdrSavageCub.h"
#include "knobs.h"

xpdrSavageCub::xpdrSavageCub(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("XPDR Savage Cub");
    addVars();
    simVars = &globals.simVars->simVars;
    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void xpdrSavageCub::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 400.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("xpdr-savage-cub.png");
    addBitmap(orig);

    if (bitmaps[0] == nullptr) {
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
void xpdrSavageCub::render()
{
    if (bitmaps[0] == nullptr) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    // Add main
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add squawk
    addSquawk(simVars->transponderCode, 198, 120);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.avionics) {
        dimInstrument();
    }
}

/// <summary>
/// Displays the squawk code
/// </summary>
void xpdrSavageCub::addSquawk(int code, int x, int y)
{
    // Transponder code is in BCO16
    int digit1 = code / 4096;
    code -= digit1 * 4096;
    int digit2 = code / 256;
    code -= digit2 * 256;
    int digit3 = code / 16;
    int digit4 = code - digit3 * 16;

    int yPos = y * scaleFactor;
    int width = 32 * scaleFactor;
    int height = 50 * scaleFactor;

    al_draw_scaled_bitmap(bitmaps[3], 32 * digit1, 0, 32, 50, x * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit2, 0, 32, 50, (x + 32) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit3, 0, 32, 50, (x + 64) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[3], 32 * digit4, 0, 32, 50, (x + 96) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void xpdrSavageCub::update()
{
    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void xpdrSavageCub::addVars()
{
    globals.simVars->addVar(name, "Transponder Code:1", false, 1, 0);
}

#ifndef _WIN32

void xpdrSavageCub::addKnobs()
{
    // BCM GPIO 20 and 21
    adjustKnob = globals.hardwareKnobs->add(20, 21, -1, -1, 0);

    // BCM GPIO 16
    adjustPush = globals.hardwareKnobs->add(16, 0, -1, -1, 0);
}

void xpdrSavageCub::updateKnobs()
{
    // Read knob for digits set
    int val = globals.hardwareKnobs->read(adjustKnob);
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
            int newVal = adjustSquawk(simVars->transponderCode, adjust);
            globals.simVars->write(KEY_XPNDR_SET, newVal);
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
            if (adjustSetSel >= 4) {
                adjustSetSel = 0;
            }
        }
        prevAdjustPush = val;
    }
}

int xpdrSavageCub::adjustSquawk(int val, int adjust)
{
    // Transponder code is in BCD
    int digit1 = val / 4096;
    val -= digit1 * 4096;
    int digit2 = val / 256;
    val -= digit2 * 256;
    int digit3 = val / 16;
    int digit4 = val - digit3 * 16;

    switch (adjustSetSel) {
    case 0:
        digit1 = adjustDigit(digit1, adjust);
        break;
    case 1:
        digit2 = adjustDigit(digit2, adjust);
        break;
    case 2:
        digit3 = adjustDigit(digit3, adjust);
        break;
    case 3:
        digit4 = adjustDigit(digit4, adjust);
        break;
    }

    // Convert to BCD
    return digit1 * 4096 + digit2 * 256 + digit3 * 16 + digit4;
}

int xpdrSavageCub::adjustDigit(int val, int adjust)
{
    int maxDigit = 7;

    val += adjust;
    if (val > maxDigit) {
        val -= maxDigit + 1;
    }
    else if (val < 0) {
        val += maxDigit + 1;
    }

    return val;
}

#endif // !_WIN32
