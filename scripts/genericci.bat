@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0..\

IF EXIST !AppDir!build (
    IF "%1" == "Release" (
        SET Version=release
    ) ELSE IF "%1" == "Debug" (
        SET Version=debug
    ) ELSE (
        ECHO Only the Release and Debug options are supported.

        EXIT /B 1
    )

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

    SET OrigDir=!CD!

    CALL !AppDir!clean

    TITLE Building and testing the !version! version of OpenCOR using !Generator!...

    IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat" (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) ELSE (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    )

    CD !AppDir!build

    cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=%1 -DENABLE_SAMPLE_PLUGINS=ON -DENABLE_TEST_PLUGINS=ON -DENABLE_TESTS=ON ..

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

    IF NOT !ExitCode! EQU 0 (
        EXIT /B !ExitCode!
    )

    !AppDir!build\bin\runtests.exe

    EXIT /B !ERRORLEVEL!
) ELSE (
    ECHO OpenCOR's build directory is missing.

    EXIT /B 1
)
