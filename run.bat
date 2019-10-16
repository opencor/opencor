@ECHO OFF

TITLE Running OpenCOR...

SET AppBase=%~dp0build\bin\OpenCOR
SET NeedInformation=

IF NOT EXIST %AppBase%.com SET NeedInformation=Yes
IF NOT EXIST %AppBase%.exe SET NeedInformation=Yes

IF DEFINED NeedInformation (
    ECHO OpenCOR must first be built before being run.
) ELSE (
    %AppBase% %*
)
