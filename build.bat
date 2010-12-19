@TITLE Building OpenCOR...

@CD build

@cmake -G "MinGW Makefiles" .. 1> NUL 2> NUL
@mingw32-make -j 1> NUL 2> NUL

@CD ..
