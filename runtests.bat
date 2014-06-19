@ECHO OFF

TITLE Running OpenCOR's tests...

SET NeedInformation=

IF NOT EXIST build\tests\runtests.exe SET NeedInformation=Yes

IF DEFINED NeedInformation (
    ECHO OpenCOR's tests must first be built before being run.
) ELSE (
    build\tests\runtests.exe
)
