@ECHO OFF

TITLE Running the Jupyter Lab environment...

CALL "%~dp0.Python\Scripts\runjupyter.bat" lab --ip 127.0.0.1 %*
