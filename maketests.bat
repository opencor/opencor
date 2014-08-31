@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

FOR %%X IN (ninja.exe) DO (
    SET NinjaFound=%%~$PATH:X
)

IF DEFINED NinjaFound (
    SET Generator=Ninja
) ELSE (
    SET Generator=JOM
)

TITLE Making OpenCOR and its tests (using !Generator!)...

IF NOT DEFINED SetupMSVC2013Environment (
    IF EXIST "C:\Program Files (x86)\" (
        SET ProgFilesDir=C:\Program Files ^(x86^)
    ) ELSE (
        SET ProgFilesDir=C:\Program Files
    )

    IF EXIST "C:\Program Files (x86)\" (
        CALL "!ProgFilesDir!\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
    ) ELSE (
        CALL "!ProgFilesDir!\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
    )

    SET SetupMSVC2013Environment=Done
)

CD build

IF DEFINED NinjaFound (
    SET CMakeGenerator=Ninja
) ELSE (
    SET CMakeGenerator=NMake Makefiles JOM
)

cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON ..

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    IF DEFINED NinjaFound (
        ninja

        SET ExitCode=!ERRORLEVEL!
    ) ELSE (
        jom

        SET ExitCode=!ERRORLEVEL!
    )
)

CD ..

EXIT /B !ExitCode!
