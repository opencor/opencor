SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0..\

IF EXIST %AppDir%build (
    IF "%1" == "Release" (
        SET CMakeBuildType=Release
        SET EnableSamplePlugins=OFF
        SET EnableTestPlugins=OFF
        SET EnableTests=OFF
    ) ELSE IF "%1" == "Debug" (
        SET CMakeBuildType=Debug
        SET EnableSamplePlugins=OFF
        SET EnableTestPlugins=OFF
        SET EnableTests=ON
    ) ELSE IF "%1" == "CIRelease" (
        SET CMakeBuildType=Release
        SET EnableSamplePlugins=ON
        SET EnableTestPlugins=ON
        SET EnableTests=ON
    ) ELSE IF "%1" == "CIDebug" (
        SET CMakeBuildType=Debug
        SET EnableSamplePlugins=ON
        SET EnableTestPlugins=ON
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

    IF "%1" == "Release" (
        SET TitleTests=
    ) ELSE (
        SET TitleTests= and its tests
    )

    TITLE Making OpenCOR!TitleTests! using !Generator!...

    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

    SET OrigDir=%CD%

    CD %AppDir%build

    IF DEFINED NinjaFound (
        SET CMakeGenerator=Ninja
    ) ELSE (
        SET CMakeGenerator=NMake Makefiles JOM
    )

    cmake -G "!CMakeGenerator!" -DCMAKE_BUILD_TYPE=!CMakeBuildType! -DENABLE_SAMPLE_PLUGINS=!EnableSamplePlugins! -DENABLE_TEST_PLUGINS=!EnableTestPlugins! -DENABLE_TESTS=!EnableTests! ..

    SET ExitCode=!ERRORLEVEL!

    IF !ExitCode! EQU 0 (
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
    )

    CD "%OrigDir%"

    EXIT /B !ExitCode!
) ELSE (
    ECHO OpenCOR's build directory is missing.

    EXIT /B 1
)
