@ECHO OFF

IF "%1" == "all" GOTO ALL1

TITLE Cleaning OpenCOR...

GOTO NEXT

:ALL1

TITLE Cleaning all of OpenCOR...

:NEXT

SET CurrentPath=%CD%

CD %CurrentPath%\build

FOR    %%I IN (*.*) DO ATTRIB -R "%%I"
FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (*.*) DO DEL /Q "%%I"

IF "%1" == "all" GOTO ALL2

GOTO END

:ALL2

CD %CurrentPath%\ext

FOR /D %%I IN (*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (*.*) DO DEL /Q "%%I"

:END

CD %CurrentPath%
