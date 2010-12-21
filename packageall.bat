@ECHO OFF

CALL cleanall
CALL buildall

TITLE Packaging OpenCOR...

CD build

cpack

CD ..
