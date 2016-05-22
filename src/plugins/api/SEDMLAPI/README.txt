We build our own copy of libSEDML, which requires the following:
 - libSEDML's source code (see https://github.com/fbergmann/libSEDML/).

From there, using cmake-gui on Windows or ccmake on Linux / OS X, we want to:
 - Customise libSEDML:
    ===> LIBSBML_INCLUDE_DIR=<libSBMLIncludeDirectory>
    ===> LIBSBML_LIBRARY=<libSBMLLibraryFile>
    ===> LIBZ_INCLUDE_DIR=<libSBMLDependenciesIncludeDirectory> (if needed)
    ===> WITH_CPP_NAMESPACE=ON
    ===> WITH_SWIG=OFF
    ===> WITH_ZLIB=ON
 - Ask for a 'Release' build (or a 'Debug' build on Windows since both 'Release' and 'Debug' binaries are needed on that platform):
    ===> CMAKE_BUILD_TYPE=Release
 - Ask for C++11 compilation to be used on Linux / OS X (on Windows, MSVC uses C++11 by default) (this is part of the advanced settings):
    - Linux:
       ===> CMAKE_CXX_FLAGS=-std=c++11
    - OS X:
       ===> CMAKE_CXX_FLAGS=-std=c++11 -stdlib=libc++
 - Update the installation destination so that, upon 'installation', we have a ready to use version of the libSEDML binaries (this is particularly useful on OS X since the 'installation' will result in 'clean' binaries):
    ===> CMAKE_INSTALL_PREFIX=<InstallationDestination>

Once we have built and 'installed' libSEDML, we can replace the OpenCOR version of the include and binary files, which are located in the 'installed' include and lib folders, respectively.

Note: on Linux and OS X, we want to strip the binary file off its non-global symbols (using strip -x)...
