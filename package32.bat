@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

CALL clean
CALL make32

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    TITLE Packaging OpenCOR...

    CD build

    cpack

    SET ExitCode=!ERRORLEVEL!

    CD ..
)

EXIT /B !ExitCode!
