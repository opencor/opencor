We build our own copy of the `Zinc <http://physiomeproject.org/software/opencmiss/zinc>`__ library (on Linux and macOS for now), which requires doing the following from the command line:

::

  git clone https://github.com/OpenCMISS/manage.git OpenCMISS/manage
  cd OpenCMISS/manage/build
  cmake -DOC_USE_IRON=NO -DMPI=none ..
  cmake --build .

Once everything has been built, we will most likely end up with a library file named something like ``libzinc.r9dddfb.so.3`` on Linux and ``libzinc.r9dddfb.3.dylib`` on macOS. Yet, we don't want the revision number, so to get the 'right' name (i.e. ``libzinc.so.3`` on Linux and ``libzinc.3.dylib`` on macOS), we need to edit ``[OpenCMISS]/src/zinc/CMakeLists.txt``, so that ``ZINC_RELEASE`` gets set to ``TRUE``. From there, we can rebuild `Zinc <http://physiomeproject.org/software/opencmiss/zinc>`__ library using ``make zinc``.
