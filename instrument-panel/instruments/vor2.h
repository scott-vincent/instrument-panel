#ifndef _VOR2_H_
#define _VOR2_H_

#include "instrument.h"

class vor2 : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle;

    // Hardware knobs
    int calKnob = -1;

public:
    vor2(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _VOR2_H
