#ifndef _EGT_H_
#define _EGT_H_

#include "instrument.h"

class egt : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long instrumentVar;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

public:
    egt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
};

#endif // _EGT_H
