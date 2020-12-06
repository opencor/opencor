@ECHO OFF

CALL %~dp0clean
CALL %~dp0scripts\genericmake CIRelease

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! NEQ 0 (
    EXIT /B !ExitCode!
)

CALL %~dp0runtests

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! NEQ 0 (
    EXIT /B !ExitCode!
)

CALL %~dp0clean
CALL %~dp0scripts\genericmake CIDebug

SET ExitCode=!ERRORLEVEL!

IF !ExitCode! NEQ 0 (
    EXIT /B !ExitCode!
)

CALL %~dp0runtests
