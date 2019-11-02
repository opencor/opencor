@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET HomeDir=%~dp0

CALL "%HomeDir%clean"
CALL "%HomeDir%make" %*

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    TITLE Packaging OpenCOR...

    SET OrigDir=%CD%

    CD "%HomeDir%build"

    cpack

    SET ExitCode=!ERRORLEVEL!

    CD "%OrigDir%"
)

EXIT /B !ExitCode!
