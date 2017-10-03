SETLOCAL ENABLEDELAYEDEXPANSION

SET CMakeBuildType=%1

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

FOR /F "TOKENS=1,4" %%X IN ('qmake --version') DO (
    IF "%%X" == "Using" (
        SET FullQtVersion=%%Y
        SET QtVersion=!FullQtVersion:~0,3!
    )
)

CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"

CD build

IF DEFINED NinjaFound (
    SET CMakeGenerator=Ninja
) ELSE (
    SET CMakeGenerator=NMake Makefiles JOM
)

cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=!CMakeBuildType! -DENABLE_TESTS=!EnableTests! ..

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    IF DEFINED NinjaFound (
        ninja %2 %3 %4 %5 %6 %7 %8 %9

        SET ExitCode=!ERRORLEVEL!
    ) ELSE (
        jom %2 %3 %4 %5 %6 %7 %8 %9

        SET ExitCode=!ERRORLEVEL!
    )
)

CD ..

EXIT /B !ExitCode!
