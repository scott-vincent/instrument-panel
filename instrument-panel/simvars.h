#ifndef _SIMVARS_H_
#define _SIMVARS_H_

#include <thread>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define SOCKADDR sockaddr
#define closesocket close
#endif
#include "globals.h"
#include "simvarDefs.h"

extern globalVars globals;

class simvars {
public:
    SimVars simVars;

private:
    std::thread* dataLinkThread = nullptr;
    char settingsFile[256];

    SOCKET writeSockfd = INVALID_SOCKET;
    sockaddr_in writeAddr;
    Request writeRequest;

    int currentVar = 0;
    int varCount = 0;
    char varGroup[256][256] = {};
    char varName[256][256] = {};
    int varOffset[256] = {};
    bool varIsBool[256] = {};
    double varScaling[256] = {};
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
    simvars(const char *settingsFile);
    ~simvars();
    char *view();
    void doKeypress(int keycode);
    void addVar(const char* group, const char* name, bool isBool, double scaling, double val);
    void addSetting(const char* group, const char* name);
    long* readSettings(const char* group, int defaultX, int defaultY, int defaultSize);
    bool isEnabled(const char* group);
    void write(EVENT_ID eventId, double value = 0);
    
private:
    void loadSettings();
    void saveSettings();
    int settingIndex(const char* attribName);
    int settingValue(const char* value);
    void showCentre(FILE* outfile, const char* group, int x, int y, int size);
    void saveGroup(FILE* outfile, const char* group);
    int getVarIdx(int num);
    bool isCorrectType(int idx);
    void getNextVar();
    void getPrevVar();
    void adjustVar(double amount);
};

#endif // _SIMVARS_H_
