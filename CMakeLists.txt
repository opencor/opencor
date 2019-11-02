cmake_minimum_required(VERSION 3.14)

# Project definition

project(OpenCOR)

# Our options

option(ENABLE_CLANG_TIDY "Enable the use of Clang-Tidy" OFF)

option(ENABLE_SAMPLE_PLUGINS "Enable the sample plugins to be built" OFF)
option(ENABLE_TEST_PLUGINS "Enable the test plugins to be built" OFF)
option(ENABLE_TESTS "Enable the tests to be built" OFF)

if(NOT WIN32 AND NOT APPLE)
    option(USE_PREBUILT_ICU_PACKAGE "Use the pre-built version of the ICU package" ON)
    option(USE_PREBUILT_MESA_PACKAGE "Use the pre-built version of the Mesa package" ON)
endif()

option(USE_PREBUILT_QTWEBKIT_PACKAGE "Use the pre-built version of the QtWebKit package" ON)

option(USE_PREBUILT_BIOSIGNALML_PACKAGE "Use the pre-built version of the BioSignalML package" ON)
option(USE_PREBUILT_CELLMLAPI_PACKAGE "Use the pre-built version of the CellML API package" ON)
option(USE_PREBUILT_LIBGIT2_PACKAGE "Use the pre-built version of the libgit2 package" ON)
option(USE_PREBUILT_LIBNUML_PACKAGE "Use the pre-built version of the libNuML package" ON)
option(USE_PREBUILT_LIBSBML_PACKAGE "Use the pre-built version of the libSBML package" ON)
option(USE_PREBUILT_LIBSEDML_PACKAGE "Use the pre-built version of the libSEDML package" ON)
option(USE_PREBUILT_LIBXDIFF_PACKAGE "Use the pre-built version of the LibXDiff package" ON)
option(USE_PREBUILT_LLVMCLANG_PACKAGE "Use the pre-built version of the LLVMClang package" ON)
option(USE_PREBUILT_OAUTH_PACKAGE "Use the pre-built version of the OAuth package" ON)
option(USE_PREBUILT_OPENSSL_PACKAGE "Use the pre-built version of the OpenSSL package" ON)
option(USE_PREBUILT_PYTHON_PACKAGE "Use the pre-built version of the Python package" ON)
option(USE_PREBUILT_PYTHON_PACKAGES_PACKAGE "Use the pre-built version of various Python packages" ON)
option(USE_PREBUILT_PYTHONQT_PACKAGE "Use the pre-built version of the PythonQt package" ON)
option(USE_PREBUILT_QSCINTILLA_PACKAGE "Use the pre-built version of the QScintilla package" ON)
option(USE_PREBUILT_QWT_PACKAGE "Use the pre-built version of the Qwt package" ON)
option(USE_PREBUILT_SUNDIALS_PACKAGE "Use the pre-built version of the SUNDIALS package" ON)
option(USE_PREBUILT_ZLIB_PACKAGE "Use the pre-built version of the zlib package" ON)

if(ENABLE_TEST_PLUGINS)
    option(USE_PREBUILT_ZINC_PACKAGE "Use the pre-built version of the Zinc package" ON)
endif()

# Determine whether we are using the prebuilt version of our Python and QtWebKit
# packages

if(USE_PREBUILT_PYTHON_PACKAGE AND USE_PREBUILT_QTWEBKIT_PACKAGE)
    set(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES ON)
endif()

# Make sure that we are using the compiler we support

if(WIN32)
    string(REPLACE "." ";"
           CMAKE_CXX_COMPILER_VERSION_LIST "${CMAKE_CXX_COMPILER_VERSION}")

    list(GET CMAKE_CXX_COMPILER_VERSION_LIST 0 CMAKE_CXX_COMPILER_VERSION_MAJOR)
    list(GET CMAKE_CXX_COMPILER_VERSION_LIST 1 CMAKE_CXX_COMPILER_VERSION_MINOR)

    if(   NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"
       OR NOT "${CMAKE_CXX_COMPILER_VERSION_MAJOR}" STREQUAL "19"
       OR NOT "${CMAKE_CXX_COMPILER_VERSION_MINOR}" STREQUAL "16")
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using MSVC 2017 on Windows...")
    endif()
elseif(APPLE)
    if(    NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
       AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using (Apple) Clang on macOS...")
    endif()
else()
    if(    NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"
       AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using GCC/G++ on Linux...")
    endif()
endif()

# Determine the effective build directory

set(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR})

if(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    # With Xcode, we have a configuration directory, but it messes up our build
    # system, so ask for all the binaries to be generated in our build folder

    set(XCODE TRUE)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
else()
    # Check whether there is a configuration directory (the case with MSVC) and,
    # if so, make use of it

    set(XCODE FALSE)

    if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
        set(PROJECT_BUILD_DIR ${PROJECT_BUILD_DIR}/${CMAKE_CFG_INTDIR})
    endif()
endif()

# Make sure that we are building OpenCOR on a supported architecture
# Note: normally, we would check the value of CMAKE_SIZEOF_VOID_P, but in some
#       cases it may not be set (e.g. when generating an Xcode project file), so
#       we determine and retrieve that value ourselves...

try_run(ARCHITECTURE_RUN ARCHITECTURE_COMPILE
        ${PROJECT_BUILD_DIR} ${CMAKE_SOURCE_DIR}/cmake/architecture.c
        RUN_OUTPUT_VARIABLE ARCHITECTURE)

if(NOT ARCHITECTURE_COMPILE)
    message(FATAL_ERROR "We could not determine your architecture. Please clean your ${CMAKE_PROJECT_NAME} environment and try again...")
elseif(NOT ${ARCHITECTURE} EQUAL 64)
    message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in 64-bit mode...")
endif()

# By default, we are building a release version of OpenCOR, unless we are
# explicitly asked for a debug version

if(   "${CMAKE_BUILD_TYPE}" STREQUAL ""
   OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(BUILD_INFORMATION "Building a release version of ${CMAKE_PROJECT_NAME}")

    set(RELEASE_MODE TRUE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(BUILD_INFORMATION "Building a debug version of ${CMAKE_PROJECT_NAME}")

    set(RELEASE_MODE FALSE)
else()
    message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in release or debug mode...")
endif()

# Try to build our silent program

set(SILENTRUN ${PROJECT_BUILD_DIR}/silentrun)

if(WIN32)
    execute_process(COMMAND ${CMAKE_C_COMPILER} /O2 /Fe${SILENTRUN} ${PROJECT_SOURCE_DIR}/cmake/silentrun.c
                    RESULT_VARIABLE RESULT OUTPUT_QUIET ERROR_QUIET)
else()
    if(APPLE)
        set(INCLUDE_ARGUMENT -I ${CMAKE_OSX_SYSROOT}/usr/include)
    endif()

    execute_process(COMMAND ${CMAKE_C_COMPILER} -O3 ${INCLUDE_ARGUMENT} -o ${SILENTRUN} ${PROJECT_SOURCE_DIR}/cmake/silentrun.c
                    RESULT_VARIABLE RESULT OUTPUT_QUIET ERROR_QUIET)
endif()

if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "silentrun could not be built...")
endif()

# Try to build our runpath2rpath program, if we are on Linux

if(NOT WIN32 AND NOT APPLE)
    set(RUNPATH2RPATH ${PROJECT_BUILD_DIR}/runpath2rpath)

    execute_process(COMMAND ${CMAKE_C_COMPILER} -O3 -o ${RUNPATH2RPATH} ${PROJECT_SOURCE_DIR}/cmake/runpath2rpath.c
                    RESULT_VARIABLE RESULT OUTPUT_QUIET ERROR_QUIET)

    if(NOT RESULT EQUAL 0)
        message(FATAL_ERROR "runpath2rpath could not be built...")
    endif()
endif()

# Look and get ready for Clang-Tidy
# Note: we don't want Clang-Tidy to analyse files in our build directory.
#       Indeed, some files generated by Qt may result in Clang-Tidy warnings
#       that have nothing to do with OpenCOR. We therefore ignore them by
#       creating a special .clang-tidy file in our build directory. That file
#       disables all checks but one, which is necessary for Clang-Tidy to work.
#       That check is for boost-use-to-string, which is as if we didn't have
#       any checks since we don't use Boost ...

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)

    if(CLANG_TIDY)
        file(WRITE "${PROJECT_BUILD_DIR}/.clang-tidy"
"---
Checks: >-
  -*,
  boost-use-to-string
...
")
    else()
        message(FATAL_ERROR "Clang-Tidy could not be found...")
    endif()
endif()

# Make sure that clcache/ccache gets used, if available

if(WIN32)
    find_program(CLCACHE clcache)

    if(CLCACHE)
        set(CLCACHEWRAPPER ${PROJECT_BUILD_DIR}/clcachewrapper)

        execute_process(COMMAND ${CMAKE_C_COMPILER} /O2 /Fe${CLCACHEWRAPPER} ${PROJECT_SOURCE_DIR}/cmake/clcachewrapper.c
                        RESULT_VARIABLE RESULT OUTPUT_QUIET ERROR_QUIET)

        if(RESULT EQUAL 0)
            set(CMAKE_C_COMPILER_LAUNCHER ${CLCACHEWRAPPER})
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CLCACHEWRAPPER})
        endif()
    endif()
else()
    find_program(CCACHE ccache)

    if(CCACHE)
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    endif()
endif()

# Keep track of some basic information about Qt, after making sure that we have
# the version of Qt that we need

find_package(Qt5Core REQUIRED)

set(QT_VERSION ${Qt5Core_VERSION})
set(QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR})
set(QT_VERSION_MINOR ${Qt5Core_VERSION_MINOR})

