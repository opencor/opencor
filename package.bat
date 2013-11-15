@ECHO OFF

CALL clean
CALL make

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO PackageOpenCOR

EXIT /B %ExitCode%

:PackageOpenCOR

TITLE Packaging OpenCOR...

CD build

cpack -C CPackConfig.cmake

SET ExitCode=%ERRORLEVEL%

CD ..

EXIT /B %ExitCode%
