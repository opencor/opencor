Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::

    git diff --no-prefix v2.0.2 v2.0.2-opencor \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/PythonPackages/src/matplotlib_2.0.2.diff

IPython
-------

In our IPython's git controlled directory
::
    git diff --no-prefix 6.1.0 PythonQt-v6.1.0 \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/PythonPackages/src/ipython_6.1.0.diff
