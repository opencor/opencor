@ECHO OFF

TITLE Running OpenCOR's tests...

SET appTestsExe=%~dp0build\bin\runtests.exe

IF NOT EXIST %appTestsExe% (
    ECHO OpenCOR's tests must first be built before being run.
) ELSE (
    "%appTestsExe%"
)
