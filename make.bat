@ECHO OFF

TITLE Making OpenCOR...

IF NOT "%SetupMSVC2010Environment%" == "" GOTO MakeOpenCOR

IF NOT EXIST "C:\Program Files (x86)\" GOTO SetupMSVC2010EnvironmentOn32BitSystem

SET ProgFilesDir=C:\Program Files (x86)

GOTO ContinueSetupMSVC2010Environment

:SetupMSVC2010EnvironmentOn32BitSystem

SET ProgFilesDir=C:\Program Files

:ContinueSetupMSVC2010Environment

CALL "%ProgFilesDir%\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

SET SetupMSVC2010Environment=Done

:MakeOpenCOR

CD build

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO BuildOpenCOR

CD ..

EXIT /B %ExitCode%

:BuildOpenCOR

jom

SET ExitCode=%ERRORLEVEL%

CD ..

EXIT /B %ExitCode%
