@ECHO OFF

SET HomeDir=%~dp0

CALL "%HomeDir%scripts\genericmake" Tests %*
