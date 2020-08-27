#ifndef _ADI_LEARJET_H_
#define _ADI_LEARJET_H_

#include "instrument.h"

class adiLearjet : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    float bankAngle = 0;
    float pitchAngle = 0;
    signed short currentAdiCal = 0;
    int gyroSpinTime = 0;
    int failCount = 201;
    time_t lastPowerTime;

    // Hardware knobs
    int calKnob = -1;

public:
    adiLearjet(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _ADI_LEARJET_H
