In order to build `QtWebKit <https://github.com/qt/qtwebkit>`__, we need to:

- **Windows:** install:

  - `Perl >= 5.20 <https://activestate.com/activeperl>`__;
  - `Python 3.x <https://python.org/>`__;
  - `Ruby >= 1.9 <https://rubyinstaller.org/>`__;
  - `Bison and Flex <https://sourceforge.net/projects/winflexbison/>`__;
  - `gperf <http://gnuwin32.sourceforge.net/packages/gperf.htm>`__; and

  There are additional dependencies (``ICU``, ``libjpeg``, ``libpng``, ``libxml2``, ``libxslt``, ``SQLite`` and ``zlib``) that are automatically downloaded upon building QtWebKit.
  (The prebuilt version of those binaries originally comes from `https://github.com/Vitallium/qtwebkit-libs-win/releases <https://github.com/Vitallium/qtwebkit-libs-win/releases>`__, to which we removed unneeded files.)

- **Linux:** run the following command from the terminal:

  ::

    sudo apt install bison gperf libgstreamer-plugins-base1.0-dev libhyphen-dev libsqlite3-dev libxcomposite-dev libxml2-dev libxrender-dev libxslt1-dev nasm ruby zlib1g-dev

- **macOS:** run the following command from the terminal (if needed, install `Homebrew <https://brew.sh/>`__):

  ::

    brew install nasm
