@ECHO OFF

CALL cleanprojall
CALL makeproj

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
