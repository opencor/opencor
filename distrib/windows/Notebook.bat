@ECHO OFF

TITLE Running the Jupyter notebook environment...

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Start Jupyter natebook
CALL "%OPENCOR_DIR%Jupyter.bat" notebook %*
