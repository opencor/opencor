@ECHO OFF

TITLE Running the Jupyter Lab environment...

SET appDir=%~dp0

CALL "%appDir%runjupyter.bat" lab --ip 127.0.0.1 %*
