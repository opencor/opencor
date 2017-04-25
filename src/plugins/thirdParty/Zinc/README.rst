We build our own copy of the `Zinc <https://github.com/OpenCMISS/zinc>`__ library, which requires doing the following from the command line:

::

  mkdir OpenCMISS
  cd OpenCMISS
  mkdir setup-build
  git clone https://github.com/OpenCMISS/setup.git
  cd setup-build
  cmake -DOPENCMISS_ROOT=.. -DOPENCMISS_LIBRARIES=zinc ../setup
  cmake --build .
