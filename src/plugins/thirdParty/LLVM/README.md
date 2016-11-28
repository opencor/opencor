We need to build [LLVM](http://www.llvm.org/) (incl. [Clang](http://clang.llvm.org/)) ourselves so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build [LLVM](http://www.llvm.org/) as a shared library using MSVC (because of the need to expose classes/methods as `__declspec(dllexport)` or `__declspec(dllimport)` depending on whether [LLVM](http://www.llvm.org/) is being built or referenced, respectively).

To address this shortcoming, we must 'manually' expose the classes/methods that are needed by OpenCOR (`grep` for `---OPENCOR---` to find out which ones those are). Should OpenCOR ever need direct access to another class/method, then that class/method will obviously have to be exposed, and [LLVM](http://www.llvm.org/) rebuilt. In addition to exposing some classes/methods, we also had to 'fix' a few things. Again, those are highlighted (just look for `---OPENCOR---`).

In case [LLVM](http://www.llvm.org/) is to be built (by setting the `USE_PREBUILT_LLVM_PLUGIN` option to `OFF`), then keep in mind that configuration files have to be up to date. So, if needed, they may have to be regenerated using [CMake](https://www.cmake.org/) (on **Windows**, this may require installing [Python 2.7.x](http://www.python.org/download/)):
 * Download the [LLVM](http://www.llvm.org/) and [Clang](http://clang.llvm.org/) source codes from http://llvm.org/releases/download.html;
 * Extract their corresponding `.tar.xz` file;
 * Move the contents of `[Clang]` to `[LLVM]/tools/clang`;
 * From the command line:

   ```
   cd [LLVM]
   mkdir build
   cd build
   ```

   and then

   ```
   ccmake -G "Ninja" ..
   ```

   or

   ```
   ccmake -G "Unix Makefiles" ..
   ```

   or

   ```
   ccmake -G "NMake Makefiles JOM" ..
   ```

 * Configure the build with the following options:

   ```
   CLANG_ENABLE_ARCMT:             OFF
   CLANG_ENABLE_STATIC_ANALYZER:   OFF
   CLANG_INCLUDE_DOCS:             OFF
   CLANG_INCLUDE_TESTS:            OFF
   CLANG_INSTALL_SCANBUILD:        OFF
   CLANG_INSTALL_SCANVIEW:         OFF
   CLANG_PLUGIN_SUPPORT:           OFF
   LLVM_BUILD_RUNTIME:             OFF
   LLVM_BUILD_TOOLS:               OFF
   LLVM_ENABLE_TERMINFO:           OFF
   LLVM_ENABLE_ZLIB:               OFF
   LLVM_INCLUDE_DOCS:              OFF
   LLVM_INCLUDE_EXAMPLES:          OFF
   LLVM_INCLUDE_GO_TESTS:          OFF
   LLVM_INCLUDE_TESTS:             OFF
   LLVM_INCLUDE_UTILS:             OFF
   LLVM_TARGETS_TO_BUILD:          X86
   ```

   **Note:** to enable `TERMINFO` and `ZLIB` would require a more complicated build process and it is not worth it...
 * From the command line:

   ```
   cmake --build .
   ```

From there, the configuration files are to be manually copied over to the `windows`, `linux` and `macos` folders of the various folders. When it comes to `[LLVM]/include/llvm/Config/[PLATFORM_DIR]/config.h`, we should make sure that `HAVE_LIBEDIT` is not set (this is part of keeping the [LLVM](http://www.llvm.org/) build as simple as possible).
