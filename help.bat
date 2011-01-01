@ECHO OFF

TITLE Building OpenCOR's help...

CD doc

qcollectiongenerator OpenCOR.qhcp -o ..\res\OpenCOR.qhc

CD ..
