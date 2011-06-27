@ECHO OFF

TITLE Running the OpenCOR environment...

IF NOT EXIST build\bin\OpenCOR.com GOTO Information
IF NOT EXIST build\bin\OpenCOR.exe GOTO Information

build\bin\OpenCOR %*

GOTO End

:Information

ECHO OpenCOR must first be built before being run.
ECHO.

:End
