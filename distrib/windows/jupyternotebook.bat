@ECHO OFF

TITLE Running the Jupyter notebook environment...

REM Get the directory containing this batch file
SET appDir=%~dp0

REM Start a Jupyter notebook server
CALL "%appDir%runjupyter.bat" notebook --ip 127.0.0.1 %*
