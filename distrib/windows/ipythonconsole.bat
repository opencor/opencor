@ECHO OFF

TITLE Running the Jupyter Lab environment...

SET appDir=%~dp0

CALL "%appDir%runjupyter.bat" console --kernel OpenCOR %*
