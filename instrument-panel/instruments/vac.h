#ifndef _VAC_H_
#define _VAC_H_

#include "instrument.h"

class vac : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long instrumentVar;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

public:
    vac(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
};

#endif // _VAC_H
