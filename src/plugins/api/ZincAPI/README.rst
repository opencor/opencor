We build our own copy of the `Zinc API <http://physiomeproject.org/software/opencmiss/zinc>`__, which requires doing the following (on macOS, for now):

::

  git clone https://github.com/OpenCMISS/manage.git OpenCMISS
  cd OpenCMISS/build
  ccmake -DOC_USE_IRON=NO -DMPI=none ..
  cmake --build .
