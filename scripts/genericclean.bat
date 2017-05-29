@ECHO OFF

IF "%1" == "all" GOTO ALL1

:ALL1

SET All=" all"

GOTO NEXT

:NEXT

TITLE Cleaning%All% OpenCOR...

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
