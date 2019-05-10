@ECHO OFF

TITLE Running the Jupyter environment...

REM Get the directory containing this batch file
SET OPENCOR_DIR=%~dp0

REM Make sure the path to Python is correct before starting Jupyter
CALL "%OPENCOR_DIR%set_python_path.bat"

REM Start Jupyter
"%OPENCOR_DIR%Python\bin\Python.exe" "%OPENCOR_DIR%Python\Scripts\start_jupyter.py" %*
