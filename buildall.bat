@ECHO OFF

TITLE Building OpenCOR...

ECHO ---------------------------------------
ECHO Building the console version of OpenCOR
ECHO ---------------------------------------

CD winConsole\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

COPY /Y OpenCOR.exe ..\..\build
RENAME ..\..\build\OpenCOR.exe OpenCOR.com

ECHO -----------------------------------
ECHO Building the GUI version of OpenCOR
ECHO -----------------------------------

CD ..\..\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

CD ..
