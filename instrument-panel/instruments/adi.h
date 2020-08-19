#ifndef _ADI_H_
#define _ADI_H_

#include "instrument.h"

class adi : public instrument
{
private:
    float scaleFactor;

    // FlightSim vars (external variables that influence this instrument)
    long pitch;
    long bank;
    signed short adiCal;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float bankAngle = 0;
    float pitchAngle = 0;
    signed short currentAdiCal = 0;

    // Hardware knobs
    int calKnob = -1;

public:
    adi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
    bool updateKnobs();

};

#endif // _ADI_H
