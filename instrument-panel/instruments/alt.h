#ifndef _ALT_H_
#define _ALT_H_

#include "instrument.h"

class alt : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long instrumentVar;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float instrumentValue;

    // Hardware knobs
    int newKnob = -1;

public:
    alt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
    bool updateKnobs();

};

#endif // _ALT_H
