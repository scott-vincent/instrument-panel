#ifndef _ANNUNCIATOR_H_
#define _ANNUNCIATOR_H_

#include "instrument.h"

class annunciator : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long instrumentVar;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;

public:
    annunciator(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();

};

#endif // _ANNUNCIATOR_H
