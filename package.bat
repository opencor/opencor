@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

SET AppDir=%~dp0

CALL !AppDir!clean
CALL !AppDir!make %*

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! EQU 0 (
    SET EvCertificate=!AppDir!EVCertificate.pfx
    SET OrigDir=!CD!

    CD !AppDir!build

    IF EXIST !EvCertificate! (
        TITLE Code signing OpenCOR...

        set VSCMD_ARG_HOST_ARCH=x64
        set VSCMD_ARG_TGT_ARCH=x64

        IF EXIST "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\vsdevcmd\core\winsdk.bat" (
            CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\vsdevcmd\core\winsdk.bat"
        ) ELSE (
            CALL "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\vsdevcmd\core\winsdk.bat"
        )

        "!WindowsSdkVerBinPath!\x64\signtool.exe" sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /fd SHA256 /td SHA256 /f !EvCertificate! !AppDir!build\OpenCOR.com
        "!WindowsSdkVerBinPath!\x64\signtool.exe" sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /fd SHA256 /td SHA256 /f !EvCertificate! !AppDir!build\OpenCOR.exe

        "!WindowsSdkVerBinPath!\x64\signtool.exe" sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /fd SHA256 /td SHA256 /f !EvCertificate! !AppDir!build\Python\bin\python.exe

        SET ExitCode=!ERRORLEVEL!
    ) ELSE (
        SET ExitCode=0
    )

    IF !ExitCode! EQU 0 (
        TITLE Packaging OpenCOR...

        cpack

        SET ExitCode=!ERRORLEVEL!
    )

    IF !ExitCode! EQU 0 AND EXIST !EvCertificate! (
        TITLE Code signing OpenCOR's installer...

        "!WindowsSdkVerBinPath!\x64\signtool.exe" sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /fd SHA256 /td SHA256 /f !EvCertificate! !AppDir!build\OpenCOR-*.exe

        SET ExitCode=!ERRORLEVEL!
    )

    CD !OrigDir!
)

EXIT /B !ExitCode!