set(REQUIRED_QT_LTS_VERSION_MAJOR 5)
set(REQUIRED_QT_LTS_VERSION_MINOR 12)

if(   NOT ${QT_VERSION_MAJOR} EQUAL ${REQUIRED_QT_LTS_VERSION_MAJOR}
   OR NOT ${QT_VERSION_MINOR} EQUAL ${REQUIRED_QT_LTS_VERSION_MINOR})
    message(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using Qt ${REQUIRED_QT_LTS_VERSION_MAJOR}.${REQUIRED_QT_LTS_VERSION_MINOR}.x LTS...")
endif()

set(QT_DIR ${_qt5Core_install_prefix})
set(QT_BINARY_DIR ${QT_DIR}/bin)
set(QT_LIBRARY_DIR ${QT_DIR}/lib)
set(QT_PLUGINS_DIR ${QT_DIR}/plugins)

get_target_property(QMAKE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)

set(QMAKE_COMMAND ${QMAKE} -Wnone)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Determine our platform directory

if(WIN32)
    set(PLATFORM windows)

    if(RELEASE_MODE)
        set(TARGET_PLATFORM ${PLATFORM}.release)
    else()
        set(TARGET_PLATFORM ${PLATFORM}.debug)
    endif()
else()
    if(APPLE)
        set(PLATFORM macos)
    else()
        set(PLATFORM linux)
    endif()

    set(TARGET_PLATFORM ${PLATFORM})
endif()

set(PLATFORM_DIR ${PLATFORM})

# Location of our distribution files

set(DISTRIB_DIR ${CMAKE_SOURCE_DIR}/distrib)
set(PLATFORM_DISTRIB_DIR ${DISTRIB_DIR}/${PLATFORM})

# Default location of external binaries and packages

if(WIN32)
    if(RELEASE_MODE)
        set(EXTERNAL_PACKAGE_DIR release)
    else()
        set(EXTERNAL_PACKAGE_DIR debug)
    endif()

    set(DEST_EXTERNAL_LIBRARIES_DIR bin)
else()
    set(EXTERNAL_PACKAGE_DIR .)

    if(APPLE)
        set(DEST_EXTERNAL_LIBRARIES_DIR ${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)
    else()
        set(DEST_EXTERNAL_LIBRARIES_DIR lib)
    endif()
endif()

set(FULL_DEST_EXTERNAL_LIBRARIES_DIR ${PROJECT_BUILD_DIR}/${DEST_EXTERNAL_LIBRARIES_DIR})

if(NOT EXISTS ${FULL_DEST_EXTERNAL_LIBRARIES_DIR})
    file(MAKE_DIRECTORY ${FULL_DEST_EXTERNAL_LIBRARIES_DIR})
endif()

# Some in-house CMake macros

include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Make sure that we can use the ExternalProject module, and let it know where we
# want to build our external packages
# Note: indeed, otherwise on Windows we may end up with path names that are too
#       long...

include(ExternalProject)

set(EXTERNAL_PROJECT_BUILD_DIR ${PROJECT_BUILD_DIR}/ext)

set_property(DIRECTORY PROPERTY EP_BASE ${EXTERNAL_PROJECT_BUILD_DIR})

# A few constants to help us with building an external project

if(CCACHE)
    set(CMAKE_ARGS -DCMAKE_CXX_COMPILER_LAUNCHER=${CCACHE})
endif()

if(APPLE)
    set(CMAKE_ARGS ${CMAKE_ARGS} -DCMAKE_MACOSX_RPATH=ON)
endif()

if(WIN32)
    set(MAKE_NMAKE_COMMAND nmake)
    set(MAKE_JOM_COMMAND jom)
else()
    set(MAKE_NMAKE_COMMAND make)
    set(MAKE_JOM_COMMAND ${MAKE_NMAKE_COMMAND})

    include(ProcessorCount)

    ProcessorCount(PROCESSOR_COUNT)

    if(NOT PROCESSOR_COUNT EQUAL 0)
        set(MAKE_NMAKE_COMMAND ${MAKE_NMAKE_COMMAND} -j ${PROCESSOR_COUNT})
        set(MAKE_JOM_COMMAND ${MAKE_JOM_COMMAND} -j ${PROCESSOR_COUNT})
    endif()
endif()

# A couple of variables that make it easier to specify library file names with a
# version (e.g. to be able to reference libz.so.1 and libz.1.dylib, we could
# simply use libz${PRE}.1${POST})

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX_POST ${CMAKE_SHARED_LIBRARY_SUFFIX})
elseif(NOT WIN32)
    set(CMAKE_SHARED_LIBRARY_SUFFIX_PRE ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

# On Windows, specify the version of ICU that we are using while on Linux, add
# our copy of the ICU and Mesa libraries
# Note #1: on Linux, ICU is needed so that our external projects that need ICU
#          (e.g. QtWebKit) can rely on the ICU version that we want to use...
# Note #2: on Linux, Mesa is needed so that we can run OpenCOR on machines that
#          don't have 'proper' OpenGL support (e.g. on a Linux virtual machine
#          in VirtualBox)...

if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
    if(WIN32)
        set(ICU_VERSION 57)
    elseif(NOT APPLE)
        add_subdirectory(${PLATFORM_DISTRIB_DIR}/icu)
        add_subdirectory(${PLATFORM_DISTRIB_DIR}/mesa)
    endif()
endif()

# Retrieve or build our copy of QtWebKit, but only if we are not building Python

if(USE_PREBUILT_PYTHON_PACKAGE)
    add_subdirectory(src/3rdparty/QtWebKit)
endif()

# Required Qt modules

if(ENABLE_TESTS)
    set(TEST Test)
endif()

set(REQUIRED_QT_MODULES
    Network
    ${TEST}
    Widgets
)

if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
    set(WEBKIT WebKit)
    set(WEBKITWIDGETS WebKitWidgets)

    if(WIN32)
        list(APPEND REQUIRED_QT_MODULES ${WEBKIT})
    endif()
endif()

foreach(REQUIRED_QT_MODULE ${REQUIRED_QT_MODULES})
    find_package(Qt5${REQUIRED_QT_MODULE} REQUIRED)
endforeach()

# On macOS, keep track of the Qt libraries against which we need to link and
# make sure that we always use their release version
# Note: indeed, from Qt 5.12.5, to build a debug version of a Qt-based
#       application implies using the debug version of the Qt libraries, which
#       we don't want to do (since it would require us to generate both a
#       release and a debug version of our Qt-based third-party libraries)...

if(APPLE)
    if(ENABLE_TESTS)
        set(TEST Test)
    endif()

    if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
        set(WEBKIT WebKit)
        set(WEBKITWIDGETS WebKitWidgets)
    endif()

    set(MACOS_QT_LIBRARIES
        Concurrent
        Core
        DBus
        Gui
        Help
        MacExtras
        Multimedia
        MultimediaWidgets
        Network
        OpenGL
        Positioning
        PrintSupport
        Qml
        Quick
        Sensors
        Sql
        Svg
        ${TEST}
        WebChannel
        ${WEBKIT}
        ${WEBKITWIDGETS}
        Widgets
        Xml
        XmlPatterns
    )

    foreach(QT_LIBRARY ${MACOS_QT_LIBRARIES})
        find_package(Qt5${QT_LIBRARY} REQUIRED)

        get_target_property(QT_RELEASE_LIBRARY Qt5::${QT_LIBRARY} IMPORTED_LOCATION_RELEASE)
        set_target_properties(Qt5::${QT_LIBRARY} PROPERTIES "IMPORTED_LOCATION_DEBUG" "${QT_RELEASE_LIBRARY}")
    endforeach()
endif()

# Some general build settings
# Note #1: we force the overriding of CMAKE_CXX_FLAGS_RELEASE and
#          CMAKE_CXX_FLAGS_DEBUG, so that we can really use the compilation
#          flags we want, as opposed to CMake forcing us to use the ones it
#          wants...
# Note #2: MSVC enables C++11 support by default, so we just need to enable it
#          on Linux and macOS...

set(CMAKE_CXX_FLAGS_RELEASE "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "" FORCE)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /WX /GR /EHsc")
    # Note: MSVC has a /Wall flag, but it results in MSVC being very pedantic,
    #       so instead we use what MSVC recommends for production code, which is
    #       /W3 and which is also what CMake uses by default...

    set(LINK_FLAGS_PROPERTIES "/STACK:10000000")
else()
    set(CMAKE_CXX_FLAGS "-Wall -W -Werror -std=c++11")

    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(LINK_FLAGS_PROPERTIES "-stdlib=libc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")
        # Note: needed so that the OpenCOR binary doesn't get considered as a
        #       shared library file on Ubuntu when using Nautilus (see
        #       https://stackoverflow.com/a/45332687)...
    endif()
endif()

# On macOS, we want to be able to access Cocoa

if(APPLE)
    set(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -framework AppKit")
endif()

# Some build settings that depend on whether we want a release or a debug
# version of OpenCOR

if(RELEASE_MODE)
    # Default release compiler and linker settings

    if(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /MD /O2 /Ob2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
    endif()

    if(NOT WIN32 AND NOT APPLE)
        set(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
        # Note #1: -Wl,-s strips all the symbols, thus reducing the final size
        #          of OpenCOR or one its shared libraries...
        # Note #2: the above linking option has become obsolete on macOS...
    endif()

    # Make sure that debugging is disabled in Qt

    add_definitions(-DQT_NO_DEBUG)
else()
    # Default debug compiler and linker settings

    if(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Z7 /Ob0 /Od /RTC1")
        set(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /DEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    endif()

    # Make sure that debugging is enabled in Qt

    add_definitions(-DQT_DEBUG)
endif()

# Ask for Qt deprecated uses to be reported

add_definitions(-DQT_DEPRECATED_WARNINGS)

# Make sure that Unicode is defined
# Note: at least needed for QtSingleApplication on Windows...

add_definitions(-DUNICODE)

# On macOS, make sure that we support version 10.12 and later, unless a specific
# deployment target has been specified

if(APPLE)
    if("${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
        set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
    endif()

    set(BUILD_INFORMATION "${BUILD_INFORMATION} for macOS ${CMAKE_OSX_DEPLOYMENT_TARGET} and later")
endif()

# Destination of our plugins so that we don't have to deploy OpenCOR on Windows
# and Linux before being able to test it

if(APPLE)
    set(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${CMAKE_PROJECT_NAME})
else()
    set(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/plugins/${CMAKE_PROJECT_NAME})
endif()

# Set the RPATH (and RPATH link, if needed) information on Linux and macOS

if(APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR TRUE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks;@executable_path/../PlugIns/${CMAKE_PROJECT_NAME}")
elseif(NOT WIN32)
    set(CMAKE_SKIP_RPATH TRUE)

    set(LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN/../lib'")
    set(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-rpath-link,${QT_LIBRARY_DIR} ${LINK_RPATH_FLAG}")
endif()

# Show what we are about to build

message("${BUILD_INFORMATION} using Qt ${QT_VERSION} LTS...")

# Keep track of our source and build directories (needed to run our tests)

set(SOURCE_DIRECTORY_FILENAME ${PROJECT_BUILD_DIR}/sourcedirectory.txt)
set(BUILD_DIRECTORY_FILENAME ${PROJECT_BUILD_DIR}/builddirectory.txt)

file(WRITE ${SOURCE_DIRECTORY_FILENAME} "${CMAKE_SOURCE_DIR}")
file(WRITE ${BUILD_DIRECTORY_FILENAME} "${PROJECT_BUILD_DIR}")

track_files(
    ${SOURCE_DIRECTORY_FILENAME}
    ${BUILD_DIRECTORY_FILENAME}
)

# Version/snapshot of OpenCOR

if("${PROJECT_VERSION}" STREQUAL "")
    set(SNAPSHOT ON)
endif()

string(TIMESTAMP DATE "%Y-%m-%d")

if(SNAPSHOT)
    set(VERSION ${DATE})
else()
    set(VERSION ${PROJECT_VERSION})
endif()

set(VERSION_DATE_FILENAME ${PROJECT_BUILD_DIR}/versiondate.txt)

file(WRITE ${VERSION_DATE_FILENAME} "${VERSION}\n${DATE}")

track_files(${VERSION_DATE_FILENAME})

# Configure our QRC files

set(COMMON_QRC_FILENAME ${PROJECT_BUILD_DIR}/res/common.qrc)
set(I18N_QRC_FILENAME ${PROJECT_BUILD_DIR}/res/i18n.qrc)

configure_file(${CMAKE_SOURCE_DIR}/res/common.qrc.in ${COMMON_QRC_FILENAME})
configure_file(${CMAKE_SOURCE_DIR}/res/i18n.qrc.in ${I18N_QRC_FILENAME})

# The Python interface depends on Python headers, so we only include it if we
# are using the prebuilt version of our Python and QtWebKit packages

if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
    set(PYTHON_INTERFACE_FILENAME src/plugins/pythoninterface.cpp)
endif()

# Files that make up the GUI version of OpenCOR

set(SOURCES
    src/checkforupdatesdialog.cpp
    src/generalpreferenceswidget.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/pluginsdialog.cpp
    src/preferencesdialog.cpp
    src/splashscreenwindow.cpp

    src/misc/cliapplication.cpp
    src/misc/cliutils.cpp
    src/misc/guiapplication.cpp
    src/misc/guiutils.cpp

    src/plugins/cliinterface.cpp
    src/plugins/coreinterface.cpp
    src/plugins/datastoreinterface.cpp
    src/plugins/filehandlinginterface.cpp
    src/plugins/filetypeinterface.cpp
    src/plugins/guiinterface.cpp
    src/plugins/i18ninterface.cpp
    src/plugins/plugin.cpp
    src/plugins/plugininfo.cpp
    src/plugins/plugininterface.cpp
    src/plugins/pluginmanager.cpp
    src/plugins/preferencesinterface.cpp
    ${PYTHON_INTERFACE_FILENAME}
    src/plugins/solverinterface.cpp
    src/plugins/viewinterface.cpp
    src/plugins/windowinterface.cpp
)

set(UIS
    src/checkforupdatesdialog.ui
    src/generalpreferenceswidget.ui
    src/mainwindow.ui
    src/pluginsdialog.ui
    src/preferencesdialog.ui
    src/splashscreenwindow.ui
)

set(RESOURCES
    ${COMMON_QRC_FILENAME}
    ${I18N_QRC_FILENAME}
    res/translations.qrc
    res/ui.qrc
)

# Files that make up the CLI version of OpenCOR (Windows specific)

if(WIN32)
    set(WINDOWS_CLI_SOURCES
        src/misc/cliapplication.cpp
        src/misc/cliutils.cpp

        src/plugins/cliinterface.cpp
        src/plugins/coreinterface.cpp
        src/plugins/datastoreinterface.cpp
        src/plugins/filehandlinginterface.cpp
        src/plugins/filetypeinterface.cpp
        src/plugins/plugin.cpp
        src/plugins/plugininfo.cpp
        src/plugins/plugininterface.cpp
        src/plugins/pluginmanager.cpp
        ${PYTHON_INTERFACE_FILENAME}
        src/plugins/solverinterface.cpp

        src/windows/main.cpp
    )

    set(WINDOWS_CLI_RESOURCES
        ${COMMON_QRC_FILENAME}
    )
endif()

# Various include directories
# Note #1: access to the Core plugin's source folder is needed so that we can
#          build OpenCOR on its own, i.e. without any plugins (due to the global
#          CLI/GUI utilities needing access to some features that are in common
#          with the Core CLI/GUI utilities)...
# Note #2: we want everybody to be able to access diff-match-patch...

include_directories(
    src
    src/3rdparty/diff_match_patch/src
    src/misc
    src/plugins
    src/plugins/miscellaneous/Core/src
)

# Update the translation (.ts) files and generate the language (.qm) files
# that will later be embedded in the OpenCOR executable as resources

update_language_files(${CMAKE_PROJECT_NAME} ${SOURCES} ${UIS})

# Third-party library that must be directly embedded in the GUI version of
# OpenCOR

include(${CMAKE_SOURCE_DIR}/src/3rdparty/QtSingleApplication/QtSingleApplication.cmake)

# Set the application icon, but only for Windows and macOS, since in the case of
# Linux, it's done through the use of app_icon (see res/ui.qrc) when we register
# our URL scheme (see MainWindow::registerOpencorUrlScheme())
# Note: on Windows, we set a bit more than just the application icon. We also
#       set its product name, version, copyright, etc.

set(PROJECT_DESCRIPTION "A cross-platform modelling environment")

if(SNAPSHOT)
    set(FILE_VERSION 0)
    set(PRODUCT_VERSION "Snapshot ${VERSION}")
else()
    string(REPLACE "." ","
           FILE_VERSION "${VERSION}")

    set(PRODUCT_VERSION "Version ${VERSION}")
endif()

string(TIMESTAMP YEAR "%Y")

if(WIN32)
    set(RC_FILENAME ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.rc)

    configure_file(${CMAKE_SOURCE_DIR}/res/${CMAKE_PROJECT_NAME}.rc.in ${RC_FILENAME})

    list(APPEND SOURCES ${RC_FILENAME})
elseif(APPLE)
    set(ICNS_FILENAME ${CMAKE_PROJECT_NAME}.icns)
    set(MACOSX_BUNDLE_ICON_FILE ${ICNS_FILENAME})

    set_source_files_properties(res/${ICNS_FILENAME} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    list(APPEND SOURCES res/${ICNS_FILENAME})
endif()

# Check whether tests are required and, if so, 'reset' our list of tests and set
# the destination tests directory and build our main test program

if(ENABLE_TESTS)
    # 'Reset' our list of tests

    set(TESTS_LIST_FILENAME ${PROJECT_BUILD_DIR}/tests.txt)

    file(WRITE ${TESTS_LIST_FILENAME})

    track_files(${TESTS_LIST_FILENAME})

    # Destination tests directory
    # Note: DEST_TESTS_DIR isn't only used here, but also in our add_plugin()
    #       macro...

    if(APPLE)
        set(DEST_TESTS_DIR ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS)
    else()
        set(DEST_TESTS_DIR ${PROJECT_BUILD_DIR}/bin)
    endif()

    # Build our main test program

    set(RUNTESTS_NAME runtests)

    set(TESTS_QRC_FILENAME ${PROJECT_BUILD_DIR}/src/tests/res/tests.qrc)

    configure_file(${CMAKE_SOURCE_DIR}/src/tests/res/tests.qrc.in ${TESTS_QRC_FILENAME})

    add_executable(${RUNTESTS_NAME}
        src/tests/src/main.cpp
        src/tests/src/testsutils.cpp

        ${TESTS_QRC_FILENAME}
    )

    set_target_properties(${RUNTESTS_NAME} PROPERTIES
        OUTPUT_NAME ${RUNTESTS_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    configure_clang_and_clang_tidy(${RUNTESTS_NAME})

    target_link_libraries(${RUNTESTS_NAME}
        Qt5::Core
        Qt5::Network
    )

    # Copy our main test program to our tests directory

    set(MAIN_TEST_FILENAME ${RUNTESTS_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    add_custom_command(TARGET ${RUNTESTS_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BUILD_DIR}/${MAIN_TEST_FILENAME}
                                                        ${DEST_TESTS_DIR}/${MAIN_TEST_FILENAME})

    # Clean up our test program, if we are on macOS, our make sure that it uses
    # RPATH rather than RUNPATH on Linux

    if(APPLE)
        strip_file(${RUNTESTS_NAME} ${DEST_TESTS_DIR}/${MAIN_TEST_FILENAME})
    elseif(NOT WIN32)
        runpath2rpath(${RUNTESTS_NAME} ${DEST_TESTS_DIR}/${MAIN_TEST_FILENAME})
    endif()
endif()

# Specify a target to help us build certain plugins (e.g. our Python plugin)

set(PROJECT_BUILD_TARGET ${PROJECT_NAME}Build)

add_custom_target(${PROJECT_BUILD_TARGET} ALL)

# Specify a target to help us take advantage of our documentation (e.g. by Help
# window plugin)

set(DOCUMENTATION_BUILD_TARGET DocumentationBuild)

add_custom_target(${DOCUMENTATION_BUILD_TARGET} ALL)

# Build our plugins
# Note #1: hard dependencies (i.e. OpenSSL and zlib) must be listed before any
#          other plugins, and zlib before OpenSSL since the latter needs the
#          former...
# Note #2: Python third-party plugins must be built in order, after zlib and
#          OpenSSL, and before any other plugins...
# Note #3: soft dependencies are determined automatically, so other plugins can
#          be listed in any order...

if(USE_PREBUILT_QTWEBKIT_PACKAGE)
    # We are using the prebuilt version of our QtWebKit package, so build
    # whatever we can depending on whether we are using the prebuilt version of
    # our Python package

    set(PLUGINS
        thirdParty/OpenSSL
        thirdParty/zlib

        thirdParty/Python
    )

    if(USE_PREBUILT_PYTHON_PACKAGE)
        # We are using the prebuilt version of our Python package, so build all
        # our plugins

        list(APPEND PLUGINS
            thirdParty/libSBML
            thirdParty/PythonPackages
            thirdParty/PythonQt

            dataStore/BioSignalMLDataStore
            dataStore/CSVDataStore
            dataStore/DataStore

            editing/CellMLAnnotationView
            editing/CellMLEditingView
            editing/CellMLTextView
            editing/EditingView
            editing/RawCellMLView
            editing/RawSEDMLView
            editing/RawTextView
            editing/SEDMLEditingView

            miscellaneous/Compiler
            miscellaneous/Core
            miscellaneous/HelpWindow
            miscellaneous/JupyterKernel
            miscellaneous/PythonConsoleWindow
            miscellaneous/PythonShell
            miscellaneous/WebBrowserWindow

            organisation/FileBrowserWindow
            organisation/FileOrganiserWindow
            organisation/PMRWindow
            organisation/PMRWorkspacesWindow

            simulation/SimulationExperimentView

            solver/CVODESolver
            solver/ForwardEulerSolver
            solver/FourthOrderRungeKuttaSolver
            solver/HeunSolver
            solver/KINSOLSolver
            solver/SecondOrderRungeKuttaSolver

            support/CellMLSupport
            support/COMBINESupport
            support/PMRSupport
            support/PythonQtSupport
            support/PythonSupport
            support/SEDMLSupport
            support/SimulationSupport
            support/StandardSupport
            support/ZIPSupport

            thirdParty/CellMLAPI
            thirdParty/libBioSignalML
            thirdParty/libgit2
            thirdParty/libNuML
            thirdParty/libSEDML
            thirdParty/LibXDiff
            thirdParty/LLVMClang
            thirdParty/OAuth
            thirdParty/QScintilla
            thirdParty/Qwt
            thirdParty/SUNDIALS

            tools/CellMLTools

            widget/EditorWidget
            widget/GraphPanelWidget
            widget/MathMLViewerWidget
            widget/QScintillaWidget
            widget/WebViewerWidget
        )

        # Build our sample plugins, if required

        if(ENABLE_SAMPLE_PLUGINS)
            list(APPEND PLUGINS
                sample/Sample
                sample/SampleTools
                sample/SampleView
                sample/SampleWindow
            )

            # Let OpenCOR know about the sample plugins being enabled

            add_definitions(-DENABLE_SAMPLE_PLUGINS)
        endif()

        # Build our test plugins, if required

        if(ENABLE_TEST_PLUGINS)
            list(APPEND PLUGINS
                test/ZincWindow

                thirdParty/Zinc

                widget/ZincWidget
            )

            # Let OpenCOR know about the test plugins being enabled

            add_definitions(-DENABLE_TEST_PLUGINS)
        endif()

        # Let OpenCOR know that we want Python support

        add_definitions(-DPYTHON_SUPPORT)

        # Let OpenCOR know about the prebuilt version of our QtWebKit package
        # being used

        add_definitions(-DUSE_PREBUILT_QTWEBKIT_PACKAGE)
    endif()

    # Include different directories needed by our various plugins

    foreach(PLUGIN ${PLUGINS})
        set(PLUGIN_DIR src/plugins/${PLUGIN})

        if("${PLUGIN}" MATCHES "^thirdParty/.*$")
            include_directories(${CMAKE_SOURCE_DIR}/ext/${PLUGIN_DIR}/${EXTERNAL_PACKAGE_DIR}/include)
        endif()

        include_directories(${CMAKE_SOURCE_DIR}/${PLUGIN_DIR}/src)
    endforeach()

    # Build our various plugins

    foreach(PLUGIN ${PLUGINS})
        add_subdirectory(src/plugins/${PLUGIN})
    endforeach()
endif()

# Fetch parts of our documentation and build it, if we are neither building
# Python nor QtWebKit
# Note: we need Python because we rely on Sphinx to build our documentation...

if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/doc
                                                               ${PROJECT_BUILD_DIR}/doc)

    configure_file(${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp.in
                   ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp)

    build_documentation(user)
    build_documentation(developer)
endif()

# Build the GUI version of OpenCOR

if(APPLE)
    list(APPEND SOURCES src/misc/macos.mm)
endif()

add_executable(${CMAKE_PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${RESOURCES}
)

add_definitions(-D${CMAKE_PROJECT_NAME}_MAIN)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE GUI_SUPPORT)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

configure_clang_and_clang_tidy(${CMAKE_PROJECT_NAME})

if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        ${Qt5Core_QTMAIN_LIBRARIES}
        ${PYTHON_LIBRARY}
    )
endif()

foreach(REQUIRED_QT_MODULE ${REQUIRED_QT_MODULES})
    target_link_libraries(${CMAKE_PROJECT_NAME}
        Qt5::${REQUIRED_QT_MODULE}
    )
endforeach()

# We can't compile Python interface code until after the Python headers are in
# place

if(NOT "${PYTHON_DEPENDENCIES}" STREQUAL "")
    add_dependencies(${CMAKE_PROJECT_NAME} ${PYTHON_DEPENDENCIES})
endif()

# On Linux, our Python package needs to be directly linked to OpenCOR otherwise
# Python extension DSOs can't find symbols
# Note: this is because the lookup scope changes for DSOs that are loaded
#       using dlopen()...
#       (See https://www.akkadia.org/drepper/dsohowto.pdf)

if(NOT WIN32 AND NOT APPLE)
    include_directories(${PYTHON_INCLUDE_DIR})

    target_link_libraries(${CMAKE_PROJECT_NAME}
        ${PYTHON_LIBRARY}
    )

    add_dependencies(${CMAKE_PROJECT_NAME} PythonPlugin)
endif()

# Build the CLI version of OpenCOR (Windows specific)
# Note: when it comes to WINDOWS_CLI_PROJECT_NAME, we used to have it set to
#       ${CMAKE_PROJECT_NAME}.com, but Ninja found a duplicate rule (from a copy
#       command), so now we use '_' instead...

if(WIN32)
    set(WINDOWS_CLI_PROJECT_NAME ${CMAKE_PROJECT_NAME}_com)

    add_executable(${WINDOWS_CLI_PROJECT_NAME}
        ${WINDOWS_CLI_SOURCES}
        ${WINDOWS_CLI_RESOURCES}
    )

    set_target_properties(${WINDOWS_CLI_PROJECT_NAME} PROPERTIES
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    configure_clang_and_clang_tidy(${WINDOWS_CLI_PROJECT_NAME})

    target_link_libraries(${WINDOWS_CLI_PROJECT_NAME}
        Qt5::Core
        Qt5::Network
        ${PYTHON_LIBRARY}
    )

    if(NOT "${PYTHON_DEPENDENCIES}" STREQUAL "")
        add_dependencies(${WINDOWS_CLI_PROJECT_NAME} ${PYTHON_DEPENDENCIES})
    endif()
endif()

# Some post-processing specific stuff

if(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something that can't be done using CMake (or so it seems)...

    set(INFO_PLIST_FILENAME ${PROJECT_BUILD_DIR}/Info.plist)

    configure_file(${PLATFORM_DISTRIB_DIR}/Info.plist.in ${INFO_PLIST_FILENAME})

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST_FILENAME}
    )

    # Note: ideally, what follows would be done using macdeployqt, but it has
    #       regularly been causing us problems, so instead we have decided to do
    #       everything ourselves...

    # Clean up the OpenCOR executable

    strip_file(${CMAKE_PROJECT_NAME} ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS/${CMAKE_PROJECT_NAME})

    # Qt libraries required by OpenCOR

    foreach(QT_LIBRARY ${MACOS_QT_LIBRARIES})
        macos_deploy_qt_library(Qt${QT_LIBRARY})
    endforeach()

    # Qt plugins required by OpenCOR

    macos_deploy_qt_plugins(imageformats qjpeg)
    macos_deploy_qt_plugins(mediaservice qavfmediaplayer)
    macos_deploy_qt_plugins(platforms qcocoa)
    macos_deploy_qt_plugins(printsupport cocoaprintersupport)
    macos_deploy_qt_plugins(sqldrivers qsqlite)
    macos_deploy_qt_plugins(styles qmacstyle)
else()
    # Make sure that OpenCOR uses RPATH rather than RUNPATH on Linux

    if(NOT WIN32)
        runpath2rpath(${PROJECT_NAME} ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME})
    endif()

    # Copy the GUI version of the OpenCOR executable to the build/bin folder

    copy_file_to_build_dir(${CMAKE_PROJECT_NAME} ${PROJECT_BUILD_DIR} bin
                           ${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    if(WIN32)
        # Copy the CLI version of the OpenCOR executable to both the build and
        # build/bin folders
        # Note: the idea is that, on Windows, we can test both the CLI and the
        #       GUI versions of OpenCOR from either build (the case when running
        #       OpenCOR from within Qt Creator) or build/bin (the case if we
        #       want to test OpenCOR as if it was deployed on someone's
        #       computer)...

        set(WINDOWS_CLI_COPY ${WINDOWS_CLI_PROJECT_NAME}_COPY)

        add_custom_target(${WINDOWS_CLI_COPY} ALL)

        copy_file_to_build_dir(${WINDOWS_CLI_COPY} ${PROJECT_BUILD_DIR} .
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)
        copy_file_to_build_dir(${WINDOWS_CLI_COPY} ${PROJECT_BUILD_DIR} bin
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)

        add_dependencies(${WINDOWS_CLI_COPY} ${WINDOWS_CLI_PROJECT_NAME})

        # Additional files required by OpenCOR
        # Note #1: these files may or not be needed in order to test OpenCOR
        #          locally. It all depends on the way the user's computer is set
        #          up. So, by copying them over, we are sure that the
        #          release/debug version of OpenCOR will work fine...
        # Note #2: the second set of additional files is needed in case OpenCOR
        #          is to be run on a non-up-to-date copy of Windows 7...

        if(RELEASE_MODE)
            set(MSVC_DIRNAME "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.16.27012/x64/Microsoft.VC141.CRT")
            set(DEBUG_TAG)
        else()
            set(MSVC_DIRNAME "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.16.27012/debug_nonredist/x64/Microsoft.VC141.DebugCRT")
            set(DEBUG_TAG d)
        endif()

        set(ADDITIONAL_FILES
            ${MSVC_DIRNAME}/msvcp140${DEBUG_TAG}.dll
            ${MSVC_DIRNAME}/vcruntime140${DEBUG_TAG}.dll
        )

        set(REMOTE_DEBUGGER_DIRNAME "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/Remote Debugger/x64")

        list(APPEND ADDITIONAL_FILES
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-file-l1-2-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-file-l2-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-localization-l1-2-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-processthreads-l1-1-1.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-synch-l1-2-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-core-timezone-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-conio-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-convert-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-environment-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-filesystem-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-heap-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-locale-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-math-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-runtime-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-stdio-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-string-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-time-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/api-ms-win-crt-utility-l1-1-0.dll
            ${REMOTE_DEBUGGER_DIRNAME}/ucrtbase.dll
        )

        foreach(ADDITIONAL_FILE ${ADDITIONAL_FILES})
            get_filename_component(DIRNAME ${ADDITIONAL_FILE} DIRECTORY)
            get_filename_component(FILENAME ${ADDITIONAL_FILE} NAME)

            copy_file_to_build_dir(DIRECT ${DIRNAME} . ${FILENAME})
            copy_file_to_build_dir(DIRECT ${DIRNAME} bin ${FILENAME})
        endforeach()

        # Finally, add Qt's version of the Mesa library so that we can run
        # OpenCOR on machines that don't have 'proper' OpenGL support (e.g. on a
        # Windows virtual machine in VirtualBox)

        set(ORIG_MESA_FILENAME opengl32sw.dll)
        set(DEST_MESA_FILENAME opengl32.dll)

        copy_file_to_build_dir(DIRECT ${QT_BINARY_DIR} . ${ORIG_MESA_FILENAME} ${DEST_MESA_FILENAME})
        copy_file_to_build_dir(DIRECT ${QT_BINARY_DIR} bin ${ORIG_MESA_FILENAME} ${DEST_MESA_FILENAME})
    endif()
endif()

# Package OpenCOR

set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "Team OpenCOR (team@opencor.ws)")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${DISTRIB_DIR}/readMe.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME}" "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Physiome Project")
set(CPACK_PACKAGE_VERSION "${VERSION}")

if(SNAPSHOT)
    set(CPACK_PACKAGE_VERSION_PATCH "")
else()
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
endif()

string(REPLACE "${PROJECT_BUILD_DIR}/" ""
       RELATIVE_PYTHON_SCRIPT_DIR "${PYTHON_SCRIPT_DIR}")

if(WIN32)
    # Select NSIS, if available, and ZIP as the packagers on Windows

    if(   EXISTS "C:/Program\ Files/NSIS/makensis.exe"
       OR EXISTS "C:/Program\ Files\ (x86)/NSIS/makensis.exe")
        set(NSIS_FOUND TRUE)
        set(NSIS_GENERATOR NSIS)
    else()
        set(NSIS_FOUND FALSE)
    endif()

    set(CPACK_GENERATOR ${NSIS_GENERATOR} ZIP)
    set(CPACK_SYSTEM_NAME "Windows")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    # Specify the default installation directory

    if(NSIS_FOUND)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
        set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${CMAKE_PROJECT_NAME}.exe")
    endif()

    # Install both the GUI and CLI versions of OpenCOR

    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME
            DESTINATION bin)
    install(FILES ${PROJECT_BUILD_DIR}/bin/${CMAKE_PROJECT_NAME}.com
            DESTINATION bin)

    # Additional files required by OpenCOR (incl. the Mesa library)

    install(FILES ${ADDITIONAL_FILES}
            DESTINATION bin)
    install(FILES ${QT_BINARY_DIR}/${ORIG_MESA_FILENAME}
            DESTINATION bin
            RENAME ${DEST_MESA_FILENAME})

    # Qt libraries required by OpenCOR

    foreach(QT_LIBRARY Core Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel ${WEBKIT} ${WEBKITWIDGETS} Widgets Xml XmlPatterns)
        windows_deploy_qt_library(Qt5${QT_LIBRARY})
    endforeach()

    if(USE_PREBUILT_PYTHON_AND_QTWEBKIT_PACKAGES)
        foreach(QT_LIBRARY icudt icuin icuuc)
            windows_deploy_qt_library(${QT_LIBRARY}${ICU_VERSION})
        endforeach()
    endif()

    # Qt plugins required by OpenCOR

    windows_deploy_qt_plugins(imageformats qjpeg)
    windows_deploy_qt_plugins(platforms qwindows)
    windows_deploy_qt_plugins(printsupport windowsprintersupport)
    windows_deploy_qt_plugins(sqldrivers qsqlite)
    windows_deploy_qt_plugins(styles qwindowsvistastyle)

    # Batch and VBScript files to run OpenCOR (useful when downloading a ZIPped
    # version of OpenCOR)

    set(BAT_FILENAME "${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.bat")
    set(VBS_FILENAME "${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.vbs")

    configure_file(${PLATFORM_DISTRIB_DIR}/application.bat.in ${BAT_FILENAME})
    configure_file(${PLATFORM_DISTRIB_DIR}/application.vbs.in ${VBS_FILENAME})

    install(FILES ${BAT_FILENAME}
            DESTINATION .)
    install(FILES ${VBS_FILENAME}
            DESTINATION .)

    # Python runtime libraries and utilities

    install(DIRECTORY ${PROJECT_BUILD_DIR}/Python
            DESTINATION .)

    # Batch files to setup and run Python and Jupyter

    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} ${RELATIVE_PYTHON_SCRIPT_DIR} runjupyter.bat)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} ${RELATIVE_PYTHON_SCRIPT_DIR} start_jupyter.py)

    install(FILES ${PLATFORM_DISTRIB_DIR}/runjupyter.bat
                  ${PLATFORM_DISTRIB_DIR}/start_jupyter.py
            DESTINATION ${RELATIVE_PYTHON_SCRIPT_DIR})

    set(JUPYTER_CONSOLE_FILENAME "${PROJECT_BUILD_DIR}/jupyterconsole.bat")
    set(PYTHON_SHELL_FILENAME "${PROJECT_BUILD_DIR}/pythonshell.bat")

    configure_file(${PLATFORM_DISTRIB_DIR}/jupyterconsole.bat.in ${JUPYTER_CONSOLE_FILENAME})
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyterlab.bat)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyterlab.vbs)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyternotebook.bat)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyternotebook.vbs)
    configure_file(${PLATFORM_DISTRIB_DIR}/pythonshell.bat.in ${PYTHON_SHELL_FILENAME})

    install(FILES ${JUPYTER_CONSOLE_FILENAME}
                  ${PLATFORM_DISTRIB_DIR}/jupyterlab.bat
                  ${PLATFORM_DISTRIB_DIR}/jupyterlab.vbs
                  ${PLATFORM_DISTRIB_DIR}/jupyternotebook.bat
                  ${PLATFORM_DISTRIB_DIR}/jupyternotebook.vbs
                  ${PYTHON_SHELL_FILENAME}
            DESTINATION .)

    # File type association
    # Note: the calls to SHChangeNotify are to ensure that Windows refreshes
    #       file icons (so that it is clear to the user that an extension has
    #       been (un)registered...

    if(NSIS_FOUND)
        set(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}\\\\distrib\\\\windows\\\\FileAssociation.nsh")

        set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
            \\\${RegisterExtension} \\\"\\\$INSTDIR\\\\bin\\\\${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\\\" \\\".cellml\\\" \\\"CellML File\\\"
            System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
        ")
        set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
            \\\${UnregisterExtension} \\\".cellml\\\" \\\"CellML File\\\"
            System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
        ")
    endif()
elseif(APPLE)
    # Select PackageMaker and ZIP as the packagers on macOS
    # Note: this is actually done within the package script, so that the ZIP
    #       package can be done 'properly', i.e. we don't have the full tree
    #       structure in the ZIP file...

    set(CPACK_SYSTEM_NAME "macOS")

    set(CPACK_RESOURCE_FILE_WELCOME "${PLATFORM_DISTRIB_DIR}/welcome.txt")
    set(CPACK_RESOURCE_FILE_README "${DISTRIB_DIR}/readMe.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    set(CPACK_SET_DESTDIR TRUE)

    install(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE
            DESTINATION .)

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    set(SHELL_SCRIPT_FILENAME ${PROJECT_BUILD_DIR}/application)

    configure_file(${PLATFORM_DISTRIB_DIR}/application.in ${SHELL_SCRIPT_FILENAME} @ONLY)

    install(FILES ${SHELL_SCRIPT_FILENAME}
            DESTINATION .
            RENAME ${CMAKE_PROJECT_NAME}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

    # Shell scripts to setup and run Python and Jupyter

    set(RUN_JUPYTER_FILENAME ${PYTHON_SCRIPT_DIR}/runjupyter)

    configure_file(${PLATFORM_DISTRIB_DIR}/runjupyter.in ${RUN_JUPYTER_FILENAME} @ONLY)

    set(JUPYTER_CONSOLE_FILENAME ${PROJECT_BUILD_DIR}/jupyterconsole)
    set(JUPYTER_LAB_FILENAME ${PROJECT_BUILD_DIR}/jupyterlab)
    set(JUPYTER_NOTEBOOK_FILENAME ${PROJECT_BUILD_DIR}/jupyternotebook)
    set(PYTHON_SHELL_FILENAME ${PROJECT_BUILD_DIR}/pythonshell)

    configure_file(${PLATFORM_DISTRIB_DIR}/jupyterconsole.in ${JUPYTER_CONSOLE_FILENAME} @ONLY)
    configure_file(${PLATFORM_DISTRIB_DIR}/jupyterlab.in ${JUPYTER_LAB_FILENAME} @ONLY)
    configure_file(${PLATFORM_DISTRIB_DIR}/jupyternotebook.in ${JUPYTER_NOTEBOOK_FILENAME} @ONLY)
    configure_file(${PLATFORM_DISTRIB_DIR}/pythonshell.in ${PYTHON_SHELL_FILENAME} @ONLY)

    install(FILES ${JUPYTER_CONSOLE_FILENAME}
                  ${JUPYTER_LAB_FILENAME}
                  ${JUPYTER_NOTEBOOK_FILENAME}
                  ${PYTHON_SHELL_FILENAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
else()
    # Select TGZ as the packager on Linux

    set(CPACK_GENERATOR TGZ)
    set(CPACK_SYSTEM_NAME "Linux")

    # OpenCOR itself

    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME
            DESTINATION bin)

    # Mesa library

    install(FILES ${MESA_LIBRARY}
            DESTINATION lib)

    # Library needed on Ubuntu 19.04 and later (while we build OpenCOR on Ubuntu
    # 18.04 LTS)

    foreach(LIBRARY tinfo.so.5)
        set(FULL_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})

        get_filename_component(REAL_FULL_LIBRARY /lib/x86_64-linux-gnu/${FULL_LIBRARY} REALPATH)

        install(FILES ${REAL_FULL_LIBRARY}
                DESTINATION lib
                RENAME ${FULL_LIBRARY})
    endforeach()

    # Qt libraries required by OpenCOR
    # Note: unlike for Windows and macOS, we don't have a macro for deploying
    #       all of our Qt libraries at once. Indeed, on Linux, the version of a
    #       file is appended to its suffix, so unless making the macro
    #       unnecessarily cumbersome to use, it's better to deploy one Qt
    #       library at a time...

    foreach(QT_LIBRARY Core DBus Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel ${WEBKIT} ${WEBKITWIDGETS} Widgets XcbQpa Xml XmlPatterns)
        if(   "${QT_LIBRARY}" STREQUAL "${WEBKIT}"
           OR "${QT_LIBRARY}" STREQUAL "${WEBKITWIDGETS}")
            set(REAL_QT_LIBRARY_DIR ${QTWEBKIT_LIBRARIES_DIR})
        else()
            set(REAL_QT_LIBRARY_DIR ${QT_LIBRARY_DIR})
        endif()

        linux_deploy_qt_library(DIRECT ${REAL_QT_LIBRARY_DIR} ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR})
    endforeach()

    # Qt plugins required by OpenCOR

    linux_deploy_qt_plugins(imageformats qjpeg)
    linux_deploy_qt_plugins(platforms qxcb)
    linux_deploy_qt_plugins(printsupport cupsprintersupport)
    linux_deploy_qt_plugins(sqldrivers qsqlite)
    linux_deploy_qt_plugins(xcbglintegrations qxcb-egl-integration qxcb-glx-integration)

    # Shell script to run OpenCOR

    set(SHELL_SCRIPT_FILENAME ${PROJECT_BUILD_DIR}/application)

    configure_file(${PLATFORM_DISTRIB_DIR}/application.in ${SHELL_SCRIPT_FILENAME} @ONLY)

    install(FILES ${SHELL_SCRIPT_FILENAME}
            DESTINATION .
            RENAME ${CMAKE_PROJECT_NAME}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

    # Python runtime libraries and utilities

    install(DIRECTORY ${PROJECT_BUILD_DIR}/python
            DESTINATION .
            USE_SOURCE_PERMISSIONS)

    # Shell scripts to setup and run Python and Jupyter

    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} ${RELATIVE_PYTHON_SCRIPT_DIR} runjupyter)

    install(FILES ${PLATFORM_DISTRIB_DIR}/runjupyter
            DESTINATION ${RELATIVE_PYTHON_SCRIPT_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

    set(JUPYTER_CONSOLE_FILENAME ${PROJECT_BUILD_DIR}/jupyterconsole)
    set(PYTHON_SHELL_FILENAME ${PROJECT_BUILD_DIR}/pythonshell)

    configure_file(${PLATFORM_DISTRIB_DIR}/jupyterconsole.in ${JUPYTER_CONSOLE_FILENAME} @ONLY)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyterlab)
    copy_file_to_build_dir(DIRECT ${PLATFORM_DISTRIB_DIR} . jupyternotebook)
    configure_file(${PLATFORM_DISTRIB_DIR}/pythonshell.in ${PYTHON_SHELL_FILENAME} @ONLY)

    install(FILES ${JUPYTER_CONSOLE_FILENAME}
                  ${PLATFORM_DISTRIB_DIR}/jupyterlab
                  ${PLATFORM_DISTRIB_DIR}/jupyternotebook
                  ${PYTHON_SHELL_FILENAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()

# Specify the package file name, depending on whether we are generating a
# snapshot version of OpenCOR

if(SNAPSHOT)
    set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${DATE}-${CPACK_SYSTEM_NAME})
else()
    set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${PROJECT_VERSION_MAJOR}-${PROJECT_VERSION_MINOR})

    if(NOT "${PROJECT_VERSION_PATCH}" STREQUAL "")
        set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${PROJECT_VERSION_PATCH})
    endif()

    set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME})
endif()

# Notice and license files

set(NOTICE_FILENAME ${PROJECT_BUILD_DIR}/NOTICE.txt)

configure_file(${CMAKE_SOURCE_DIR}/NOTICE.txt.in ${NOTICE_FILENAME})

set(FILES
    ${CMAKE_SOURCE_DIR}/LICENSE.txt
    ${NOTICE_FILENAME}
)

foreach(FILE ${FILES})
    install(FILES ${FILE}
            DESTINATION .
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

# Some sample CellML files

set(MODEL_FILES
    hodgkin_huxley_squid_axon_model_1952.cellml
    noble_model_1962.cellml
    van_der_pol_model_1928.cellml
)

foreach(MODEL_FILE ${MODEL_FILES})
    install(FILES ${CMAKE_SOURCE_DIR}/models/${MODEL_FILE}
            DESTINATION models
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

# Some user-defined format files

set(FORMAT_FILES
    C.xml
    F77.xml
    MATLAB.xml
    Python.xml
    README.txt
)

foreach(FORMAT_FILE ${FORMAT_FILES})
    install(FILES ${CMAKE_SOURCE_DIR}/formats/${FORMAT_FILE}
            DESTINATION formats
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

include(CPack)
