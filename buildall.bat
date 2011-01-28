@ECHO OFF

TITLE Building OpenCOR...

ECHO --------------------------------------------
ECHO Building the command line version of OpenCOR
ECHO --------------------------------------------

CD winCmdLine\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

ECHO -----------------------------------
ECHO Building the GUI version of OpenCOR
ECHO -----------------------------------

CD ..\..\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

CD ..
