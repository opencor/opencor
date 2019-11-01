@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET CrtDir=%~dp0

CALL "%CrtDir%clean"
CALL "%CrtDir%make" %*

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    TITLE Packaging OpenCOR...

    SET OrigDir=%CD%

    CD "%CrtDir%build"

    cpack

    SET ExitCode=!ERRORLEVEL!

    CD "%OrigDir%"
)

EXIT /B !ExitCode!
