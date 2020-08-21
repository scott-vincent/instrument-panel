#ifndef _OIL_H_
#define _OIL_H_

#include "instrument.h"

class oil : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long instrumentVar;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

public:
    oil(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
};

#endif // _OIL_H
