@ECHO OFF

TITLE Running the Jupyter Lab environment...

REM Get the directory containing this batch file
SET appDir=%~dp0

REM Start an IPython console attached to an OpenCOR kernel
CALL "%appDir%runjupyter.bat" console --kernel OpenCOR %*
