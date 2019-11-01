@ECHO OFF

TITLE Running the Jupyter Lab environment...

SET CrtDir=%~dp0

CALL "%CrtDir%runjupyter.bat" lab --ip 127.0.0.1 %*
