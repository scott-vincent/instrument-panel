#ifndef _COM_SAVAGE_CUB_H_
#define _COM_SAVAGE_CUB_H_

#include "simvars.h"
#include "instrument.h"

class comSavageCub : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    int adjustSetSel = 0;
    int com1Freq;
    int com1Standby;

    // Hardware knobs
    int selPush = -1;
    int adjustKnob = -1;
    int adjustPush = -1;
    int prevSelVal = 0;
    int prevSelPush = 0;
    int prevAdjustVal = 0;
    int prevAdjustPush = 0;
    time_t lastAdjust = 0;
    time_t now;

public:
    comSavageCub(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addFreq3dp(int freq, int x, int y);
    void addVars();
    void addKnobs();
    void updateKnobs();
    double adjustCom(double val, int adjust);
};

#endif // _COM_SAVAGE_CUB_H
