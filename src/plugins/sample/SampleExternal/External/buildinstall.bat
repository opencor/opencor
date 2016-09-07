@echo off
cl -c -Iinclude -DEXTERNAL_LIBRARY=1 src\external.cpp
link /dll /implib:external.lib /out:external.dll external.obj
echo Installing into %1
mkdir %1
mkdir %1\include
mkdir %1\lib
xcopy /y /s include %1\include
xcopy /y external.dll %1\lib
xcopy /y external.lib %1\lib
