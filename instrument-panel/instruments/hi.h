#ifndef _HI_H_
#define _HI_H_

#include "instrument.h"

class hi : public instrument
{
private:
    float scaleFactor;

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
};

#endif // _HI_H
