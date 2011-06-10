@ECHO OFF

CALL cleanproj
CALL makeproj %1

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
