@ECHO OFF

TITLE Running the Jupyter Notebook environment...

CALL "%~dp0.Python\Scripts\runjupyter.bat" notebook --ip 127.0.0.1 %*
