#ifndef _XPDR_SAVAGE_CUB_H_
#define _XPDR_SAVAGE_CUB_H_

#include "simvarDefs.h"
#include "instrument.h"

class xpdrSavageCub : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    int adjustSetSel = 0;

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
    xpdrSavageCub(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addSquawk(int freq, int x, int y);
    double adjustSquawk(double val, int adjust);
    void addVars();

};

#endif // _XPDR_SAVAGE_CUB_H
