SETLOCAL ENABLEDELAYEDEXPANSION

SET Bitness=%1
SET CMakeBuildType=%2

IF "!CMakeBuildType!" == "Release" (
    SET EnableTests=OFF
) ELSE (
    SET CMakeBuildType=Debug
    SET EnableTests=ON
)

FOR %%X IN (ninja.exe) DO (
    SET NinjaFound=%%~$PATH:X
)

IF DEFINED NinjaFound (
    SET Generator=Ninja
) ELSE (
    SET Generator=JOM
)

IF "!CMakeBuildType!" == "Release" (
    SET TitleTests=
) ELSE (
    SET TitleTests= and its tests
)

TITLE Making OpenCOR!TitleTests! (using !Generator!)...

IF "!Bitness!" == "32" (
    SET OLDPATH=!PATH!
)

IF EXIST "C:\Program Files (x86)\" (
    SET ProgFilesDir=C:\Program Files ^(x86^)
) ELSE (
    SET ProgFilesDir=C:\Program Files
)

FOR /F "TOKENS=1,4" %%X IN ('qmake --version') DO (
    IF "%%X" == "Using" (
        SET FullQtVersion=%%Y
        SET QtVersion=!FullQtVersion:~0,3!
    )
)

IF "!Bitness!" == "Default" (
    IF EXIST "C:\Qt\!QtVersion!\msvc2013_64\bin\" (
        CALL "!ProgFilesDir!\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
    ) ELSE (
        CALL "!ProgFilesDir!\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
    )
) ELSE (
    SET PATH=C:\Qt\!QtVersion!\msvc2013\bin;!PATH!

    CALL "!ProgFilesDir!\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
)

CD build

IF DEFINED NinjaFound (
    SET CMakeGenerator=Ninja
) ELSE (
    SET CMakeGenerator=NMake Makefiles JOM
)

cmake -G "!CMakeGenerator!" -DSHOW_INFORMATION_MESSAGE=ON -DCMAKE_BUILD_TYPE=!CMakeBuildType! -DENABLE_TESTS=!EnableTests! ..

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

IF "!Bitness!" == "32" (
    SET PATH=!OLDPATH!
)

EXIT /B !ExitCode!
