@ECHO OFF

SET AppDir=%~dp0

CALL "%AppDir%scripts\genericmake" Release %*
