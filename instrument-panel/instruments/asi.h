#ifndef _ASI_H_
#define _ASI_H_

#include "simvarDefs.h"
#include "instrument.h"

class asi : public instrument
{
private:
    const double FastPlaneSizeFactor = 1.075;

    SimVars* simVars;
    Aircraft loadedAircraft = NO_AIRCRAFT;
    bool fastAircraft = false;
    float scaleFactor;

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
    asi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void renderFast();
    void resize();
    void resizeFast();
    void updateFast();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ASI_H
