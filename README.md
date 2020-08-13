#MICROSOFT FLIGHT SIMULATOR 2020 - INSTRUMENT PANEL

An instrument panel for Microsoft Flight Simulator 2020. This program is designed to run
on a separate computer to the one running FS2020. It can run on either a PC or a
Raspberry Pi.

This is a work in progress but the idea is to read the variables from the flight
simulator over the network. A small program will be required on the host PC to obtain
these variables using the FS2020 SDK and make them available over the LAN.

The simvars part of this program will be enhanced to read them from the LAN. Currently
the values are just simulated.

This program was heavily inspired by Dave Ault and contains original artwork by him.
 
  http://82.7.215.98/Learjet45Chimera/index.html
  https://hangar45.net/hangar-45-forum/topic/standby-gauge-software-by-dave-ault
 
It has been completely rewritten and updated to use Allegro5. Hopefully,
Allegro5 will support hardware acceleration on the Raspberry Pi 4 soon!

NOTE: Allegro5 must be built on RasPi4 as a standard Linux build, not the
specific Raspberry Pi build, i.e.:

  mkdir build
    cd allegro5/build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make && sudo make install

#KEYS

p ........ Adjust position and size of individual instruments.
v ........ Adjust FlightSim variables. Simulates changes even if no FlightSim connected.
m ........ Move the display to the next monitor if multiple monitors are connected.
s ........ Enable/disable shadows on instruments. Shadows give a more realistic 3D look.
Esc ...... Quit the program.

To make adjustments use the arrow keys. Up/down arrows select the previous or next
setting and left/right arrows change the value. You can also use numpad left/right
arrows to make larger adjustments.
