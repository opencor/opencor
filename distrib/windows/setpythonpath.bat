@ECHO OFF

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Make sure the path to Python is correct
"%OPENCOR_DIR%Python\bin\Python.exe" "%OPENCOR_DIR%Python\Scripts\setpythonpath.py" "%OPENCOR_DIR%Python" -s
