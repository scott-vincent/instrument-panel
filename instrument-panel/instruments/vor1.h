#ifndef _VOR1_H_
#define _VOR1_H_

#include "instrument.h"

class vor1 : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double compassAngle;
    double locAngle;
    double slopeAngle;
    int toFromOn;
    int glideSlopeOn;

    // Hardware knobs
    int obsKnob = -1;

public:
    vor1(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _VOR1_H
