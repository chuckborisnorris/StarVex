# StarVex
The hack consists of 2 parts: 
* A hacked ROM which will create wireframe models for all polygon objects. This will also write vertex/sprite/frame info to a buffer at the beginning of the second SFX RAM bank ($71).
* A C++ application which reads the frame data and plots the appropriate objects using openGL. This will also work with a Helios DAC to blit the images, via ILDA, to a laser display. Details on the Helios DAC project can be found here (link).

Currently the game has only been tested with the following setup, don't expect the game to work straight out of the box with a different setup (it certainly wont work on Windows yet!):
* Ubuntu 20.04 / 22.04
* Retroarch emulator system (latest build)
* Snes9x 2010 (1.52) - I will add the codes for the current Snes9x build soon, I promise

To compile the C++ source you will need the openGL library and I suggest using g++ to compile. You can use this command line to compile if you have g++:

g++ -pthread -o test main.cpp HeliosDac.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -std=c++17 -lusb-1.0

...and if there are no errors then you can use this to run the app (after starting the ROM):

sudo ./test

(if there are errors then it is likely you are missing one of the above libraries

## Finding Code
The code which writes the frame buffer on the ROM can be searched easily by looking for the value m_vaddr in all files in the StarFox(greenframe)/SG folder (Notepad++ is you friend here!). There are certain hex codes used to define the type of data that follows:
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
The easiest way I've found (on Linux anyway) is to use GameConqueror. Searching the Retroarch process for the bytearray '00 ef be' should give the start of the frame data somewhere around 0x220000 bytes from the start of the CPU RAM (heap memory).

## Issues
* As I say, this is totally untested on Windows, Mac, Dolphin emus, MAME emus and other distros of Linux. It will 100% not work on Windows atm because Linux and Windows use fundamentally different methods of storing memory tables.
* No crosshairs.
* Missing map, controls, game over and continue screens
* Exiting via Hotkey changes start of framedata buffer, only workaround is to restart the emu system fully.
* Not all sprites are unique and some are still missing.
