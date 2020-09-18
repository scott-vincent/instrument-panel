#ifndef _NAV_H_
#define _NAV_H_

#include "instrument.h"
#include "simvars.h"

class nav : public instrument
{
private:
    enum AutopilotHdg {
        NoHdg,
        HdgSet,
        LevelFlight
    };

    enum AutopilotAlt {
        NoAlt,
        AltHold,
        PitchHold,
        VerticalSpeedHold,
    };

    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    SimVars* simVars;
    int switchSel = 0;
    int adjustSetSel = 0;
    int com1Freq;
    int com1Standby;
    int nav1Freq;
    int nav1Standby;
    int com2Freq;
    int com2Standby;
    int nav2Freq;
    int nav2Standby;
    int transponderState = 3;
    AutopilotHdg autopilotHdg;
    AutopilotAlt autopilotAlt;

    // Hardware knobs
    int selKnob = -1;
    int selPush = -1;
    int adjustKnob = -1;
    int adjustPush = -1;
    int prevSelVal = 0;
    int prevSelPush = 0;
    int prevAdjustVal = 0;
    int prevAdjustPush = 0;
    time_t lastAdjust = 0;
    time_t now;

public:
    nav(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void renderNav();
    void renderAutopilot();
    void addNum3(int val, int x, int y);
    void addNum4(int val, int x, int y, bool leading = true);
    void addNum5(int val, int x, int y, bool leading = true);
    void addFreq2dp(int freq, int x, int y);
    void addFreq3dp(int freq, int x, int y);
    void addSquawk(int code, int x, int y);
    void addVerticalSpeed(int x, int y);
    void addVars();
    void addKnobs();
    void updateKnobs();
    void navSwitchPressed();
    void autopilotSwitchPressed();
    void navAdjustDigits(int adjust);
    void autopilotAdjustDigits(int adjust);
    double adjustCom(double val, int adjust);
    double adjustNav(double val, int adjust);
    int adjustAdf(int val, int adjust);
    int adjustSquawk(int val, int adjust);
    int adjustSpeed(int val, int adjust);
    int adjustHeading(int val, int adjust);
    int adjustAltitude(int val, int adjust);
    int adjustVerticalSpeed(int val, int adjust);
    int adjustDigit(int val, int adjust, bool isSquawk = false);
};

#endif // _NAV_H
