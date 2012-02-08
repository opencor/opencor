@ECHO OFF

TITLE Running OpenCOR...

IF NOT EXIST build\bin\OpenCOR.com GOTO Information
IF NOT EXIST build\bin\OpenCOR.exe GOTO Information

build\bin\OpenCOR %*

GOTO End

:Information

ECHO OpenCOR must first be built before being run.
ECHO.

:End
