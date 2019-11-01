@ECHO OFF

TITLE Running the Jupyter environment...

SET CrtDir=%~dp0

"%CrtDir%..\bin\Python.exe" "%CrtDir%\set_python_path.py" "%CrtDir%.." -s

"%CrtDir%..\bin\Python.exe" "%CrtDir%\start_jupyter.py" %*
