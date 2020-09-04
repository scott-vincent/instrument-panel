#ifndef _NAV_H_
#define _NAV_H_

#include "instrument.h"

class nav : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle;

    // Hardware knobs
    int calKnob = -1;

public:
    nav(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _NAV_H
