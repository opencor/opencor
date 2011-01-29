@ECHO OFF

CALL cleanall
CALL makeall

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
