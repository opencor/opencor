@ECHO OFF

TITLE Making OpenCOR and its tests...

ECHO.
ECHO ----------------------------
ECHO Making OpenCOR and its tests
ECHO ----------------------------
ECHO.

CD build

cmake -G "MinGW Makefiles" -DENABLE_TESTING=True ..
mingw32-make %*

CD ..
