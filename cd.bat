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

    IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat" (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) ELSE (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    )

    SET OrigDir=!CD!

    CD !AppDir!build

    cmake -G "!CMakeGenerator!" ../doc

    SET ExitCode=!ERRORLEVEL!

    IF NOT !ExitCode! EQU 0 (
        CD !OrigDir!

        EXIT /B !ExitCode!
    )

    FOR /F "TOKENS=1,* DELIMS= " %%X IN ("%*") DO (
        SET Args=%%Y
    )

    IF DEFINED NinjaFound (
        ninja !Args!

        SET ExitCode=!ERRORLEVEL!
    ) ELSE (
        jom !Args!

        SET ExitCode=!ERRORLEVEL!
    )

    CD !OrigDir!

    EXIT /B !ExitCode!
) ELSE (
    ECHO OpenCOR's build directory is missing.

    EXIT /B 1
)
