#ifndef _RADIO_H_
#define _RADIO_H_

#include "instrument.h"

class radio : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double angle;

    // Hardware knobs
    int calKnob = -1;

public:
    radio(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _RADIO_H
