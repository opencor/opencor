@ECHO OFF

TITLE Cleaning OpenCOR...

FOR /D %%I IN (build\*.*) DO RMDIR /S /Q "%%I"
FOR    %%I IN (build\*.*) DO DEL /Q "%%I"
