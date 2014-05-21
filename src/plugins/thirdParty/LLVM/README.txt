We want to build OpenCOR using MSVC on Windows, as well as have a shared library version of LLVM (incl. Clang) so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build LLVM as a shared library using MSVC (because of the need to expose classes/methods as __declspec(dllexport) or __declspec(dllimport) depending on whether LLVM is being built or referenced, respectively).

To address this shortcoming, we manually expose the classes/methods that are currently needed by OpenCOR (grep for ---OPENCOR--- to find out which ones those are). Should OpenCOR ever need direct access to another class/method, then that class/method will obviously have to be exposed, and LLVM rebuilt.

In addition to exposing some classes/methods, we also had to 'fix' a few things. Again, those are highlighted (just look for ---OPENCOR---).

In case LLVM is to be built (by setting USE_PREBUILT_LLVM_PLUGIN to OFF in [OpenCOR]/cmake/common.cmake), then keep in mind the following:
 - Configuration files are to be generated using CMake (on Windows, we also need Python 2.7.x; http://www.python.org/download/):
    1) Download the LLVM and Clang source codes from http://llvm.org/releases/download.html
    2) Uncompress their corresponding tar.gz file
    3) Move the contents of [Clang] to [LLVM]/tools/clang
    4) From the command line:
           cd [LLVM]
           mkdir build
           cd build
           cmake ..   OR   cmake -G "NMake Makefiles" ..
           make       OR   jom
       From there, the configuration files are to be manually copied over to the windows, linux and osx folders of the various folders.
 - Neither HAVE_TERMINFO nor HAVE_ZLIB_H must be set, while LLVM_ENABLE_ZLIB must be set to 0 in [LLVM]/include/llvm/Config/config.h on all three platforms. To do anything else will require additional features that may or not be present on the build machine.
 - OpencOR is currently supported on Windows, Linux and OS X, so we only need to target X86 at this stage.
 - on OS X, a normal build of LLVM (incl. Clang) will produce a 'dirty' binary (it is so that it can work without any problem with XCode). Yet, we need a 'clean' binary, so LLVM (incl. Clang) must be built as if we wanted to package OpenCOR.
