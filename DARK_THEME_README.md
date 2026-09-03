# OpenCOR Dark Theme Guide

This guide explains how to run OpenCOR with the configurable Dark theme. It is
written for anyone using the dark-theme source branch and does not assume a
particular username, checkout directory, Qt installation, or Linux
distribution.

The theme is integrated into OpenCOR. Standard Qt widgets use an application
palette, while editors, plots, OpenCOR-generated HTML, and selected artwork
have additional theme handling.

## Do I need to build OpenCOR?

It depends on what you already have:

| Situation | Build required? | What to do |
|---|---:|---|
| You have an OpenCOR binary built from feature/dark-theme | No | Run that binary and select Dark in Preferences |
| This checkout already has a current build | No | Run the executable from the build directory |
| You cloned the source and have no build | Yes | Configure and build it once, then run it |
| You pulled or edited source code after the last build | Yes | Run the build command again; it will normally be incremental |
| You changed only this README | No | Run the existing binary |
| Your OpenCOR has no Appearance/Theme preference | Yes, or obtain a compatible binary | That executable does not contain this theme feature |

You do not rebuild OpenCOR every time you launch it. Building creates the
application; running starts it. A normal downloaded OpenCOR release supports
this guide only if its Preferences dialog contains the Theme option.

## Quick start with an existing build

From the repository root, use the command for your platform.

Linux:

~~~bash
./build/bin/OpenCOR
~~~

macOS:

~~~bash
open build/OpenCOR.app
~~~

Windows PowerShell:

~~~powershell
& .\build\Release\OpenCOR.exe
~~~

The exact Windows path can vary with the CMake generator. If it is not under
build/Release, search the build directory for OpenCOR.exe.

Once OpenCOR starts:

1. Open **Tools → Preferences…**.
2. Find the **Appearance** section.
3. Set **Theme** to **Dark**.
4. Select **OK**.

The open interface changes immediately; a restart is not required. The choice
is saved for later launches.

If the Appearance section or Theme selector is missing, the executable was not
built from a revision containing the dark-theme feature.

## Build from source

Skip this section if you already have a compatible executable.

### 1. Get the dark-theme source

For a new checkout:

~~~bash
git clone https://github.com/opencor/opencor.git
cd opencor
git switch feature/dark-theme
~~~

For an existing checkout:

~~~bash
git switch feature/dark-theme
git pull --ff-only
~~~

Check the active branch:

~~~bash
git branch --show-current
~~~

It should report feature/dark-theme.

### 2. Install the build requirements

The current source requires:

- a 64-bit operating system;
- Git;
- CMake 3.22 or newer;
- Qt 5.12.x development files (not Qt 5.15 or Qt 6);
- a build tool such as Ninja or Make;
- GCC/G++ on Linux, Apple Clang on macOS, or the supported Visual Studio 2022
  toolchain on Windows;
- the platform development libraries normally required by Qt;
- network access and disk space for third-party packages that are not already
  cached.

Confirm CMake:

~~~bash
cmake --version
~~~

Locate the Qt 5.12 installation directory. It is the directory containing both
of these paths:

~~~text
bin/qmake
lib/cmake/Qt5Core/Qt5CoreConfig.cmake
~~~

Typical Qt Online Installer layouts resemble:

~~~text
/opt/Qt/5.12.12/gcc_64
C:\Qt\5.12.12\msvc2017_64
~~~

These are examples only. Use the real location on your computer.

On Linux or macOS, set a quoted variable:

~~~bash
export OPENCOR_QT_PREFIX="/absolute/path/to/Qt/5.12.x/compiler_64"
"$OPENCOR_QT_PREFIX/bin/qmake" -query QT_VERSION
~~~

On Windows PowerShell:

~~~powershell
$env:OPENCOR_QT_PREFIX = "C:\absolute\path\to\Qt\5.12.x\compiler_64"
& "$env:OPENCOR_QT_PREFIX\bin\qmake.exe" -query QT_VERSION
~~~

The reported version must start with 5.12.

Do not enter documentation placeholders containing angle brackets literally.
In shells such as Bash, angle brackets are redirection operators.

### 3. Configure and compile

Linux or macOS with Ninja or Make:

~~~bash
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTS=OFF \
    -DCMAKE_PREFIX_PATH="$OPENCOR_QT_PREFIX"

cmake --build build --parallel
~~~

Windows PowerShell with a multi-configuration generator:

~~~powershell
cmake -S . -B build -DENABLE_TESTS=OFF -DCMAKE_PREFIX_PATH="$env:OPENCOR_QT_PREFIX"
cmake --build build --config Release --parallel
~~~

A successful build ends with the OpenCOR target completed. The usual output
locations are:

| Platform | Application |
|---|---|
| Linux | build/bin/OpenCOR |
| macOS | build/OpenCOR.app |
| Windows | build/Release/OpenCOR.exe |

The build may also deploy libraries and plugins beside the application. Keep
that directory structure together when running the development build.

### 4. Rebuild after source changes

Use the same build directory and rerun only the build step:

~~~bash
cmake --build build --parallel
~~~

For a multi-configuration generator:

~~~bash
cmake --build build --config Release --parallel
~~~

CMake will normally compile only what changed. Reconfigure first if CMake files,
dependencies, the Qt installation, or build options changed.

## Run a model

