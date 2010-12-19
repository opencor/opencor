@TITLE Cleaning up the OpenCOR environment...

@CD build

@mingw32-make -j clean 1> NUL 2> NUL

@DEL /F *Make* 1> NUL 2> NUL
@RMDIR /S /Q CMakeFiles Src 1> NUL 2> NUL

@CD ..
