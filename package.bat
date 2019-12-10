@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0

CALL %AppDir%clean
CALL %AppDir%make %*

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    TITLE Packaging OpenCOR...

    SET OrigDir=%CD%

    CD %AppDir%build

    cpack

    SET ExitCode=!ERRORLEVEL!

    CD %OrigDir%
)

EXIT /B !ExitCode!
