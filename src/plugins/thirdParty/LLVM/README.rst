We need to build `LLVM <http://www.llvm.org/>`__ (incl. `Clang <http://clang.llvm.org/>`__) ourselves so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build `LLVM <http://www.llvm.org/>`__ as a shared library using MSVC (because of the need to expose classes/methods as ``__declspec(dllexport)`` or ``__declspec(dllimport)`` depending on whether `LLVM <http://www.llvm.org/>`__ is being built or referenced, respectively).

In case `LLVM <http://www.llvm.org/>`__ is to be built (by setting the ``USE_PREBUILT_LLVM_PLUGIN`` option to ``OFF``), then keep in mind that configuration files have to be up to date. So, if needed, they may have to be regenerated using `CMake <https://www.cmake.org/>`__ (on **Windows**, this may require installing `Python 2.7.x <http://www.python.org/download/>`__):

- Download the `LLVM <http://www.llvm.org/>`__ and `Clang <http://clang.llvm.org/>`__ source codes from http://llvm.org/releases/download.html;
- Extract their corresponding ``.tar.xz`` file;
- Move the contents of ``[Clang]`` to ``[LLVM]/tools/clang``;
- From the command line:
  ::
    cd [LLVM]
    mkdir build
    cd build
