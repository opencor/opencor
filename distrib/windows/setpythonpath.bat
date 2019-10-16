@ECHO OFF

REM Get the directory containing this batch file
SET appDir=%~dp0

REM Make sure the path to Python is correct
"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\setpythonpath.py" "%appDir%Python" -s
