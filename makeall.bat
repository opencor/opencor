@ECHO OFF

TITLE Building OpenCOR...

ECHO.
ECHO ---------------------------------------
ECHO Building the console version of OpenCOR
ECHO ---------------------------------------
ECHO.

CD winConsole\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

ECHO.
ECHO -----------------------------------
ECHO Building the GUI version of OpenCOR
ECHO -----------------------------------
ECHO.

CD ..\..\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

CD ..
