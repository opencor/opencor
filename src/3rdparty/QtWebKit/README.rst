In order to build `QtWebKit <https://wiki.qt.io/QtWebKit>`__, we need to:

- **Windows:** install:

  - `Perl 5.22 <http://www.activestate.com/activeperl/>`__;
  - `Python 2.7 <https://www.python.org/>`__;
  - `Ruby >= 1.9 <http://rubyinstaller.org/>`__;
  - `Bison and Flex <https://sourceforge.net/projects/winflexbison/>`__ (make a copy of ``win_flex.exe`` and name it ``flex.exe``);
  - `gperf <http://gnuwin32.sourceforge.net/packages/gperf.htm>`__; and

    **Note:** the `GnuWin32 <http://gnuwin32.sourceforge.net/>`__ version of `Flex <http://gnuwin32.sourceforge.net/packages/flex.htm>`__ is not suitable, hence we must use `Win flex-bison <https://sourceforge.net/projects/winflexbison/>`__ instead.

  There are additional dependencies (``ICU``, ``libjpeg``, ``libpng``, ``libxml2``, ``libxslt``, ``SQLite`` and ``zlib``) that are automatically downloaded upon building QtWebKit.
  (The prebuilt version of those binaries originally comes from `https://github.com/Vitallium/qtwebkit-libs-win/releases <https://github.com/Vitallium/qtwebkit-libs-win/releases>`__, to which we removed unneeded files.)

- **Linux:** run the following command from the terminal:

  ::

    sudo apt install bison gperf libsqlite3-dev libxml2-dev libxslt1-dev nasm ruby zlib1g-dev

- **macOS:** run the following command from the terminal (if needed, install `Homebrew <https://brew.sh/>`__):

  ::

    brew install nasm
