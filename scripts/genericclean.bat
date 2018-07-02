@ECHO OFF

IF "%1" == "all" GOTO ALL1

TITLE Cleaning OpenCOR...

GOTO NEXT

:ALL1

TITLE Cleaning all of OpenCOR...

:NEXT

SET CurrentPath=%CD%

CD %CurrentPath%\build

FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (*.*) DO DEL /Q "%%I"

IF "%1" == "all" GOTO ALL2

GOTO END

:ALL2

CD %CurrentPath%\ext

FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (*.*) DO DEL /Q "%%I"

FOR %%I IN (distrib src) DO (
    CD %CurrentPath%\%%I

    FOR /D /R %%J IN (*) DO (
        IF /I "%%~NXJ" == "ext" (
            RMDIR /S /Q %%J
        )
    )
)

:END

CD %CurrentPath%
