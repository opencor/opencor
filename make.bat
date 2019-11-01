@ECHO OFF

SET CrtDir=%~dp0

CALL "%CrtDir%scripts\genericmake" Release %*
