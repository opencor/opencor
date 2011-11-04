The binaries were built using CMake on the source code of LLVM:
    cd <LLVM-ROOT>
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_INCLUDE_TOOLS=OFF -DLLVM_INCLUDE_TESTS=OFF ..
    make
    cpack -G ZIP

Note: on Windows, you must also pass -G "MinGW Makefiles" to cmake and use MinSizeRel as the build type rather than Release. Otheriwse, you must call mingw32-make rather than make itself.
