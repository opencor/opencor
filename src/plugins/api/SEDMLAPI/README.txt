We build our own copy of libSEDML, which requires the following:
 - libSEDML's source code (see https://github.com/fbergmann/libSEDML/).

From there, using cmake-gui on Windows or ccmake on Linux / OS X, we want to:
 - Customise libSEDML:
    ===> LIBSBML_INCLUDE_DIR=<libSBMLIncludeDirectory>
    ===> LIBSBML_LIBRARY=<libSBMLLibraryFile>
    ===> WITH_CPP_NAMESPACE=ON
    ===> WITH_SWIG=OFF
    ===> WITH_ZLIB=ON
 - Ask for a 'Release' build (or a 'Debug' build on Windows since both 'Release' and 'Debug' binaries are needed on that platform):
    ===> CMAKE_BUILD_TYPE=Release
 - Update the installation destination so that, upon 'installation', we have a ready to use version of the libSBML binaries (this is particularly useful on OS X since the 'installation' will result in 'clean' binaries):
    ===> CMAKE_INSTALL_PREFIX=<InstallationDestination>
 - Specify the deployment target (OS X only):
    ===> MACOSX_DEPLOYMENT_TARGET=10.7

Once we have built and 'installed' libSBML, we can replace the OpenCOR version of the include and binary files, which are located in the 'installed' include and lib folders, respectively.
