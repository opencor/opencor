@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

FOR %%X IN (ninja.exe) DO (SET NinjaFound=%%~$PATH:X)

IF DEFINED NinjaFound (
    SET Generator=Ninja
) ELSE (
    SET Generator=JOM
)

TITLE Making OpenCOR (using !Generator!)...

IF NOT DEFINED SetupMSVC2010Environment (
    IF EXIST "C:\Program Files (x86)\" (
        SET ProgFilesDir=C:\Program Files ^(x86^)
    ) ELSE (
        SET ProgFilesDir=C:\Program Files
    )

    CALL "!ProgFilesDir!\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

    SET SetupMSVC2010Environment=Done
)

CD build

IF DEFINED NinjaFound (
    SET CMakeGenerator=Ninja
) ELSE (
    SET CMakeGenerator=NMake Makefiles
)

cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=Release ..

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
