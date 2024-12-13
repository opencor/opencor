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

        "C:\Program Files (x86)\Windows Kits\10\App Certification Kit\signtool.exe" sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /fd SHA256 /td SHA256 /f !EvCertificate! !AppDir!build\bin\OpenCOR.exe

        SET ExitCode=!ERRORLEVEL!
    )

    SET ExitCode=!ERRORLEVEL!

    IF !ExitCode! EQU 0 (
        TITLE Packaging OpenCOR...

        cpack

        SET ExitCode=!ERRORLEVEL!
    )

    CD !OrigDir!
)

EXIT /B !ExitCode!
