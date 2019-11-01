@ECHO OFF

TITLE Running the Jupyter Notebook environment...

SET CrtDir=%~dp0

CALL "%CrtDir%runjupyter.bat" notebook --ip 127.0.0.1 %*
