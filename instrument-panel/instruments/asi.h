#ifndef _ASI_H_
#define _ASI_H_

#include "instrument.h"

class asi : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long airspeed;
    signed short airspeedCal;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    long airspeedKnots;
    float airspeedAngle;
    float angle;

    // Hardware knobs
    int calKnob = -1;

public:
    asi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
    bool updateKnobs();

};

#endif // _ASI_H
