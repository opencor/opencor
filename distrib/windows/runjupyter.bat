@ECHO OFF

TITLE Running the Jupyter environment...

SET CrtDir=%~dp0

"%CrtDir%Python\bin\Python.exe" "%CrtDir%Python\Scripts\set_python_path.py" "%CrtDir%Python" -s

"%CrtDir%Python\bin\Python.exe" "%CrtDir%Python\Scripts\start_jupyter.py" %*
