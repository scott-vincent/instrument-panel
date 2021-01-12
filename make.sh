echo Building instrument-panel
cd instrument-panel
g++ -lwiringPi -lpthread -lallegro -lallegro_image -lallegro_font \
    -o instrument-panel \
    -I . \
    -I instruments \
    simvarDefs.cpp \
    simvars.cpp \
    knobs.cpp \
    instrument.cpp \
    instruments/adf.cpp \
    instruments/adi.cpp \
    instruments/alt.cpp \
    instruments/annunciator.cpp \
    instruments/asi.cpp \
    instruments/digitalClock.cpp \
    instruments/egt.cpp \
    instruments/fuel.cpp \
    instruments/hi.cpp \
    instruments/nav.cpp \
    instruments/oil.cpp \
    instruments/radCom.cpp \
    instruments/rpm.cpp \
    instruments/rpmPercent.cpp \
    instruments/tc.cpp \
    instruments/trimFlaps.cpp \
    instruments/vac.cpp \
    instruments/vor1.cpp \
    instruments/vor2.cpp \
    instruments/vsi.cpp \
    instrument-panel.cpp \
    instruments/genericFast/altFast.cpp \
    instruments/genericFast/asiFast.cpp \
    instruments/learjet/adiLearjet.cpp \
    instruments/savageCub/asiSavageCub.cpp \
    instruments/savageCub/rpmSavageCub.cpp \
    instruments/savageCub/comSavageCub.cpp \
    instruments/savageCub/xpdrSavageCub.cpp \
    || exit
echo Done
echo Run with: ./run.sh
