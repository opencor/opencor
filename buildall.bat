@ECHO OFF

TITLE Building OpenCOR...

CD build

cmake -G "MinGW Makefiles" ..
mingw32-make

CD ..
