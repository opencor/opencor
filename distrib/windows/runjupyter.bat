@ECHO OFF

TITLE Running the Jupyter environment...

REM Get the directory containing this batch file
SET appDir=%~dp0

REM Make sure the path to Python is correct before starting Jupyter
CALL "%appDir%setpythonpath.bat"

REM Start Jupyter
"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\start_jupyter.py" %*
