@ECHO OFF

TITLE Cleaning the OpenCOR environment...

CD build

mingw32-make -j clean

DEL /F *Make* install_manifest.txt OpenCOR* version.txt 1> NUL 2> NUL
RMDIR /S /Q _CPack_Packages 3rdparty CMakeFiles src 1> NUL 2> NUL

CD ..
