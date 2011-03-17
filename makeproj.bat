@ECHO OFF

TITLE Making OpenCOR...

ECHO.
ECHO ---------------------------------
ECHO Making the GUI version of OpenCOR
ECHO ---------------------------------
ECHO.

CD build

cmake -G "MinGW Makefiles" ..
mingw32-make -j %1

ECHO.
ECHO -------------------------------------
ECHO Making the console version of OpenCOR
ECHO -------------------------------------
ECHO.

CD ..\winConsole\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j %1

CD ..\..
