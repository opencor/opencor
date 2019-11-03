@ECHO OFF

TITLE Running OpenCOR's tests...

SET AppTestsExe=%~dp0build\bin\runtests.exe

IF NOT EXIST %AppTestsExe% (
    ECHO OpenCOR's tests must first be built before being run.
) ELSE (
    "%AppTestsExe%"
)
