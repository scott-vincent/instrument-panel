#ifndef _TRIM_FLAPS2_H_
#define _TRIM_FLAPS2_H_

#include "simvars.h"
#include "instrument.h"

class trimFlaps2 : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    bool fastAircraft = false;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    float trimOffset;
    float rudderTrimOffset;
    float flapsOffset = 0;
    float targetFlaps;
    int gearUpWarning = 0;
    double requestedTugHeading = -1;

public:
    trimFlaps2(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void writeTugHeading(double heading);
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _TRIM_FLAPS2_H_
