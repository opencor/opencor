For various reasons (e.g. to be able to use C++11), we need to use the CellML API's latest snapshot. However, binaries don't always get built for a given snapshot. So, in the end, we build the CellML API ourselves and this on the different platforms supported by OpenCOR. To do so, we first need to get the CellML API's source code from https://github.com/cellmlapi/cellml-api.

To build the CellML API, you will need to:
 - Disable testing:
    ===> BUILD_TESTING=OFF
 - Ask for a release build:
    ===> CMAKE_BUILD_TYPE=Release
 - Update the installation destination so that upon 'installation', we have a ready to use version of the CellML API binaries (this is particularly important on OS X since the 'installation' will result in 'clean' binaries):
    ===> CMAKE_INSTALL_PREFIX=<InstallationDestination>
 - Enable the AnnoTools, CCGS, CeVAS, CUSES, MaLaES, RDF and VACSS services:
    ===> ENABLE_ANNOTOOLS=ON
    ===> ENABLE_CCGS=ON
    ===> ENABLE_CEVAS=ON
    ===> ENABLE_CUSES=ON
    ===> ENABLE_MALAES=ON
    ===> ENABLE_RDF=ON
    ===> ENABLE_VACSS=ON
 - Ask for C++11 compilation:
    - OS X:
       ===> CMAKE_CXX_FLAGS=-std=c++0x -stdlib=libc++

Once you have built and 'installed' the CellML API, you can replace the OpenCOR version of the include and binary files, which are located in the 'installed' include and lib folders, respectively (the 'installed' share folder is of no interest to us here). However, regarding the include files, there are a couple of things to keep in mind:
 - The cda_config.h, IfaceCellML_events.hxx and IfaceDOM_events.hxx files are to be discarded; and
 - The cda_compiler_support.h file shouldn't include the cda_config.h file anymore.

Note that, on OS X, you need the Mac OS X 10.7 SDK or later so that the CellML API can be built using C++11.
