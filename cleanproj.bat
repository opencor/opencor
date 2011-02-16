@ECHO OFF

TITLE Cleaning the OpenCOR environment...

FOR /D %%I IN (build\_CPack_Packages\*.* build\CMakeFiles\*.* build\src\*.* winConsole\build\CMakeFiles\*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (build\*.* build\_CPack_Packages\*.* build\CMakeFiles\*.* build\src\*.* winConsole\build\*.* winConsole\build\CMakeFiles\*.*) DO DEL /Q "%%I"
FOR    %%I IN (build\_CPack_Packages build\CMakeFiles build\src winConsole\build\CMakeFiles) DO IF EXIST "%%I" ( RMDIR /Q "%%I" )
