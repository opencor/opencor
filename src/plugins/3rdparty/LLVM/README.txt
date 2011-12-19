Ideally, our LLVM plugin would work in a way similar to that of, say, the QJson plugin, i.e. the plugin itself would contain all of LLVM. However, this cannot easily be done, not least because both LLVM is a big library. So, instead, we do something similar to the CellML plugin, i.e. build LLVM as a shared library and deploy it that way while use the plugin itself to get access to LLVM's classes, methods, etc.

Note that to build a shared version of LLVM using CMake is not possible on Windows and is not recommended on other operating systems (see http://llvm.org/docs/CMake.html). We therefore had to rely on LLVM's other and prefered build system: Autotools. There shouldn't be a need for you to build LLVM. Still, should you want to build it on Linux and/or Mac OS X, then you should be fine as long as you are all set to build OpenCOR on those systems (see doc/developer/preRequirements.html). However, on Windows, your Qt environment will not be sufficient. We would therefore recommend building LLVM using a virtual machine (so as not to mess up your Qt environment) which first involves installing the following:
 - MinGW/MSYS (see http://sourceforge.net/projects/mingw/): select C++ Compiler, MSYS Basic System and MinGW Developer ToolKit in the list of components to install;
 - Python (see http://www.python.org/download/; you want version 2.7.2 not 3.x): make sure that Python is accessible from a shell prompt, e.g. export PATH=/c/Python27:$PATH.

From there, you need to get LLVM's source code (see http://www.llvm.org/releases/) and from a shell prompt:
    cd <WHERE-llvm-3.0.tar.gz-IS>
    tar xvzf llvm-3.0.tar.gz
    mkdir build
    cd build
    ../llvm-3.0.src/configure --disable-docs --enable-shared --enable-targets=host
    make
