#ifndef _NAV_H_
#define _NAV_H_

#include "instrument.h"

class nav : public instrument
{
private:
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    int com1Freq;
    int com1Standby;
    int nav1Freq;
    int nav1Standby;
    int com2Freq;
    int com2Standby;
    int nav2Freq;
    int nav2Standby;
    int adfFreq;
    int adfStandby;

    // Hardware knobs
    int selKnob = -1;
    int freqKnob = -1;

public:
    nav(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addFreq(int freq, int x, int y);
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _NAV_H
