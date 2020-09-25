# MICROSOFT FLIGHT SIMULATOR 2020 - INSTRUMENT PANEL

![Screenshot](Screenshot.jpg)

# QUICK START

Download the following two zip files.

Link: [Latest release of Instrument Panel for Windows](https://github.com/scott-vincent/instrument-panel/releases/download/v1.3.0/instrument-panel-v1.3.0-Windows-x64.zip)

Link: [Latest release of Instrument Data Link for Windows](https://github.com/scott-vincent/instrument-data-link/releases/download/v1.3.0/instrument-data-link-v1.3.0-windows-x64.zip)

Unzip instrument-data-link into its own folder and double-click instrument-data-link.exe to run it.

Unzip instrument-panel into its own folder. If you are running it on the same PC as FS2020 the default settings are correct so just double-click instrument-panel.exe to run it. Press M to switch the display to a different monitor so it's not on the same monitor as FS2020. If you then press Escape to exit the program the settings will be saved and it will run on the same monitor next time.

To run instrument-panel on a different PC, unzip instrument-panel to a folder on the new PC. Edit settings/instrument-panel.json and in the "Data Link" section change the IP address of the "Host" to the address where FS2020 is running on your local network, e.g. 192.168.0.1 - You can find the correct address of your host by running a command prompt on the host machine and running ipconfig, then scroll back and look for the first "IPv4 Address" line.

# RELEASES

Ready-built releases for Windows x64 and Raspberry Pi 4 can be found by clicking on the Releases section on the right of this readme.

# INTRODUCTION

An instrument panel for MS FlightSim 2020. This program is designed to run
on a separate computer to the one running FS2020 but you can just run it on
a different monitor if you wish. It can run on either a PC or a Raspberry Pi.

You can see it running on a Raspberry Pi 4 here: https://www.youtube.com/watch?v=vA6rveEP_9o&t=4s

It requires the companion program from here

  https://github.com/scott-vincent/instrument-data-link

The companion program runs on the same host as MS FS2020 and collects all
the data required by the instrument panel.

The instrument panel connects to the instrument-data-link over your network and
receives the data at regular intervals so that the instruments can be drawn with
up-to-date values.

You can run as many instrument panels as you like. They all connect to the same
data-link. Each panel must be configured to point to the host where data-link
is running. You do this by editing the settings file and updating the Data Link
Host IP address. If you just want to test the instrument panel on the same
host where FS2020 is running leave the IP address as 127.0.0.1 and it will
connect to the local host.

This program was heavily inspired by Dave Ault and contains original artwork by him.

  http://www.learjet45chimera.co.uk/
  
  https://hangar45.net/hangar-45-forum/topic/standby-gauge-software-by-dave-ault
  
It has been completely rewritten and updated to use Allegro5.

NOTE: Allegro5 must be built on RasPi4 as a standard Linux build, not the
specific Raspberry Pi build, i.e.:
```
  mkdir build
  cd allegro5/build
  cmake .. -DCMAKE_BUILD_TYPE=Release
  make && sudo make install
```
### KEYS
```
p ........ Adjust position and size of individual instruments.
v ........ Adjust FlightSim variables. Simulates changes even if no FlightSim connected.
m ........ Move the display to the next monitor if multiple monitors are connected.
s ........ Enable/disable shadows on instruments. Shadows give a more realistic 3D look.
Esc ...... Quit the program.
```
To make adjustments use the arrow keys. Up/down arrows select the previous or next
setting and left/right arrows change the value. You can also use numpad left/right
arrows to make larger adjustments.

You can choose which instruments are included in the panel by setting the Enabled
attribute for each instrument in the settings file:
```
  settings/instrument-panel.json
```
On Raspberry Pi you can configure hardware Rotary Encoders for each instrument.
Each rotary encoder is connected to two BCM GPIO pins (+ ground on centre pin).
See individual instruments for pins used. Not all instruments have manual controls.

# Donate

If you find this project useful, would like to see it developed further or would just like to buy the author a beer, please consider a small donation.

[<img src="donate.svg" width="210" height="40">](https://paypal.me/scottvincent2020)

# ADDITIONAL PHOTOS

Panel Front
![Panel Front](Panel_Front.jpg)

Panel Back
![Panel Back](Panel_Back.jpg)

Panel Wiring
![Panel Wiring](Panel_Wiring.jpg)
