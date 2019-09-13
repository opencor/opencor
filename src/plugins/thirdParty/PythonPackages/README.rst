Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::

    git diff --no-prefix v3.1.1 PythonQt-v3.1.1 > ~/build/OpenCOR/src/plugins/thirdparty/PythonPackages/src/matplotlib_3.1.1.diff
