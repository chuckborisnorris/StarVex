# StarVex
The hack consists of 2 parts: 
* A hacked ROM (StarVex.sfc) which will create wireframe models for all polygon objects. This will also write vertex/sprite/frame info to a buffer at the beginning of the second SFX RAM bank ($71). Wireframe mode courtesy of Kandowontu and co.
* A C++ application which reads the frame data and plots the appropriate objects using [openGL](https://www.opengl.org). This will also work with a [Helios DAC](https://github.com/Grix/helios_dac) to blit the images, via ILDA, to a laser display.

Currently the game has only been tested with the following setup, don't expect the game to work straight out of the box with a different setup (it certainly wont work on Windows yet!):
* Ubuntu 20.04 / 22.04
* Retroarch emulator system (latest build)
* Snes9x 2010 (1.52) - I will add the codes for the current Snes9x build soon, I promise

To compile the C++ source you will need the openGL library and I suggest using g++ to compile. You can use this command line to compile if you have g++:

g++ -pthread -o test main.cpp HeliosDac.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -std=c++17 -lusb-1.0

...and if there are no errors then you can use this to run the app (after starting the ROM):

sudo ./test

(if there are errors then it is likely you are missing one of the above libraries)

## Finding Code
The modified code written to the frame buffer on the ROM starts with a hex code, that is used to define the type of data that follows (naming conventions are chosen at random):
* 0xbeef - This is the start of the buffer and contains the following header info:
** total amount of data to read (in bytes)
** player hp
** player boost
** player bombs
** boss hp
** boss max hp (currently janky)
* 0xfffe - end of line (shouldn't be necessary but results look much cleaner with it in)
* 0xabcd - change colour (a lot of these are repeated in the ROM code, I need to fix that still)
* 0xface - sprite info:
** type
** scale
** projected x pos
** projected y pos
* All other data is considered to be plotted vertices.

## Debugging
If you start the application, with the ROM running, and see a blank screen then it's most likely that the start of the buffer is in a different place in memory to what the C++ application is expecting.
The easiest way I've found to debug this (on Linux anyway) is to use [GameConqueror](https://linuxhint.com/use-gameconqueror-cheat-engine-linux/). 
* Pause the ROM on the intro screen, once some objects start showing, and open GameConqueror. 
* Search the Retroarch process for the bytearray '00 ef be' and you should see the start of the frame data somewhere around 0x220000 bytes from the start of the CPU RAM (heap memory). 
* If you do not see a value starting with 0x22 then browse each heap address and look for the value CD AB FF 3F repeated in the lines below. 
* Now change the heapAddr variable in the file main.cpp to the searched value with the last 4 digits changed to zeros. e.g. If this address is 0x1234567 then change the heapAddr in main.cpp to 0x1230000.
* That should fix the most common issue.

## Issues
* As I say, this is totally untested on Windows, Mac, Dolphin emus, MAME emus and other distros of Linux. It will 100% not work on Windows atm because Linux and Windows use fundamentally different methods of storing memory tables.
* No crosshairs.
* Missing map, controls, game over and continue screens
* Exiting via Hotkey changes start of framedata buffer, only workaround is to restart the emu system fully.
* Not all sprites are unique and some are still missing.

## Last Notes
The laser display is limited by the number of points being drawn per second. I'd like to see this running on a classic vector monitor but can't get the hardware at the moment. If you have the ability then please feel free to branch and modify the code.
