#ifndef _ALT_H_
#define _ALT_H_

#include "instrument.h"

class alt : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double mb;            // millibars
    double inhg;          // inches of mercury
    double angle;
    double altitude = 0;
    double altitudeTarget = 0;

    // Hardware knobs
    int calKnob = -1;

public:
    alt(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ALT_H
