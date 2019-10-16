@ECHO OFF

SET appDir=%~dp0

"%appDir%Python\bin\Python.exe" "%appDir%Python\Scripts\setpythonpath.py" "%appDir%Python" -s
