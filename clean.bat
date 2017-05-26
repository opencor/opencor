@ECHO OFF

TITLE Cleaning OpenCOR...

SET CurrentPath=%CD%

FOR %%I IN (build ext) DO (
    CD %CurrentPath%\%%I

    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO DEL /Q "%%I"
)

FOR %%I IN (distrib src) DO (
    CD %CurrentPath%\%%I

    FOR /D /R %%J IN (*) DO (
        IF /I "%%~NXJ" == "ext" (
            RMDIR /S /Q %%J
        )
    )
)

CD %CurrentPath%
