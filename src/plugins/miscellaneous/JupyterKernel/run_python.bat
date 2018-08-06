@ECHO OFF

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Start our script that runs Python code in OpenCOR
"%OPENCOR_DIR%Python\bin\Python.exe" "%OPENCOR_DIR%run_python.py" %*
