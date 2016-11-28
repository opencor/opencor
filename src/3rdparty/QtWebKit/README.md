[QtWebKit](https://wiki.qt.io/QtWebKit) has been officially replaced with [QtWebEngine](https://wiki.qt.io/QtWebEngine) starting with [Qt 5.6 LTS](https://www.qt.io/qt5-6/). However, [QtWebEngine](https://wiki.qt.io/QtWebEngine) is still lacking some features, making it impossible for us to switch to it just yet. Our original plan was therefore to stick with the Qt 5.5 branch, but it has known issues (e.g. https://bugreports.qt.io/browse/QTBUG-46882) and they will never be addressed since that branch is not being maintained anymore. We therefore decided to switch to [Qt 5.6 LTS](https://www.qt.io/qt5-6/), which means having to build our own copy of [QtWebKit](https://wiki.qt.io/QtWebKit). This requires the following:
 * [QtWebKit](https://wiki.qt.io/QtWebKit)'s core source code (see http://download.qt.io/community_releases/5.6/5.6.2/); and
 * Making sure that we are all set to to build [QtWebKit](https://wiki.qt.io/QtWebKit) on:
    * **Windows**, i.e. installing:
       * [Perl 5.22](http://www.activestate.com/activeperl/);
       * [Python 2.7](https://www.python.org/);
       * [Ruby >= 1.9](http://rubyinstaller.org/);
       * [Bison and Flex](https://sourceforge.net/projects/winflexbison/) (you will need to make a copy of win_flex.exe and name it flex.exe);
       * [gperf](http://gnuwin32.sourceforge.net/packages/gperf.htm); and
       * [ICU](http://www.npcglib.org/~stathis/blog/precompiled-icu/) (uncompress the binaries in `C:\icu`).<br/>
         **Note:** the GnuWin32 version of Flex is not suitable, hence we use [Win flex-bison](https://sourceforge.net/projects/winflexbison/) instead.
    * **Linux**, i.e. running the following command from the terminal:

      ```
      sudo apt-get install bison flex gperf libicu-dev libsqlite3-dev libxrender-dev
      ```

      as well as building ICU from [source](http://site.icu-project.org/download/) (make sure to get the version used by [Qt](https://www.qt.io/); `find /opt/Qt -name *icu*`) and from there:

      ```
      cd [ICU]
      mkdir build
      cd build
      ../source/runConfigureICU Linux --prefix=[ICU]/install
      make
      make install
      ```

From there, do the following from the command prompt or terminal:
 * **Command prompt:**

   ```
   cd [QtWebKit]
   SET INCLUDE=C:\icu\include;%INCLUDE%
   SET LIB=C:\icu\lib64;%LIB%
   SET PATH=C:\icu\lib64;%PATH%
   SET SQLITE3SRCDIR=C:\Qt\5.6\Src\qtbase\src\3rdparty\sqlite
   qmake
   jom
   ```

 * **Terminal *(Linux)*:**

   ```
   cd [QtWebKit]
   export PKG_CONFIG_PATH=[ICU]/install/lib/pkgconfig:$PKG_CONFIG_PATH
   qmake
   make -j 2
   ```

 * **Terminal *(macOS)*:**

   ```
   cd [QtWebKit]
   qmake
   make -j 4 release
   ```

Once [QtWebKit](https://wiki.qt.io/QtWebKit) is fully built:
 * For **each supported platform**:
    * Copy the contents of `[QtWebKit]/lib/cmake` to `[OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir]`;
    * Replace

      ```
      get_filename_component(_qt5WebKit_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
      ```

      with

      ```
      set(_qt5WebKit_install_prefix ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)
      ```

      in `[OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir]/Qt5WebKit/Qt5WebKitConfig.cmake`; and
    * Replace

      ```
      get_filename_component(_qt5WebKitWidgets_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
      ```

      with

      ```
      set(_qt5WebKitWidgets_install_prefix ${CMAKE_SOURCE_DIR}/src/3rdparty/QtWebKit)
      ```

      in `[OpenCOR]/src/3rdparty/QtWebKit/cmake/[PlatformDir]/Qt5WebKitWidgets/Qt5WebKitWidgetsConfig.cmake`.
 * Copy the **macOS** contents of `[QtWebKit]/lib/QtWebKit.framework/Headers` to `[OpenCOR]/src/3rdparty/QtWebKit/include/QtWebKit` and the **macOS** contents of `[QtWebKit]/lib/QtWebKitWidgets.framework/Headers` to `[OpenCOR]/src/3rdparty/QtWebKit/include/QtWebKitWidgets`; and
 * For **each supported platform**, create and upload to the [OpenCOR website](http://www.opencor.ws/) (under [OpenCOR]/binaries/src/3rdparty/QtWebKit/[PlatformDir]) a file called `QtWebKit.tar.gz` that contains:
    * **Windows:**
       * `[QtWebKit]/bin` containing the `.dll` files from `[QtWebKit]/lib`, and `icudt57.dll`, `icuin57.dll` and `icuuc57.dll` from the ICU library; and
       * `[QtWebKit]/lib` containing only the original `.lib` files;
    * **Linux:** `[QtWebKit]/lib` containing only the original `.so*` files; and
    * **macOS:** `[QtWebKit]/lib` containing only the original `.framework` folders.
