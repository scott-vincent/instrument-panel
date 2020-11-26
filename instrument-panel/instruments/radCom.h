#ifndef _RAD_COM_H_
#define _RAD_COM_H_

#include "simvars.h"
#include "instrument.h"

class radCom : public instrument
{
private:
    enum Switch {
        Swap,
        Adjust,
        Adjusting
    };

    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    Switch switchSel = Swap;
    int adjustSetSel = 0;
    int comFreq;
    int comStandby;

    // Hardware knobs
    int selKnob = -1;
    int selPush = -1;
    int prevVal = 0;
    int prevPush = 0;
    time_t lastAdjust = 0;
    time_t now;

public:
    radCom(int xPos, int yPos, int size);
    void render();
    void addFreq3dp(int freq, int x, int y, bool isLarge);
    void update();

private:
    void resize();
    void addKnobs();
    void updateKnobs();
    void adjustDigits(int adjust);
    double adjustCom(double val, int adjust);

};

#endif // _RAD_COM_H
