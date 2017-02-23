Upgrades
--------

matplotlib --> 2.0.0

Linux requirements
------------------

    sudo apt-get install libreadline-dev
    sudo apt-get install libncurses-dev
    sudo apt-get install sqlite3
    sudo apt-get install libsqlite3-dev

OS/X requirements
-----------------

    brew install sqlite3

Windows
-------

We can not build a debug version since debug versions of numpy, scipy, and
matplotlib are not available as binary wheels. (The alternative is to build
our own debug versions of these packages...).

Matplotlib
----------

In our matplotlib's git controlled directory
::
    git diff --no-prefix v1.5.3 PythonQt_v1.5.3 \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/Python/src/matplotlib_1.5.3.diff

IPython
-------

In our ipython's git controlled directory
::
    git diff --no-prefix 5.2.2 PythonQt_v5.2.2 \
      > ~/build/OpenCOR-upstream/src/plugins/thirdparty/Python/src/ipython_5.2.2.diff
