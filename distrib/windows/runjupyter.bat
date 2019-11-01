@ECHO OFF

TITLE Running the Jupyter environment...

SET appDir=%~dp0

"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\set_python_path.py" "%appDir%Python" -s

"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\start_jupyter.py" %*
