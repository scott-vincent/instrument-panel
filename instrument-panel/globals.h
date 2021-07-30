#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#ifndef _WIN32
#include <cstring>
#define _stricmp strcasecmp
#endif

class simvars;
class knobs;

enum Aircraft {
    UNDEFINED,
    NO_AIRCRAFT,
    CESSNA_152,
    CESSNA_172,
    CESSNA_CJ4,
    SAVAGE_CUB,
    SHOCK_ULTRA,
    ASOBO_A320NEO,
    FBW_A320NEO,
    SUPERMARINE_SPITFIRE,
    F15_EAGLE,
    OTHER_AIRCRAFT,
    OTHER_AIRCRAFT2
};

struct globalVars
{
    const char* Cessna_152_Text = "Cessna 152";
    const int Cessna_152_Len = 10;
    const char* Cessna_172_Text = "Cessna Skyhawk";
    const int Cessna_172_Len = 14;
    const char* Cessna_CJ4_Text = "Cessna CJ4";
    const int Cessna_CJ4_Len = 10;
    const char* Savage_Cub_Text = "Asobo Savage Cub";
    const int Savage_Cub_Len = 16;
    const char* Shock_Ultra_Text = "Savage Shock Ultra";
    const int Shock_Ultra_Len = 18;
    const char* Asobo_A320neo_Text = "Airbus A320 Neo";
    const int Asobo_A320neo_Len = 15;
    const char* FBW_A320neo_Text = "FBW Airbus A320 Neo";
    const int FBW_A320neo_Len = 19;
    const char* Supermarine_Spitfire_Text = "Supermarine Spitfire";
    const int Supermarine_Spitfire_Len = 20;
    const char* F15_Eagle_Text = "DCD F-15";
    const int F15_Eagle_Len = 8;
    const int FastAircraftSpeed = 195;
    const int SupersonicAircraftSpeed = 666;

    const char* BitmapDir = "bitmaps/";
    const char* SettingsDir = "settings/";
    const char* SettingsFile = "settings/instrument-panel.json";

    simvars* simVars = nullptr;
    knobs* hardwareKnobs = nullptr;

    ALLEGRO_FONT* font = nullptr;
    ALLEGRO_DISPLAY* display = nullptr;

    int displayX = 0;
    int displayY = 0;
    int displayWidth = 0;
    int displayHeight = 0;

    char error[256] = {'\0'};

    char dataLinkHost[64] = "127.0.0.1";
    int dataLinkPort = 52020;
    int dataRateFps = 30;
    int startOnMonitor = 0;
    Aircraft aircraft;
    char lastAircraft[32];

    bool quit = false;
    bool arranging = false;
    bool simulating = false;
    bool dataLinked = false;
    bool connected = false;
    bool electrics = false;
    bool avionics = false;
    bool enableShadows = true;
};

#endif // _GLOBALS_H_
