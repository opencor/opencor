@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

IF "%1" == "all" (
    TITLE Cleaning all of OpenCOR...
) ELSE (
    TITLE Cleaning OpenCOR...
)

SET AppDir=%~dp0..\
SET OrigDir=!CD!

IF EXIST !AppDir!build (
    CD !AppDir!build

    FOR    %%I IN (*.*) DO ATTRIB -R "%%I"
    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
)

IF "%1" == "all" (
    IF EXIST !AppDir!ext (
        CD !AppDir!ext

        FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
        FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
    )
)

CD !OrigDir!
