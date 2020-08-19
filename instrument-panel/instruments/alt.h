#ifndef _ALT_H_
#define _ALT_H_

#include "instrument.h"

class alt : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    unsigned short pressure;
    unsigned short pressure2;
    short units;
    long altitude1;
    double altitude2;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float mb;            // millibars
    float inhg;          // inches of mercury
    float angle;
    long altitude = 0;
    long altitudeTarget = 0;

    // Hardware knobs
    int calKnob = -1;

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
