#ifndef _ASI_FAST_H_
#define _ASI_FAST_H_

#include "simvars.h"
#include "instrument.h"

class asiFast : public instrument
{
private:
    const double FastPlaneSizeFactor = 1.075;

    SimVars* simVars;
    float scaleFactor;
    bool hasParent;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double airspeedCal;
    double airspeedKnots;
    double airspeedAngle = 0;
    double angle;

    double targetAirspeedAngle;
    double machAngle;
    double prevMachAngle = 248.14444;

    // Hardware knobs
    int calKnob = -1;
    int prevVal = 0;

public:
    asiFast(int xPos, int yPos, int size, const char* parentName = nullptr);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ASI_FAST_H
