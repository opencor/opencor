@ECHO OFF

TITLE Running the Jupyter environment...

REM Get the directory containing this batch file
SET OPENCOR_ROOT=%~dp0

REM Make sure the path to Python is correct before starting Jupyter
"%OPENCOR_ROOT%Python\bin\Python.exe" "%OPENCOR_ROOT%Python\Scripts\set_python_path.py" "%OPENCOR_ROOT%Python" -s

REM Start Jupyter
"%OPENCOR_ROOT%Python\bin\Python.exe" "%OPENCOR_ROOT%Python\Scripts\start_jupyter.py" %*
