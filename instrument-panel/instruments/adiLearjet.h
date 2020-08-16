#ifndef _ADI_LEARJET_H_
#define _ADI_LEARJET_H_

#include "instrument.h"

class adiLearjet : public instrument
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
    int gyroSpinTime = 0;
    int failCount = 201;
    time_t lastPowerTime;

    // Hardware knobs
    int adiCalKnob = -1;

public:
    adiLearjet(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    bool fetchVars();
    void addKnobs();
    bool updateKnobs();
};

#endif // _ADI_LEARJET_H
