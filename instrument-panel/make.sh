g++ -lwiringPi -lpthread -lallegro -lallegro_image -lallegro_font \
    -o instrument-panel \
    -I . \
    -I instruments \
    simvars.cpp \
    knobs.cpp \
    instrument.cpp \
    instruments/airspeedIndicator.cpp \
    instruments/altimeter.cpp \
    instruments/attitudeIndicator.cpp \
    instrument-panel.cpp \
    || exit
echo Built instrument-panel
echo Run with: ./run.sh
