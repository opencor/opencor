For various reasons (e.g. to be able to use C++11), we need to use the CellML API's latest snapshot. However, not all binaries always get built for all the platforms we want to support. So, in the end, we build the CellML API ourselves, which can be quite useful (e.g. on OS X, it makes the binaries much smaller since we don't need universal binaries).

Some prerequisites before we can actually build the CellML API ourselves:
 - Doxygen (see http://www.doxygen.org/);
 - omniidl (see http://omniorb.sourceforge.net/; working Windows binaries can be found in version 4.1.5); and
 - the CellML API source code (see https://github.com/cellmlapi/cellml-api/).

On OS X, you will also need the Mac OS X 10.7 SDK or later (earlier SDKs don't support C++11).

From there, using cmake-gui on Windows or ccmake on Linux / OS X, you need to:
 - Disable testing:
    ===> BUILD_TESTING=OFF
 - Ask for a release build (or a debug build on Windows since both release and debug binaries are needed on Windows):
    ===> CMAKE_BUILD_TYPE=Release
 - Ask for C++11 compilation to be used on Linux / OS X (on Windows, MSVC uses C++11 by default) (this is part of the advanced settings):
    - Linux:
       ===> CMAKE_CXX_FLAGS=-std=c++0x
    - OS X:
       ===> CMAKE_CXX_FLAGS=-std=c++0x -stdlib=libc++
 - Update the installation destination so that, upon 'installation', we have a ready to use version of the CellML API binaries (this is particularly useful on OS X since the 'installation' will result in 'clean' binaries):
    ===> CMAKE_INSTALL_PREFIX=<InstallationDestination>
 - Enable the AnnoTools, CCGS, CeVAS, CUSES, MaLaES, RDF and VACSS services:
    ===> ENABLE_ANNOTOOLS=ON
    ===> ENABLE_CCGS=ON
    ===> ENABLE_CEVAS=ON
    ===> ENABLE_CUSES=ON
    ===> ENABLE_MALAES=ON
    ===> ENABLE_RDF=ON
    ===> ENABLE_VACSS=ON

Once you have built and 'installed' the CellML API, you can replace the OpenCOR version of the include and binary files, which are located in the 'installed' include and lib folders, respectively. Regarding the include files, there are a few things that need to be done:
 - the cda_config.h, IfaceCellML_events.hxx and IfaceDOM_events.hxx files are to be discarded;
 - the cda_compiler_support.h file shouldn't include the cda_config.h file; and
 - the cellml-api-cxx-support.hpp file shouldn't include the windows.h file and it shouldn't define the CDAMutex, CDALock and CDA_RefCount classes, and the CDA_IMPL_REFCOUNT macro (all of these will otherwise cause various problems with LLVM on Windows).
