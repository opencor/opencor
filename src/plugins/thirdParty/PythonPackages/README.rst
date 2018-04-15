Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::

    git diff --no-prefix v2.2.2 opencor-v2.2.2 \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/PythonPackages/src/matplotlib_2.2.2.diff
