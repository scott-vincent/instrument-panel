#ifndef _HI_H_
#define _HI_H_

#include "instrument.h"

class hi : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long heading;
    long headingBug;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float angle;
    float bugAngle;

    // Hardware knobs
    int hdgKnob = -1;

public:
    hi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();

};

#endif // _HI_H
