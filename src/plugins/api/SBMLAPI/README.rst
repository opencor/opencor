We build our own copy of `libSBML <http://sbml.org/Software/libSBML>`__, which requires the following:

- `libSBML's core source code <http://sourceforge.net/projects/sbml/files/libsbml/>`__; and
- libSBML dependencies (**Windows** only; the contents of which is to be located under ``[libSBML]/dependencies``):

  - `Release <http://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/libSBML_dependencies_vs14_release_x64.zip>`__; and
  - `Debug <http://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/libSBML_dependencies_vs14_debug_x64.zip>`__.

From there, using ``cmake-gui`` on **Windows** or ``ccmake`` on **Linux**/**macOS**, we want to:

- Customise `libSBML <http://sbml.org/Software/libSBML>`__:

  ::

    WITH_CPP_NAMESPACE=ON
    WITH_SWIG=OFF

- Ask for a ``Release`` build (or a ``Debug`` build on **Windows** since both ``Release`` and ``Debug`` binaries are needed on that platform):

  ::

    CMAKE_BUILD_TYPE=Release

- Ask for C++11 compilation to be used on **Linux**/**macOS** (on **Windows**, MSVC uses C++11 by default) (this is part of the advanced settings):

  - **Linux:**

    ::

      CMAKE_CXX_FLAGS=-std=c++11

  - **macOS:**

    ::

      CMAKE_CXX_FLAGS=-std=c++11 -stdlib=libc++

- Update the installation destination so that, upon 'installation', we have a ready to use version of the `libSBML <http://sbml.org/Software/libSBML>`__ binaries (this is particularly useful on **macOS** since the 'installation' will result in 'clean' binaries):

  ::

    CMAKE_INSTALL_PREFIX=<InstallationDestination>

Once we have built and 'installed' `libSBML <http://sbml.org/Software/libSBML>`__, we can replace the OpenCOR version of the include and binary files, which are located in the 'installed' ``include`` and ``lib`` folders, respectively.

**Note:** on **Linux** and **macOS**, we want to strip the binary file off its non-global symbols (using ``strip -x``)...