You can pass a model path when starting OpenCOR. For example, on Linux:

~~~bash
./build/bin/OpenCOR models/van_der_pol_model_1928.cellml
~~~

Other sample models in this repository include:

~~~text
models/hodgkin_huxley_squid_axon_model_1952.cellml
models/noble_model_1962.cellml
~~~

The selected theme changes presentation only. It does not alter models,
solvers, or simulation results.

## Switch between Dark and Light

To change themes while OpenCOR is running:

1. Open **Tools → Preferences…**.
2. Change **Appearance → Theme** to **Dark** or **Light**.
3. Select **OK**.

The interface updates immediately. Do not use **Tools → Reset All** just to
change the theme because it also clears unrelated settings.

Users without a saved theme preference start in Dark. An explicitly saved
Light preference remains Light.

## Theme persistence

OpenCOR stores the choice through QSettings at:

~~~text
Preferences/Appearance/Theme
~~~

Valid values are Light and Dark. On many Linux desktops the settings file is:

~~~text
~/.config/Physiome/OpenCOR.conf
~~~

The location differs by operating system and may also be affected by
XDG_CONFIG_HOME. Prefer the Preferences dialog over editing the file manually.

To verify persistence:

1. Select Dark and accept Preferences with **OK**.
2. Quit OpenCOR normally.
3. Start the same executable under the same user account.
4. Confirm that Dark is active when the application appears.

## Quick validation checklist

After building a new revision:

1. Switch from Dark to Light and back to Dark.
2. Check menus, toolbars, tabs, dialogs, docks, tooltips, and disabled controls.
3. Open a CellML model in the CellML Text and Raw Text views.
4. Check text, caret, selection, syntax highlighting, and error indicators.
5. Run a simulation and inspect the graph background, axes, labels, grid,
   legend, and data-series colours.
6. Quit and restart to confirm the setting is remembered.
7. Return to Light and confirm the original appearance is restored.

## Troubleshooting

### CMake cannot find Qt5Core

Make sure CMAKE_PREFIX_PATH points to the Qt installation prefix, not to its
bin or lib subdirectory:

~~~bash
test -f "$OPENCOR_QT_PREFIX/lib/cmake/Qt5Core/Qt5CoreConfig.cmake"
cmake -S . -B build -DCMAKE_PREFIX_PATH="$OPENCOR_QT_PREFIX"
~~~

### CMake rejects the Qt version

This source tree requires Qt 5.12.x:

~~~bash
"$OPENCOR_QT_PREFIX/bin/qmake" -query QT_VERSION
~~~

Qt 5.15 and Qt 6 are not accepted by the current top-level CMake configuration.

### CMake reports a generator mismatch

A build directory remembers its generator. Continue using that generator, or
configure a separate directory:

~~~bash
cmake -S . -B build-release \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTS=OFF \
    -DCMAKE_PREFIX_PATH="$OPENCOR_QT_PREFIX"

cmake --build build-release --parallel
~~~

### OpenCOR fails because a shared library is missing

On Linux, inspect unresolved dependencies:

~~~bash
ldd build/bin/OpenCOR | grep "not found"
~~~

Install the missing runtime library for the operating system or ensure the
matching Qt libraries are discoverable. Avoid globally mixing libraries or Qt
plugins from different Qt versions.

Some third-party binary packages may expect an older system-library ABI. That
is a dependency/toolchain compatibility issue, not a dark-theme setting.
Prefer a compatible toolchain or rebuild the affected dependency; do not rename
an incompatible shared library to imitate another ABI.

### OpenCOR cannot load the xcb platform plugin on Linux

Confirm that the plugin was deployed:

~~~bash
test -f build/plugins/platforms/libqxcb.so
ldd build/plugins/platforms/libqxcb.so | grep "not found"
~~~

Install any missing X11/xcb runtime packages for the Linux distribution. Do not
set QT_PLUGIN_PATH to an unrelated Qt installation.

### Dark mode is not remembered

- Accept Preferences with **OK** rather than cancelling the dialog.
- Quit OpenCOR normally.
- Use the same operating-system account for the next launch.
- Confirm that the account can write to its settings directory.

### A component remains light

Close and reopen the affected view. If OpenCOR-owned content still does not
update, record the operating system, Qt style, component or plugin name, and a
screenshot. External websites and user-provided HTML are intentionally not
restyled.

## Developer notes

The main implementation points are:

- src/misc/thememanager.h and src/misc/thememanager.cpp manage theme state,
  persistence, and the application palette;
- src/main.cpp applies the saved theme before visible widgets are created;
- src/preferencesdialog.cpp and src/preferencesdialog.ui expose the selector;
- custom-painted widgets should react to QEvent::PaletteChange;
- components should prefer QPalette roles over hard-coded foreground and
  background colours;
- semantic colours for syntax, errors, warnings, annotations, and plotted data
  should remain distinct;
- only OpenCOR-owned HTML should receive the local dark stylesheet;
- Light mode must restore the original palette and component colours.

## More information

- OpenCOR project: <https://opencor.ws/>
- User documentation: <https://opencor.ws/user/index.html>
- Developer documentation: <https://opencor.ws/developer/index.html>
- CellML/OpenCOR/PMR tutorial:
  <https://tutorial-on-cellml-opencor-and-pmr.readthedocs.io/en/latest/index.html>

