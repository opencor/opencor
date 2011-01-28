@ECHO OFF

TITLE Cleaning the OpenCOR environment...

ECHO.
ECHO ---------------------------------------
ECHO Cleaning the console version of OpenCOR
ECHO ---------------------------------------
ECHO.

CD winConsole\build

mingw32-make -j clean

DEL /F *Make* OpenCOR* 1> NUL 2> NUL
RMDIR /S /Q 3rdparty CMakeFiles 1> NUL 2> NUL

ECHO.
ECHO -----------------------------------
ECHO Cleaning the GUI version of OpenCOR
ECHO -----------------------------------
ECHO.

CD ..\..\build

mingw32-make -j clean

DEL /F *Make* install_manifest.txt OpenCOR* version.txt 1> NUL 2> NUL
RMDIR /S /Q _CPack_Packages 3rdparty CMakeFiles src 1> NUL 2> NUL

CD ..
