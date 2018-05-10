In order to build `QtWebKit <https://wiki.qt.io/QtWebKit>`_, we need to:

- **Windows:** install:

  - `Perl 5.22 <http://www.activestate.com/activeperl/>`_;
  - `Python 2.7 <https://www.python.org/>`_;
  - `Ruby >= 1.9 <http://rubyinstaller.org/>`_;
  - `Bison and Flex <https://sourceforge.net/projects/winflexbison/>`_ (make a copy of ``win_flex.exe`` and name it ``flex.exe``);
  - `gperf <http://gnuwin32.sourceforge.net/packages/gperf.htm>`_; and

    **Note:** the `GnuWin32 <http://gnuwin32.sourceforge.net/>`_ version of `Flex <http://gnuwin32.sourceforge.net/packages/flex.htm>`_ is not suitable, hence we must use `Win flex-bison <https://sourceforge.net/projects/winflexbison/>`_ instead.

  There are additional dependencies (``libxml2``, ``libxslt``, ``SQLite`` and ``zlib``) that are automatically downloaded upon building QtWebKit.
  (The prebuilt version of those binaries originally comes from `https://github.com/Vitallium/qtwebkit-libs-win/releases <https://github.com/Vitallium/qtwebkit-libs-win/releases>`_, to which we removed unneeded files.)

- **Linux:** run the following command from the terminal:

  ::

    sudo apt install bison gperf libsqlite3-dev libxml2-dev libxslt1-dev ruby zlib1g-dev
