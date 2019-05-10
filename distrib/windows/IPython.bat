@ECHO OFF

TITLE Running the Jupyter Lab environment...

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Start an IPython console attached to an OpenCOR kernel
CALL "%OPENCOR_DIR%run_jupyter.bat" console --kernel OpenCOR %*
