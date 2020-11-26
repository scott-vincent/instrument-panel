#include <stdio.h>
#include <stdlib.h>
#include "radCom.h"
#include "knobs.h"

radCom::radCom(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Rad Com");
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
void radCom::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size (original size is 800)
    scaleFactor = size / 800.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("rad-com.png");
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
    al_draw_scaled_bitmap(orig, 0, 0, 800, 800, 0, 0, size, size, 0);
    addBitmap(bmp);

    // 3 = Digits 1
    bmp = al_create_bitmap(570, 100);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 800, 570, 100, 0, 0, 570, 100, 0);
    addBitmap(bmp);

    // 4 = Digits 2
    bmp = al_create_bitmap(440, 86);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 900, 440, 86, 0, 0, 440, 86, 0);
    addBitmap(bmp);

    // 5 = Digits 2 selected
    bmp = al_create_bitmap(440, 86);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 986, 440, 86, 0, 0, 440, 86, 0);
    addBitmap(bmp);

    // 6 = Comm2
    bmp = al_create_bitmap(196 * scaleFactor, 44 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 604, 800, 196, 44, 0, 0, 196 * scaleFactor, 44 * scaleFactor, 0);
    addBitmap(bmp);

    // 7 = Swap button
    bmp = al_create_bitmap(68 * scaleFactor, 101 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 627, 844, 68, 101, 0, 0, 68 * scaleFactor, 101 * scaleFactor, 0);
    addBitmap(bmp);

    // 8 = Adjust button
    bmp = al_create_bitmap(105 * scaleFactor, 107 * scaleFactor);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 695, 844, 105, 107, 0, 0, 105 * scaleFactor, 107 * scaleFactor, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void radCom::render()
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

    // Add Comm2
    if (!simVars->com1Transmit) {
        al_draw_bitmap(bitmaps[6], 313 * scaleFactor, 78 * scaleFactor, 0);
    }

    // Add swap button
    if (switchSel == Swap) {
        al_draw_bitmap(bitmaps[7], 367 * scaleFactor, 378 * scaleFactor, 0);
    }

    // Add adjust button
    if (switchSel == Adjust) {
        al_draw_bitmap(bitmaps[8], 520 * scaleFactor, 377 * scaleFactor, 0);
    }

    // Add frequencies
    addFreq3dp(comFreq, 215, 220, true);
    addFreq3dp(comStandby, 257, 515, false);

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.active) {
        dimInstrument();
    }
}

/// <summary>
/// Displays the specified frequency to 3 d.p.
/// </summary>
void radCom::addFreq3dp(int freq, int x, int y, bool isLarge)
{
    int digit1 = freq / 100000;
    int digit2 = (freq % 100000) / 10000;
    int digit3 = (freq % 10000) / 1000;
    int digit4 = (freq % 1000) / 100;
    int digit5 = (freq % 100) / 10;
    int digit6 = freq % 10;

    int yPos = y * scaleFactor;
    int wide;
    int tall;
    int preDot = x;
    int postDot;
    int bmp;
    int fromBmp;

    if (isLarge) {
        wide = 57;
        tall = 100;
        postDot = x + wide * 3 + 30;
        bmp = 3;
    }
    else {
        wide = 44;
        tall = 86;
        postDot = x + wide * 3 + 23;
        bmp = 4;
    }

    int width = wide * scaleFactor;
    int height = tall * scaleFactor;

    // Add first 3 digits
    if (!isLarge && switchSel == Adjusting && adjustSetSel == 0) {
        fromBmp = bmp + 1;
    }
    else {
        fromBmp = bmp;
    }

    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit1, 0, wide, tall, preDot * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit2, 0, wide, tall, (preDot + wide) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit3, 0, wide, tall, (preDot + wide * 2) * scaleFactor, yPos, width, height, 0);

    // Add digit after decimal point
    if (!isLarge && switchSel == Adjusting && adjustSetSel == 1) {
        fromBmp = bmp + 1;
    }
    else {
        fromBmp = bmp;
    }

    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit4, 0, wide, tall, postDot * scaleFactor, yPos, width, height, 0);

    // Add last 2 digits
    if (!isLarge && switchSel == Adjusting && adjustSetSel == 2) {
        fromBmp = bmp + 1;
    }
    else {
        fromBmp = bmp;
    }

    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit5, 0, wide, tall, (postDot + wide) * scaleFactor, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[fromBmp], wide * digit6, 0, wide, tall, (postDot + wide * 2) * scaleFactor, yPos, width, height, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void radCom::update()
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

    // Calculate values - 3 d.p. for comms
    if (simVars->com1Transmit) {
        comFreq = (simVars->com1Freq + 0.0000001) * 1000.0;
        comStandby = (simVars->com1Standby + 0.0000001) * 1000.0;
    }
    else {
        comFreq = (simVars->com2Freq + 0.0000001) * 1000.0;
        comStandby = (simVars->com2Standby + 0.0000001) * 1000.0;
    }
}

#ifndef _WIN32

void radCom::addKnobs()
{
    // BCM GPIO 24 and 25
    selKnob = globals.hardwareKnobs->add(24, 25, -1, -1, 0);

    // BCM GPIO 19
    selPush = globals.hardwareKnobs->add(19, 0, -1, -1, 0);
}

void radCom::updateKnobs()
{
    // Read knob rotate
    int val = globals.hardwareKnobs->read(selKnob);
    if (val != INT_MIN) {
        // Convert knob value to selection (adjust for desired sensitivity)
        int diff = (prevVal - val) / 2;
        if (diff != 0) {
            switch (switchSel) {
                case Swap:
                    switchSel = Adjust;
                    break;

                case Adjust:
                    switchSel = Swap;
                    break;

                case Adjusting:
                    if (diff > 0) {
                        adjustDigits(1);
                    }
                    else {
                        adjustDigits(-1);
                    }
                    time(&lastAdjust);
                    break;
            }
            prevVal = val;
        }
    }

    // Read knob push
    val = globals.hardwareKnobs->read(selPush);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevPush % 2 == 1) {
            switch (switchSel) {
                case Swap:
                    if (simVars->com1Transmit) {
                        globals.simVars->write(KEY_COM1_RADIO_SWAP);
                    }
                    else {
                        globals.simVars->write(KEY_COM2_RADIO_SWAP);
                    }
                    break;

                case Adjust:
                    switchSel = Adjusting;
                    adjustSetSel = 0;
                    time(&lastAdjust);
                    break;

                case Adjusting:
                    if (adjustSetSel == 2) {
                        switchSel = Swap;
                    }
                    else {
                        adjustSetSel++;
                        time(&lastAdjust);
                    }
                    break;
            }
        }
        prevPush = val;
    }

    if (switchSel == Adjusting) {
        // Stop adjusting if more than 5 seconds since last adjustment
        time(&now);
        if (now - lastAdjust > 5) {
            switchSel = Adjust;
        }
    }
}

void radCom::adjustDigits(int adjust)
{
    if (simVars->com1Transmit) {
        double newVal = adjustCom(simVars->com1Standby, adjust);
        globals.simVars->write(KEY_COM1_STBY_RADIO_SET, newVal);
    }
    else {
        double newVal = adjustCom(simVars->com2Standby, adjust);
        globals.simVars->write(KEY_COM2_STBY_RADIO_SET, newVal);
    }
}

double radCom::adjustCom(double val, int adjust)
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
