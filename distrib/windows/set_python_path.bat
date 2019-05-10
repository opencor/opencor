@ECHO OFF

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Make sure the path to Python is correct
"%OPENCOR_DIR%Python\bin\Python.exe" "%OPENCOR_DIR%Python\Scripts\set_python_path.py" "%OPENCOR_DIR%Python" -s
