@ECHO OFF

TITLE Building OpenCOR...

CD doc

qcollectiongenerator OpenCOR.qhcp -o ..\res\OpenCOR.qhc

CD ..\build

cmake -G "MinGW Makefiles" ..
mingw32-make -j

CD ..
