@ECHO OFF

TITLE Running the Jupyter Lab environment...

REM Get the directory containing this batch file
SET appDir=%~dp0

REM Start a Jupyter Lab server
CALL "%appDir%runjupyter.bat" lab --ip 127.0.0.1 %*
