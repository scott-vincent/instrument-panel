cmake_minimum_required(VERSION 3.16)
project(instrument_panel)

set(CMAKE_CXX_STANDARD 17)

add_executable(instrument-panel
        instrument-panel/simvarDefs.cpp
        instrument-panel/simvars.cpp
        instrument-panel/globals.cpp
        instrument-panel/knobs.cpp
        instrument-panel/instrument.cpp
        instrument-panel/instrument-panel.cpp
        instrument-panel/instruments/adf.cpp
        instrument-panel/instruments/adi.cpp
        instrument-panel/instruments/alt.cpp
        instrument-panel/instruments/annunciator.cpp
        instrument-panel/instruments/asi.cpp
        instrument-panel/instruments/digitalClock.cpp
        instrument-panel/instruments/egt.cpp
        instrument-panel/instruments/fuel.cpp
        instrument-panel/instruments/hi.cpp
        instrument-panel/instruments/nav.cpp
        instrument-panel/instruments/oil.cpp
        instrument-panel/instruments/radCom.cpp
        instrument-panel/instruments/rpm.cpp
        instrument-panel/instruments/tc.cpp
        instrument-panel/instruments/trimFlaps.cpp
        instrument-panel/instruments/vac.cpp
        instrument-panel/instruments/vor1.cpp
        instrument-panel/instruments/vor2.cpp
        instrument-panel/instruments/vsi.cpp
        instrument-panel/instruments/alternate/altFast.cpp
        instrument-panel/instruments/alternate/asiFast.cpp
        instrument-panel/instruments/alternate/asiSupersonic.cpp
        instrument-panel/instruments/alternate/gForce.cpp
        instrument-panel/instruments/alternate/rpmPercent.cpp
        instrument-panel/instruments/alternate/vsiExtreme.cpp
        instrument-panel/instruments/learjet/adiLearjet.cpp
        instrument-panel/instruments/savageCub/asiSavageCub.cpp
        instrument-panel/instruments/savageCub/rpmSavageCub.cpp
        instrument-panel/instruments/savageCub/comSavageCub.cpp
        instrument-panel/instruments/savageCub/xpdrSavageCub.cpp
        instrument-panel/instruments/spitfire/asiSpitfire.cpp
        instrument-panel/instruments/spitfire/rpmSpitfire.cpp
        instrument-panel/instruments/spitfire/vsiSpitfire.cpp
        instrument-panel/instruments/spitfire/boostSpitfire.cpp)

target_compile_options(instrument-panel PRIVATE
        "-Wall"
        "-Wextra"
        "-pedantic")

target_include_directories(instrument-panel PRIVATE
        instrument-panel
        instrument-panel/instruments)

target_link_libraries(instrument-panel
        wiringPi
        pthread
        allegro
        allegro_image
        allegro_font)
