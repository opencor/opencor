@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0

IF EXIST !AppDir!build (
    FOR %%X IN (ninja.exe) DO (
        SET NinjaFound=%%~$PATH:X
    )

    IF DEFINED NinjaFound (
        SET Generator=Ninja
        SET CMakeGenerator=Ninja
    ) ELSE (
        SET Generator=JOM
        SET CMakeGenerator=NMake Makefiles JOM
    )

    TITLE Generating OpenCOR's documentation using !Generator!...

    SET OrigDir=!CD!

    CD !AppDir!build

    cmake -G "!CMakeGenerator!" ../doc

    SET ExitCode=!ERRORLEVEL!

    IF NOT !ExitCode! EQU 0 (
        CD !OrigDir!

        EXIT /B !ExitCode!
    )

    IF DEFINED NinjaFound (
        ninja

        SET ExitCode=!ERRORLEVEL!
    ) ELSE (
        jom

        SET ExitCode=!ERRORLEVEL!
    )

    CD !OrigDir!

    EXIT /B !ExitCode!
) ELSE (
    ECHO OpenCOR's build directory is missing.

    EXIT /B 1
)
