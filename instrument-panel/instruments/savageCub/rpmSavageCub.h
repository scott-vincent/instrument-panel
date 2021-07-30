#ifndef _RPM_SAVAGE_CUB_H_
#define _RPM_SAVAGE_CUB_H_

#include "simvars.h"
#include "instrument.h"

class rpmSavageCub : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    int digit1;
    int digit2;
    int digit3;
    int digit4;
    int digit5;
    double angle;

public:
    rpmSavageCub(int xPos, int yPos, int size, const char* parentName = nullptr);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _RPM_SAVAGE_CUB_H