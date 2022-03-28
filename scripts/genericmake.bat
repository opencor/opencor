@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0..\

IF EXIST !AppDir!build (
    IF "%1" == "Release" (
        SET CMakeBuildType=Release
        SET EnableTests=OFF
    ) ELSE IF "%1" == "Tests" (
        SET CMakeBuildType=Debug
        SET EnableTests=ON
    ) ELSE (
        ECHO Only the Release and Tests options are supported.

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

    IF "%1" == "Release" (
        SET TitleTests=
    ) ELSE (
        SET TitleTests= and its tests
    )

    TITLE Building OpenCOR!TitleTests! using !Generator!...

    IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) ELSE (
        CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    )

    SET OrigDir=!CD!

    CD !AppDir!build

    cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=!CMakeBuildType! -DENABLE_TESTS=!EnableTests! ..

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
