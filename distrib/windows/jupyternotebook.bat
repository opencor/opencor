@ECHO OFF

TITLE Running the Jupyter Notebook environment...

SET appDir=%~dp0

CALL "%appDir%runjupyter.bat" notebook --ip 127.0.0.1 %*
