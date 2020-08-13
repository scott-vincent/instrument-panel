g++ -lallegro -lallegro_image -lallegro_font -o instrument-panel \
    -I . \
    -I instruments \
    -I /opt/vc/include \
    simvars.cpp \
    instrument.cpp \
    instruments/airspeedIndicator.cpp \
    instruments/altimeter.cpp \
    instruments/attitudeIndicator.cpp \
    instrument-panel.cpp \
    || exit
echo Built instrument-panel
