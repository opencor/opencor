@ECHO OFF

TITLE Cleaning the OpenCOR environment...

CD build

mingw32-make -j clean

DEL /F *Make* 1> NUL 2> NUL
RMDIR /S /Q CMakeFiles src 1> NUL 2> NUL

CD ..
