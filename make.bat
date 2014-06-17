@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

TITLE Making OpenCOR...

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

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    jom

    SET ExitCode=!ERRORLEVEL!
)

CD ..

EXIT /B !ExitCode!
