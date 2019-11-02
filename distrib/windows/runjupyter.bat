@ECHO OFF

TITLE Running the Jupyter environment...

SET HomeDir=%~dp0..\..\

"%HomeDir%Python\bin\Python.exe" "%HomeDir%Python\Scripts\set_python_path.py" "%HomeDir%Python" -s

"%HomeDir%Python\bin\Python.exe" "%HomeDir%Python\Scripts\start_jupyter.py" %*
