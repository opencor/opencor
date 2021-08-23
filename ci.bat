@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

CALL %~dp0scripts\genericci Release %*

IF !ERRORLEVEL! EQU 0 (
    CALL %~dp0scripts\genericci Debug %*
)
