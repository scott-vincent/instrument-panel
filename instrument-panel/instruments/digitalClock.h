#ifndef _DIGITAL_CLOCK_H_
#define _DIGITAL_CLOCK_H_

#include "simvars.h"
#include "instrument.h"

class digitalClock : public instrument
{
    enum DisplayView {
        Voltage,
        Farenheit,
        Celsius
    };

    enum ClockView {
        UtcTime,
        LocalTime,
        FlightTime,
        ElapsedTime
    };

private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    DisplayView displayView = Celsius;
    ClockView clockView = UtcTime;
    bool stopWatchRunning = false;
    int stopWatchSeconds = 0;
    time_t flightStartTime;
    time_t now;
    time_t stopWatchStarted;
    time_t stopWatchPressed;
    int voltsx10;
    int tempFx10;
    int tempCx10;
    time_t lastTempChange = 0;
    int utcHours;
    int utcMins;
    int localHours;
    int localMins;
    int flightHours;
    int flightMins;
    int elapsedMins;
    int elapsedSecs;

    // Hardware knobs
    int topButton = -1;
    int leftButton = -1;
    int rightButton = -1;
    int prevTopVal = -1;
    int prevLeftVal = 0;
    int prevRightVal = 0;

public:
    digitalClock(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void drawDisplay(int digit1, int digit2, int digit3, int letter, bool isMinus = false);
    void drawClock(int digit1, int digit2, int digit3, int digit4);
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _DIGITAL_CLOCK_H
