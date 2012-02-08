@ECHO OFF

CALL clean
CALL make %*

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
