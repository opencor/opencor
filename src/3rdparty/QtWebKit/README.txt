Qt WebKit has been officially replaced with Qt WebEngine starting with Qt 5.6 LTS. However, Qt WebEngine is still lacking some features, making it impossible for us to switch to it just yet. Our original plan was therefore to stick with the Qt 5.5 branch, but it has known issues (e.g. https://bugreports.qt.io/browse/QTBUG-46882) and they will never be addressed since that branch is not being maintained anymore. We therefore decided to switch to the Qt 5.6 LTS, which means having to build our own copy of Qt WebKit. This requires the following:
 - Qt WebKit's core source code (see http://download.qt.io/community_releases/5.6/5.6.1/); and
 - Making sure that we are all set to to build Qt WebKit on:
    - Windows, i.e. installing:
       - Perl 5.22 (see http://www.activestate.com/activeperl/);
       - Python 2.7 (see https://www.python.org/);
       - Ruby >= 1.9 (see http://rubyinstaller.org/);
       - Bison and Flex (see https://sourceforge.net/projects/winflexbison/; you will need to make a copy of win_flex.exe and name it flex.exe);
       - gperf (see http://gnuwin32.sourceforge.net/packages/gperf.htm); and
       - ICU (see http://www.npcglib.org/~stathis/blog/precompiled-icu/; uncompress the binaries in C:\icu).
      Note: the GnuWin32 version of Flex is not suitable, hence we use Win flex-bison instead.
    - Linux, i.e. running the following command from the terminal:
       $ sudo apt-get install bison flex gperf libicu-dev libsqlite3-dev libxrender-dev

From there, do the following from the command prompt or terminal:
 - Command prompt:
    > cd [QtWebKit]
    > SET INCLUDE=C:\icu\include;%INCLUDE%
    > SET LIB=C:\icu\lib64;%LIB%
    > SET PATH=C:\icu\lib64;%PATH%
    > SET SQLITE3SRCDIR=C:\Qt\5.6\Src\qtbase\src\3rdparty\sqlite
    > qmake
    > jom
 - Terminal:
    $ cd [QtWebKit]
    $ qmake
    $ make
      or
      make release

Once Qt WebKit has been fully built:
 - Copy the CMake files (in [QtWebKit]/cmake/Qt5WebKit and [QtWebKit]/cmake/Qt5WebKitWidgets) to [OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir];
 - Replace
    get_filename_component(_qt5WebKit_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
   with
    set(_qt5WebKit_install_prefix ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)
   in [OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir]/Qt5WebKitConfig.cmake; and
 - Replace
    get_filename_component(_qt5WebKitWidgets_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
   with
    set(_qt5WebKitWidgets_install_prefix ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)
  in [OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir]/Qt5WebKitWidgetsConfig.cmake.
