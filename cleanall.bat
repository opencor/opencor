@ECHO OFF

TITLE Cleaning the OpenCOR environment...

FOR /D %%I IN (build\*.* winConsole\build\*.*) DO RMDIR /S /Q %%I
FOR    %%I IN (build\*.* winConsole\build\*.*) DO DEL /Q %%I
