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
    AIRBUS_A310,
    FBW,
    BOEING_747,
    BOEING_787,
    SUPERMARINE_SPITFIRE,
    F15_EAGLE,
    F18_HORNET,
    HAWKER_HUNTER,
    JUSTFLIGHT_PA28,
    KODIAK_100,
    HELI_H135,
    GLIDER,
    ORNITHOPTER,
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
    const char* Hawker_Hunter_Text = "Hawker Hunter";
    const int Hawker_Hunter_Len = 13;
    const char* JustFlight_PA28_Text = "Just Flight PA28";
    const int JustFlight_PA28_Len = 16;
    const char* SWS_Kodiak_100_Text = "Kodiak 100";
    const int SWS_Kodiak_100_Len = 10;
    const char* Heli_H135_Text = "Airbus H135";
    const int Heli_H135_Len = 11;
    const char* Glider_DG_LS8_Text = "Asobo LS8";
    const int Glider_DG_LS8_Len = 9;
    const char* Glider_DG_1001E_Text = "Asobo DG1001E";
    const int Glider_DG_1001E_Len = 13;
    const char* Ornithopter_Text = "Ornithopter";
    const int Ornithopter_Len = 11;
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
    bool monitorFullscreen = true;
    int monitorWidth = 1200;
    int monitorHeight = 800;
    int monitorPositionX = 0;
    int monitorPositionY = 0;
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
