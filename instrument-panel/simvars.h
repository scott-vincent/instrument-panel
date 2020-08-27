#ifndef _SIMVARS_H_
#define _SIMVARS_H_

#include <thread>
#include "globals.h"
#include "simvarDefs.h"

extern globalVars globals;

class simvars {
public:
    SimVars simVars;

private:
    std::thread* dataLinkThread = NULL;
    int currentVar = 0;
    int varCount = 0;
    char varGroup[256][256] = {};
    char varName[256][256] = {};
    int varOffset[256] = {};
    bool varIsBool[256] = {};
    long varScaling[256] = {};
    double varVal[256] = {};
    int settingOffset = -100;
    
    struct SettingsGroup
    {
        char name[256];
        int settingsCount;
        char settingName[256][256];
        long settingVal[256];
    };
    
    int groupCount = 0;
    struct SettingsGroup groups[64] = {};
    
public:
    simvars();
    ~simvars();
    char *view();
    void doKeypress(int keycode);
    void addVar(const char* group, const char* name, bool isBool, long scaling, long val);
    void addSetting(const char* group, const char* name);
    long* readSettings(const char* group, int defaultX, int defaultY, int defaultSize);
    bool isEnabled(const char* group);
    void write(const char *name, double value);
    
private:
    void loadSettings();
    void saveSettings();
    int settingIndex(const char* attribName);
    int settingValue(const char* value);
    void saveGroup(FILE* outfile, const char* group);
    int getVarIdx(int num);
    bool isCorrectType(int idx);
    void getNextVar();
    void getPrevVar();
    void adjustVar(double amount);
};

#endif // _SIMVARS_H_
