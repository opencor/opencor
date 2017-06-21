In order to build `QtWebKit <https://wiki.qt.io/QtWebKit>`__, we need to:

- **Windows:** install:

  - `Perl 5.22 <http://www.activestate.com/activeperl/>`__;
  - `Python 2.7 <https://www.python.org/>`__;
  - `Ruby >= 1.9 <http://rubyinstaller.org/>`__;
  - `Bison and Flex <https://sourceforge.net/projects/winflexbison/>`__ (make a copy of ``win_flex.exe`` and name it ``flex.exe``);
  - `gperf <http://gnuwin32.sourceforge.net/packages/gperf.htm>`__; and
  - `ICU <http://www.npcglib.org/~stathis/blog/precompiled-icu/>`__ (get `v57.1 <https://github.com/opencor/qtwebkit/releases/download/v5.6.2/icu-57.1-vs2015.7z>`__ and uncompress it to ``C:\icu``).

    **Note:** the `GnuWin32 <http://gnuwin32.sourceforge.net/>`__ version of `Flex <http://gnuwin32.sourceforge.net/packages/flex.htm>`__ is not suitable, hence we must use `Win flex-bison <https://sourceforge.net/projects/winflexbison/>`__ instead.

- **Linux:** run the following command from the terminal:

  ::

    sudo apt-get install bison flex gperf libicu-dev libsqlite3-dev libxrender-dev

  **Note:** ICU also gets built from `source <http://site.icu-project.org/download/>`__ (after having made sure that we got the version used by `Qt <https://www.qt.io/>`__; ``find /opt/Qt -name *icu*``).
