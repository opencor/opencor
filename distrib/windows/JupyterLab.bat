@ECHO OFF

TITLE Running the Jupyter Lab environment...

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Start Jupyter Lab
CALL "%OPENCOR_DIR%run_jupyter.bat" lab --ip 127.0.0.1 %*
