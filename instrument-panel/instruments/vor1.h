#ifndef _VOR1_H_
#define _VOR1_H_

#include "simvarDefs.h"
#include "instrument.h"

class vor1 : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double compassAngle;
    double locAngle;
    double slopeAngle;
    double crossTrkMeters;
    int toFromOn;
    int glideSlopeOn;
    int gpsControlsNavOn;

    // Hardware knobs
    int obsKnob = -1;
    int prevVal = 0;

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
