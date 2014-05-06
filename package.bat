@ECHO OFF

CALL clean
CALL make

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% NEQ 0 GOTO End

TITLE Packaging OpenCOR...

CD build

cpack -C CPackConfig.cmake

SET ExitCode=%ERRORLEVEL%

CD ..

:End

EXIT /B %ExitCode%
