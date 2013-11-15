We want and need to build OpenCOR using MSVC on Windows (need, because we use the binary version of the CellML API and only the MSVC version of those binaries works for us on Windows), as well as have a shared library version of LLVM (incl. Clang) so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build LLVM as a shared library using MSVC (because of the need to expose classes/methods as __declspec(dllexport) or __declspec(dllimport) depending on whether LLVM is being built or referenced, respectively).

To address this shortcoming, we manually expose the classes/methods that are currently needed by OpenCOR (grep for ---OPENCOR--- to find out which ones those are). Should OpenCOR ever need direct access to another class/method, then that class/method will obviously have to be exposed, and LLVM rebuilt.

In addition to exposing some classes/methods, we also had to 'fix' a few things. Again, those are highlighted (just look for ---OPENCOR---).

To build LLVM (incl. Clang) is time consuming and therefore not something that we want to do every time. We therefore have two CMakeLists.txt files:
 - CMakeLists.txt[.use]: this is our default CMakeLists.txt file. It relies on the use of a pre-built version of the LLVM binary to speed up the build process. The LLVM binary is automatically downloaded off the OpenCOR website, and updated whenever necessary.
 - CMakeLists.txt.build: this CMakeLists.txt file should be used (after having been renamed to CMakeLists.txt) whenever LLVM needs to be rebuilt for some reason or another (e.g. you want/need to build OpenCOR on an unsupported platform).

Note #1: configuration files were generated using CMake (on Windows, we also need Python 2.7.x; http://www.python.org/download/):
    1) Uncompress llvm-3.3.src.tar.gz and cfe-3.3.src.tar.gz
    2) Move the contents of [CLANG] to [LLVM]/tools/clang
    3) From the command line:
           cd [LLVM]
           mkdir build
           cd build
           cmake ..   OR   cmake -G "NMake Makefiles" ..
           make       OR   jom
       From there, the configuration files were manually copied over and can be found in the windows, linux and osx folders of various folders.
Note #2: OpencOR is currently supported on Windows, Linux and OS X, so we only need to target X86 at this stage.
