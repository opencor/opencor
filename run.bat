@ECHO OFF

TITLE Running OpenCOR...

IF NOT EXIST build\bin\OpenCOR.com SET NeedInformation=Yes
IF NOT EXIST build\bin\OpenCOR.exe SET NeedInformation=Yes

IF DEFINED NeedInformation (
    ECHO OpenCOR must first be built before being run.
) ELSE (
    build\bin\OpenCOR %*
)
