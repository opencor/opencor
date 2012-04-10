We want and need to build OpenCOR using MSVC on Windows, as well as have a shared library version of LLVM so that it can be used as a 'proper' plugin. The problem with the latter is that it's not possible to build LLVM as a shared library using MSVC (because of the need to specify qualify classes as __declspec(dllexport) or __declspec(dllimport) depending on whether LLVM is being built or referenced, respectively.

To address this shortcoming, we maunally qualify the classes that are currently needed by OpenCOR (grep for ---OPENCOR--- to find out which ones those are). Should OpenCOR ever need direct access to another class, then that class will obviously have to be qualified and LLVM rebuilt.

To build LLVM is time consuming and therefore not something we want to do everytime we build OpenCOR. We therefore have two CMakeLists.txt files:
 - CMakeLists.txt: this is our 'proper' CMakeLists.txt file and the one which is to be used by default and once a binary version of the LLVM plugin exists (such a binary is located in either the windows, linux or macosx folder under [LLVM]/bin).
 - CMakeLists.txt.build: this CMakeLists.txt file should be used whenever LLVM needs to be rebuilt. Once this is done, use of the 'proper' CMakeLists.txt should be resumed.

Note: LLVM's config files were generated using CMake:
          cd <LLVM-SOURCE_CODE>
          mkdir build
          cd build
          cmake ..
      From there, the config files were manually copied over. They can be found in the windows, linux and macosx folders under [LLVM]/include/llvm/Config and [LLVM]/include/llvm/Support.
