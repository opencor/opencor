@ECHO OFF

TITLE Running the Jupyter environment...

SET AppDir=%~dp0..\..\

"%AppDir%Python\bin\Python.exe" "%AppDir%Python\Scripts\set_python_path.py" "%AppDir%Python" -s

"%AppDir%Python\bin\Python.exe" "%AppDir%Python\Scripts\start_jupyter.py" %*
