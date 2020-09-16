#ifndef _NAV_H_
#define _NAV_H_

#include "instrument.h"
#include "simvars.h"

class nav : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int switchSel = 0;
    int digitSetSel = 0;
    int com1Freq;
    int com1Standby;
    int nav1Freq;
    int nav1Standby;
    int com2Freq;
    int com2Standby;
    int nav2Freq;
    int nav2Standby;
    int adfFreq;
    int adfStandby;
    int transponderState = 3;
    int transponderCode;

    // Hardware knobs
    int selKnob = -1;
    int selPush = -1;
    int digitsKnob = -1;
    int digitsPush = -1;
    int prevSwitchPush = 0;
    int prevDigitSetVal = 0;
    int prevDigitPush = 0;
    time_t lastDigitAdjust = 0;

public:
    nav(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addFreq(int freq, int x, int y);
    void addFreq2dp(int freq, int x, int y);
    void addFreq3dp(int freq, int x, int y);
    void addSquawk(int code, int x, int y);
    void addVars();
    void addKnobs();
    void updateKnobs();
    void adjustCom(double val, EVENT_ID eventId, int adjust);
    void adjustNav(double val, EVENT_ID eventId, int adjust);
    void adjustAdf(double val, EVENT_ID eventId, int adjust);
    void adjustSquawk(double val, EVENT_ID eventId, int adjust);
    int adjustDigit(int val, int adjust);
};

#endif // _NAV_H
