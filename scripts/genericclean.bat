@ECHO OFF

IF "%1" == "all" (
    TITLE Cleaning all of OpenCOR...
) ELSE (
    TITLE Cleaning OpenCOR...
)

SET CurrentPath=%CD%

IF EXIST %CurrentPath%\build (
    CD %CurrentPath%\build

    FOR    %%I IN (*.*) DO ATTRIB -R "%%I"
    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
)

IF "%1" == "all" (
    SET ExtPath=%CurrentPath%\ext
) ELSE (
    SET ExtPath=%CurrentPath%\ext\doc
)

IF EXIST %ExtPath% (
    CD %ExtPath%

    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
)

CD %CurrentPath%
