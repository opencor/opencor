We build our own copy of the `Zinc <http://physiomeproject.org/software/opencmiss/zinc>`__ library, which requires doing the following (on macOS, for now):

::

  git clone https://github.com/OpenCMISS/manage.git OpenCMISS/manage
  cd OpenCMISS/manage/build
  cmake -DOC_USE_IRON=NO -DMPI=none ..
  make
