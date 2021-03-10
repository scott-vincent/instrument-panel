#ifndef _RPM_SPITFIRE_H_
#define _RPM_SPITFIRE_H_

#include "simvars.h"
#include "instrument.h"

class rpmSpitfire : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double rpm;
    double angle = -180;
    double targetAngle;

public:
    rpmSpitfire(int xPos, int yPos, int size, const char* parentName = NULL);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _RPM_SPITFIRE_H