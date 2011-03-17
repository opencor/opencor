@ECHO OFF

TITLE Running the OpenCOR environment...

IF NOT EXIST build\OpenCOR.com GOTO Information
IF NOT EXIST build\OpenCOR.exe GOTO Information

build\OpenCOR %*

GOTO End

:Information

ECHO OpenCOR must first be built before being run.
ECHO.

:End
