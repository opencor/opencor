@ECHO OFF

IF "%1" == "all" (
    TITLE Cleaning all of OpenCOR...
) ELSE (
    TITLE Cleaning OpenCOR...
)

SET HomeDir=%~dp0..\
SET OrigDir=%CD%

IF EXIST "%HomeDir%build" (
    CD "%HomeDir%build"

    FOR    %%I IN (*.*) DO ATTRIB -R "%%I"
    FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
    FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
)

IF "%1" == "all" (
    IF EXIST "%HomeDir%ext" (
        CD "%HomeDir%ext"

        FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
        FOR    %%I IN (*.*) DO IF NOT "%%I" == ".gitignore" DEL /Q "%%I"
    )
)

CD "%OrigDir%"
