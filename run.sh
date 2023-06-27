cd instrument-panel
if [ -f /usr/local/bin/fs-flip.dat ]
then
  DISPLAY=:0 xrandr --output HDMI-1 --rotate inverted
else
  DISPLAY=:0 xrandr --output HDMI-1 --rotate normal
fi
./instrument-panel
