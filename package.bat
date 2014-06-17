@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

CALL clean
CALL make

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    TITLE Packaging OpenCOR...

    CD build

    cpack -C CPackConfig.cmake

    SET ExitCode=!ERRORLEVEL!

    CD ..
)

EXIT /B !ExitCode!
