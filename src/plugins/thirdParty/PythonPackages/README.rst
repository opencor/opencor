Upgrades
--------

numpy --> 1.12.1

scipy --> 0.19.0

matplotlib --> 2.0.2


Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::
    git diff --no-prefix v1.5.3 v1.5.3-opencor \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/PythonPackages/src/matplotlib_1.5.3.diff

IPython
-------

In our IPython's git controlled directory
::
    git diff --no-prefix 6.0.0 v6.0.0-opencor \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/PythonPackages/src/ipython_6.0.0.diff
