We build our own copy of libSBML. Using cmake-gui on Windows or ccmake on Linux / OS X, you want to:
 - Customise libSBML:
    ===> WITH_CPP_NAMESPACE=ON
    ===> WITH_SWIG=OFF
 - Ask for a 'Release' build (or a 'Debug' build on Windows since both 'Release' and 'Debug' binaries are needed on that platform):
    ===> CMAKE_BUILD_TYPE=Release
 - Specify the deployment target (OS X only):
    ===> MACOSX_DEPLOYMENT_TARGET=10.7

Once you have built and 'installed' libSBML, you can replace the OpenCOR version of the include and binary files, which are located in the 'installed' include and lib folders, respectively.
