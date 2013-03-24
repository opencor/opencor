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

ECHO.
ECHO ---------------------------------
ECHO Making the GUI version of OpenCOR
ECHO ---------------------------------
ECHO.

CD build

cmake -G "Visual Studio 10" ..
msbuild OpenCOR.sln /p:Configuration=Release;OutDir=.\ %*

ECHO.
ECHO -------------------------------------
ECHO Making the console version of OpenCOR
ECHO -------------------------------------
ECHO.

CD ..\windows\build

cmake -G "Visual Studio 10" ..
msbuild OpenCOR.sln /p:Configuration=Release;OutDir=.\ %*

CD ..\..
