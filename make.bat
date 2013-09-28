@ECHO OFF

TITLE Making OpenCOR...

IF NOT "%SetupMSVC2010Environment%" == "" GOTO MakeOpenCORGUI

IF NOT EXIST "C:\Program Files (x86)\" GOTO SetupMSVC2010EnvironmentOn32BitSystem

SET ProgFilesDir=C:\Program Files (x86)

GOTO ContinueSetupMSVC2010Environment

:SetupMSVC2010EnvironmentOn32BitSystem

SET ProgFilesDir=C:\Program Files

:ContinueSetupMSVC2010Environment

CALL "%ProgFilesDir%\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

SET SetupMSVC2010Environment=Done

:MakeOpenCORGUI

ECHO.
ECHO ---------------------------------
ECHO Making the GUI version of OpenCOR
ECHO ---------------------------------
ECHO.

CD build

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO BuildOpenCORGUI

CD ..

EXIT /B %ExitCode%

:BuildOpenCORGUI

jom %*

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO MakeOpenCORCLI

CD ..

EXIT /B %ExitCode%

:MakeOpenCORCLI

ECHO.
ECHO ---------------------------------
ECHO Making the CLI version of OpenCOR
ECHO ---------------------------------
ECHO.

CD ..\windows\build

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..

SET ExitCode=%ERRORLEVEL%

IF %ExitCode% EQU 0 GOTO BuildOpenCORCLI

CD ..\..

EXIT /B %ExitCode%

:BuildOpenCORCLI

jom %*

SET ExitCode=%ERRORLEVEL%

CD ..\..

EXIT /B %ExitCode%
