@ECHO OFF

TITLE Running OpenCOR's tests...

IF NOT EXIST build\tests\run.exe GOTO Information

build\tests\run %*

GOTO End

:Information

ECHO OpenCOR's tests must first be built before being run.
ECHO.

:End
