@ECHO OFF

TITLE Running the IPython environment...

SET AppDir=%~dp0..\..\
SET PYDEVD_DISABLE_FILE_VALIDATION=1

"%AppDir%.Python\bin\Python.exe" "%AppDir%.Python\Scripts\set_python_path.py" "%AppDir%.Python" -s

"%AppDir%.Python\bin\Python.exe" "%AppDir%.Python\Scripts\start_ipython.py" %*
