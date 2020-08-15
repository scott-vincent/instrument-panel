# Pull up all input pins
raspi-gpio set 2,3,4,17,27,22,10,9,11,5,6,13,19,26 pu
raspi-gpio set 14,15,18,23,24,25,8,7,12,16,20 pu
# gpio readall

# Run instrument panel
./instrument-panel
