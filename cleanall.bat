@ECHO OFF

TITLE Cleaning the OpenCOR environment...

CD build

mingw32-make -j clean

DEL /F *Make* install_manifest.txt OpenCOR* 1> NUL 2> NUL
RMDIR /S /Q CMakeFiles extra src _CPack_Packages 1> NUL 2> NUL

CD ..
