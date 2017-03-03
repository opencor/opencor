We build our own copy of the `Zinc <http://physiomeproject.org/software/opencmiss/zinc>`__ library, which requires doing the following from the command line:

::

  git clone https://github.com/OpenCMISS/manage.git OpenCMISS/manage

On Windows, we then need to use CMake's GUI client (to ensure that we can build a 64-bit version of Zinc), making sure that `OC_USE_IRON` is set to `NO` and `MPI` to `none`.

From there, we need to remove the following lines from `[OpenCMISS]/manage/build/hdf5-download.vcxproj`:

::

  "C:\Program Files\Git\cmd\git.exe" checkout v1.8.14
  if %errorlevel% neq 0 goto :cmEnd

Indeed, version 1.8.14 of HDF5 is known to break Visual Studio 2015, so we use the default version of HDF5, which is fine.

On Linux and macOS, and after having cloned OpenCMISS manage, you want to carry on as follows:

::

  cd OpenCMISS/manage/build
  cmake -DOC_USE_IRON=NO -DMPI=none ..
  cmake --build .

Once everything has been built, we will most likely end up with a library file named something like `zinc.r9dddfb.dll` on Windows, `libzinc.r9dddfb.so.3` on Linux and `libzinc.r9dddfb.3.dylib` on macOS. Yet, we don't want the revision number, so to get the 'right' name (i.e. `zinc.dll` on Windows, `libzinc.so.3` on Linux and `libzinc.3.dylib` on macOS), we need to edit `[OpenCMISS]/src/zinc/CMakeLists.txt`, so that `ZINC_DEVELOPER_VERSION` gets set to an empty string. From there, we can rebuild `Zinc <http://physiomeproject.org/software/opencmiss/zinc>`__ library using `make zinc`.
