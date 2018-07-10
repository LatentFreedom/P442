#! /bin/bash

# Open the needed windows to debug STM Board
##############################################

# Window 1: openocd runs
gnome-terminal  --geometry=100x100+1000+1000 -e ./openocd_debug.sh
# Window 2: screen runs so we can see output from STM Board
gnome-terminal -e ./screen_debug.sh
# Window 3: Run GDB on the ch.elf file in the build directory
gnome-terminal -e ./arm_gdb_debug.sh
