We build our own copy of
`libSEDML <https://github.com/fbergmann/libSEDML/>`__, which requires
the following: \* `libSEDML's source
code <https://github.com/fbergmann/libSEDML/>`__.

From there, using ``cmake-gui`` on **Windows** or ``ccmake`` on
**Linux**/**macOS**, we want to: \* Customise
`libSEDML <https://github.com/fbergmann/libSEDML/>`__:

``LIBSBML_INCLUDE_DIR=<libSBMLIncludeDirectory>    LIBSBML_LIBRARY=<libSBMLLibraryFile>    LIBZ_INCLUDE_DIR=<libSBMLDependenciesIncludeDirectory>    WITH_CPP_NAMESPACE=ON    WITH_SWIG=OFF    WITH_ZLIB=ON``

-  Ask for a ``Release`` build (or a ``Debug`` build on **Windows**
   since both ``Release`` and ``Debug`` binaries are needed on that
   platform):

``CMAKE_BUILD_TYPE=Release``

-  Ask for C++11 compilation to be used on **Linux**/**macOS** (on
   **Windows**, MSVC uses C++11 by default) (this is part of the
   advanced settings):

   -  **Linux:**

   ``CMAKE_CXX_FLAGS=-std=c++11``

   -  **macOS:**

   ``CMAKE_CXX_FLAGS=-std=c++11 -stdlib=libc++``

-  Update the installation destination so that, upon 'installation', we
   have a ready to use version of the
   `libSEDML <https://github.com/fbergmann/libSEDML/>`__ binaries (this
   is particularly useful on **macOS** since the 'installation' will
   result in 'clean' binaries):

``CMAKE_INSTALL_PREFIX=<InstallationDestination>``

Once we have built and 'installed'
`libSEDML <https://github.com/fbergmann/libSEDML/>`__, we can replace
the OpenCOR version of the include and binary files, which are located
in the 'installed' ``include`` and ``lib`` folders, respectively.

**Note:** on **Linux** and **macOS**, we want to strip the binary file
off its non-global symbols (using ``strip -x``)...
