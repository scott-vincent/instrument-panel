#ifndef _ASI_H_
#define _ASI_H_

#include "instrument.h"

class asi : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int loadedAircraft;
    double airspeedCal;
    double airspeedKnots;
    double airspeedAngle;
    double angle;

    // Hardware knobs
    int calKnob = -1;

public:
    asi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ASI_H
