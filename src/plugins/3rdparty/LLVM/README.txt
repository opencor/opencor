We want and need to build OpenCOR using MSVC on Windows (need, because we use the binary version of the CellML API and only the MSVC version of those binaries works for us on Windows), as well as have a shared library version of LLVM so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build LLVM as a shared library using MSVC (because of the need to qualify classes as __declspec(dllexport) or __declspec(dllimport) depending on whether LLVM is being built or referenced, respectively).

To address this shortcoming, we maunally qualify the classes that are currently needed by OpenCOR (grep for ---OPENCOR--- to find out which ones those are). Should OpenCOR ever need direct access to another class, then that class will obviously have to be qualified and LLVM rebuilt.

To build LLVM is time consuming and therefore not something we want to do every time. We therefore have two CMakeLists.txt files:
 - CMakeLists.txt[.use]: this is our default CMakeLists.txt file and the one which is to be used once a binary version of the LLVM plugin exists (such a binary is located in either the windows, linux or macosx folder under [LLVM]/bin).
 - CMakeLists.txt.build: this CMakeLists.txt file should be used (after having been renamed to CMakeLists.txt) whenever LLVM needs to be rebuilt. Once this is done, use of CMakeLists.txt[.use] should be resumed.

Note #1: LLVM's config files were generated using CMake (on Windows, we also need Python 2.7.x; http://www.python.org/download/):
             cd <LLVM-SOURCE_CODE>
             mkdir build
             cd build
             cmake ..
             make
         From there, the files were manually copied over. They can be found in the windows, linux and macosx folders of [LLVM]/include/llvm/Config, [LLVM]/include/llvm/Support and [LLVM]/lib/Target/X86.
Note #2: OpencOR 'only' works on Windows, Linux and Mac OS X, so we only need to target X86 at this stage.
