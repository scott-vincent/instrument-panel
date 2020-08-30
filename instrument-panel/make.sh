g++ -lwiringPi -lpthread -lallegro -lallegro_image -lallegro_font \
    -o instrument-panel \
    -I . \
    -I instruments \
    simvarDefs.cpp \
    simvars.cpp \
    knobs.cpp \
    instrument.cpp \
    instruments/adi.cpp \
    instruments/adiLearjet.cpp \
    instruments/alt.cpp \
    instruments/altLearjet.cpp \
    instruments/annunciator.cpp \
    instruments/asi.cpp \
    instruments/asiLearjet.cpp \
    instruments/digitalClock.cpp \
    instruments/egt.cpp \
    instruments/hi.cpp \
    instruments/oil.cpp \
    instruments/tc.cpp \
    instruments/trimFlaps.cpp \
    instruments/vac.cpp \
    instruments/vsi.cpp \
    instrument-panel.cpp \
    || exit
echo Built instrument-panel
echo Run with: ./run.sh
