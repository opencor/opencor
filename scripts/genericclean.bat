@ECHO OFF

IF "%1" == "all" (
    TITLE Cleaning all of OpenCOR...
) ELSE (
    TITLE Cleaning OpenCOR...
)

SET CrtDir=%~dp0
SET OrigDir=%CD%

IF EXIST "%CrtDir..\%build" (
    CD "%CrtDir..\%build"

    FOR    %%I IN (*.*) DO ATTRIB -R "%%I"
    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
)

IF "%1" == "all" (
    IF EXIST "%CrtDir..\%ext" (
        CD "%CrtDir..\%ext"

        FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
        FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
    )
)

CD "%OrigDir%"
