Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::

    git diff --no-prefix v3.0.2 opencor-v3.0.2.1 \
      > ~/build/OpenCOR/src/plugins/thirdparty/PythonPackages/src/matplotlib_3.0.2.1.diff
