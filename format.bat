@ECHO OFF

SET CurrentPath=%CD%

IF EXIST %CurrentPath%\src (
    TITLE Formatting OpenCOR...

    FOR %%X IN (clang-format.exe) DO (
        SET ClangFormatFound=%%~$PATH:X
    )

    IF DEFINED ClangFormatFound (
        CD %CurrentPath%\src

        FOR /R %%I IN (*.cpp *.h) DO (
            ECHO.%%I | FINDSTR /V "3rdparty \ext\ \qzip">NUL && (
                ECHO Formatting %%I...

                clang-format -i -style=file %%I
            )
        )

        CD %CurrentPath%
    ) ELSE (
        ECHO Clang-Format is not installed.
    )
) ELSE (
    ECHO OpenCOR's source directory is missing.
)
