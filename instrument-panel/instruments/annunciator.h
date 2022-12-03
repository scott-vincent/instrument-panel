#ifndef _ANNUNCIATOR_H_
#define _ANNUNCIATOR_H_

#ifndef _WIN32
#include <time.h>
#endif

#include "simvarDefs.h"
#include "instrument.h"

class annunciator : public instrument
{
private:
    SimVars* simVars;
    Aircraft loadedAircraft = UNDEFINED;
    bool fastAircraft = false;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    int selection = 1;
    int prevState = -1;
    bool vacWarningL = false;
    bool vacWarningR = false;
    bool fuelWarningL = false;
    bool fuelWarningR = false;
    bool oilWarning = false;
    bool voltsWarning = false;
    double fuelPercent;
    double totalPercent;
    double leftPercent;
    double rightPercent;
    double prevFuelL = 100;
    double prevFuelR = 100;
    int flashCount = 0;
#ifdef _WIN32
    long lastFlash;
#else
    timespec lastFlash;
#endif
    int dimDelay;

    // Hardware knobs
    int selSwitch = -1;

public:
    annunciator(int xPos, int yPos, int size);
    void render();
    void showAtcInfo();
    void update();

private:
    void resize();
    void checkFuel(double fuelLevel, bool* fuelWarning, double* prevFuel);
    void addKnobs();
    void updateKnobs();
};

#endif // _ANNUNCIATOR_H
