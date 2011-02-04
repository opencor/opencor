@ECHO OFF

TITLE Cleaning the OpenCOR environment...

FOR /D %%I IN (build\CMakeFiles\*.* build\src\*.* winConsole\build\CMakeFiles\*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (build\*.* build\CMakeFiles\*.* build\src\*.* winConsole\build\*.* winConsole\build\CMakeFiles\*.*) DO DEL /Q "%%I"
FOR    %%I IN (build\CMakeFiles build\src winConsole\build\CMakeFiles) DO RMDIR "%%I"
