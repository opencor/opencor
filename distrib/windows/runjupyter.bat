@ECHO OFF

TITLE Running the Jupyter environment...

SET appDir=%~dp0

CALL "%appDir%setpythonpath.bat"

"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\start_jupyter.py" %*
