@ECHO OFF

TITLE Running OpenCOR's tests...

IF NOT EXIST build\tests\runtests.exe GOTO Information

build\tests\runtests %*

GOTO End

:Information

ECHO OpenCOR's tests must first be built before being run.
ECHO.

:End
