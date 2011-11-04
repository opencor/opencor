@ECHO OFF

CALL cleanproj
CALL makeproj %*

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
