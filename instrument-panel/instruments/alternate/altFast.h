#ifndef _ALT_FAST_H_
#define _ALT_FAST_H_

#include "simvars.h"
#include "instrument.h"

class altFast : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;
    bool hasParent;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double inhg;
    double angle;
    double altitude = 0;

    // Hardware knobs
    int calKnob = -1;
    int prevVal = 0;
    time_t lastCalAdjust = 0;
    time_t now;

public:
    altFast(int xPos, int yPos, int size, const char* parentName = nullptr);
    void render();
    void update();
    void updateCustom(double inhgVal) override;

private:
    void resize();
    void addSmallNumber(int yPos, int digit1, int digit2, int digit3, int digit4);
    void addSmallShadow(int yPos);
    void addLargeNumber(int yPos1, int yPos2, int yPos3);
    void addLargeShadow();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ALT_FAST_H
