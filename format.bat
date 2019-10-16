@ECHO OFF

SET AppDir=%~dp0
SET OrigDir=%CD%

IF EXIST %AppDir%src (
    TITLE Formatting OpenCOR...

    FOR %%X IN (clang-format.exe) DO (
        SET ClangFormatFound=%%~$PATH:X
    )

    IF DEFINED ClangFormatFound (
        CD %AppDir%src

        FOR /R %%I IN (*.cpp *.h) DO (
            ECHO.%%I | FINDSTR /V "3rdparty \ext\ \qzip">NUL && (
                ECHO Formatting %%I...

                clang-format -i -style=file %%I
            )
        )

        CD %OrigDir%
    ) ELSE (
        ECHO Clang-Format is not installed.
    )
) ELSE (
    ECHO OpenCOR's source directory is missing.
)
