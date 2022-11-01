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
    FBW_A320,
    BOEING_747,
    BOEING_787,
    SUPERMARINE_SPITFIRE,
    F15_EAGLE,
    F18_HORNET,
    JUSTFLIGHT_PA28,
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
    const char* Airbus_A320_Text = "Airbus A320";
    const int Airbus_A320_Len = 11;
    const char* FBW_A320_Text = "FBW";
    const int FBW_A320_Len = 3;
    const char* Boeing_747_Text = "Salty Boeing 747";
    const int Boeing_747_Len = 16;
    const char* Boeing_787_Text = "Boeing 787";
    const int Boeing_787_Len = 10;
    const char* Supermarine_Spitfire_Text = "FlyingIron Spitfire";
    const int Supermarine_Spitfire_Len = 19;
    const char* F15_Eagle_Text = "DCD F-15";
    const int F15_Eagle_Len = 8;
    const char* F18_Hornet_Text = "Boeing F/A 18E";
    const int F18_Hornet_Len = 14;
    const char* JustFlight_PA28_Text = "Just Flight PA28";
    const int JustFlight_PA28_Len = 16;
    const int FastAircraftSpeed = 198;

    const char* BitmapDir = "bitmaps/";
    const char* SettingsDir = "settings/";
    const char* SettingsFile = "settings/instrument-panel.json";

    simvars* simVars = NULL;
    knobs* hardwareKnobs = NULL;

    ALLEGRO_FONT* font = NULL;
    ALLEGRO_DISPLAY* display = NULL;

    int displayX = 0;
    int displayY = 0;
    int displayWidth = 0;
    int displayHeight = 0;

    char error[256] = {'\0'};
    char info[256] = { '\0' };

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
