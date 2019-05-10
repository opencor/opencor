@ECHO OFF

TITLE Running the Jupyter notebook environment...

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Start Jupyter notebook
CALL "%OPENCOR_DIR%run_jupyter.bat" notebook --ip 127.0.0.1 %*
