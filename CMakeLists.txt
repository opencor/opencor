CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

# Project definition

PROJECT(OpenCOR)

# Our options

OPTION(ENABLE_SAMPLE_PLUGINS "Enable the sample plugins to be built" OFF)
OPTION(ENABLE_TEST_PLUGINS "Enable the test plugins to be built" OFF)
OPTION(ENABLE_TESTS "Enable the tests to be built" OFF)

OPTION(USE_PREBUILT_MESA_PACKAGE "Use the pre-built version of the Mesa package" ON)
OPTION(USE_PREBUILT_QTWEBKIT_PACKAGE "Use the pre-built version of the QtWebKit package" ON)

OPTION(USE_PREBUILT_BIOSIGNALMLAPI_PACKAGE "Use the pre-built version of the BioSignalML API package" ON)
OPTION(USE_PREBUILT_CELLMLAPI_PACKAGE "Use the pre-built version of the CellML API package" ON)
OPTION(USE_PREBUILT_SBMLAPI_PACKAGE "Use the pre-built version of the SBML API package" ON)
OPTION(USE_PREBUILT_SEDMLAPI_PACKAGE "Use the pre-built version of the SED-ML API package" ON)

OPTION(USE_PREBUILT_LIBGIT2_PACKAGE "Use the pre-built version of the libgit2 package" ON)
OPTION(USE_PREBUILT_LIBXDIFF_PACKAGE "Use the pre-built version of the LibXDiff package" ON)
OPTION(USE_PREBUILT_LLVMCLANG_PACKAGE "Use the pre-built version of the LLVMClang plugin" ON)
OPTION(USE_PREBUILT_OAUTH_PACKAGE "Use the pre-built version of the OAuth package" ON)
OPTION(USE_PREBUILT_OPENSSL_PACKAGE "Use the pre-built version of the OpenSSL package" ON)
OPTION(USE_PREBUILT_QSCINTILLA_PACKAGE "Use the pre-built version of the QScintilla package" ON)
OPTION(USE_PREBUILT_QWT_PACKAGE "Use the pre-built version of the Qwt package" ON)
OPTION(USE_PREBUILT_SUNDIALS_PACKAGE "Use the pre-built version of the SUNDIALS package" ON)
OPTION(USE_PREBUILT_ZINC_PACKAGE "Use the pre-built version of the Zinc package" ON)
OPTION(USE_PREBUILT_ZLIB_PACKAGE "Use the pre-built version of the zlib package" ON)

# Make sure that we are using the compiler we support

IF(WIN32)
    STRING(REGEX REPLACE "\\..*$" ""
           CMAKE_CXX_COMPILER_VERSION_MAJOR "${CMAKE_CXX_COMPILER_VERSION}")

    IF(   NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"
       OR NOT CMAKE_CXX_COMPILER_VERSION_MAJOR VERSION_EQUAL "19")
        MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using MSVC 2015 on Windows...")
    ENDIF()
ELSEIF(APPLE)
    IF(    NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
       AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using (Apple) Clang on macOS...")
    ENDIF()
ELSE()
    IF(   NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
       OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
        MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built using GCC/G++ 4.9+ on Linux...")
    ENDIF()
ENDIF()

# Make sure that we are building on a supported architecture
# Note: normally, we would check the value of CMAKE_SIZEOF_VOID_P, but in some
#       cases it may not be set (e.g. when generating an Xcode project file), so
#       we determine and retrieve that value ourselves...

TRY_RUN(ARCHITECTURE_RUN ARCHITECTURE_COMPILE
        ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/cmake/architecture.c
        RUN_OUTPUT_VARIABLE ARCHITECTURE)

IF(NOT ARCHITECTURE_COMPILE)
    MESSAGE(FATAL_ERROR "We could not determine your architecture. Please clean your ${CMAKE_PROJECT_NAME} environment and try again...")
ELSEIF(NOT ${ARCHITECTURE} EQUAL 64)
    MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in 64-bit mode...")
ENDIF()

# By default, we are building a release version of OpenCOR, unless we are
# explicitly asked for a debug version

IF(   "${CMAKE_BUILD_TYPE}" STREQUAL ""
   OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    SET(BUILD_INFORMATION "Building a release version of ${CMAKE_PROJECT_NAME}")

    SET(RELEASE_MODE TRUE)
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    SET(BUILD_INFORMATION "Building a debug version of ${CMAKE_PROJECT_NAME}")

    SET(RELEASE_MODE FALSE)
ELSE()
    MESSAGE(FATAL_ERROR "${CMAKE_PROJECT_NAME} can only be built in release or debug mode...")
ENDIF()

# Try to build our runpath2rpath program, if we are on Linux

IF(NOT WIN32 AND NOT APPLE)
    SET(RUNPATH2RPATH ${CMAKE_BINARY_DIR}/runpath2rpath)

    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -o ${RUNPATH2RPATH} ${PROJECT_SOURCE_DIR}/cmake/runpath2rpath.c
                    RESULT_VARIABLE RESULT)

    IF(NOT RESULT EQUAL 0)
        MESSAGE(FATAL_ERROR "runpath2rpath could not be built...")
    ENDIF()
ENDIF()

# Keep track of some basic information about Qt

FIND_PACKAGE(Qt5Core REQUIRED)

SET(QT_VERSION ${Qt5Core_VERSION})
SET(QT_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR})

SET(QT_DIR ${_qt5Core_install_prefix})
SET(QT_BINARY_DIR ${QT_DIR}/bin)
SET(QT_LIBRARY_DIR ${QT_DIR}/lib)
SET(QT_PLUGINS_DIR ${QT_DIR}/plugins)

GET_TARGET_PROPERTY(QMAKE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)

SET(QMAKE_COMMAND ${QMAKE} -Wnone)

STRING(REPLACE ";" " " QMAKE_COMMAND_AS_STRING "${QMAKE_COMMAND}")

# Determine our platform directory

IF(WIN32)
    SET(PLATFORM windows)

    IF(RELEASE_MODE)
        SET(TARGET_PLATFORM ${PLATFORM}.release)
    ELSE()
        SET(TARGET_PLATFORM ${PLATFORM}.debug)
    ENDIF()
ELSE()
    IF(APPLE)
        SET(PLATFORM macos)
    ELSE()
        SET(PLATFORM linux)
    ENDIF()

    SET(TARGET_PLATFORM ${PLATFORM})
ENDIF()

SET(PLATFORM_DIR ${PLATFORM})

# Determine the effective build directory

SET(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR})

IF(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    # With Xcode, we have a configuration directory, but it messes up our build
    # system, so ask for all the binaries to be generated in our build folder

    SET(XCODE TRUE)

    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})

    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BUILD_DIR})

    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BUILD_DIR})
ELSE()
    # Check whether there is a configuration directory (the case with MSVC) and,
    # if so, make use of it

    SET(XCODE FALSE)

    IF(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
        SET(PROJECT_BUILD_DIR ${PROJECT_BUILD_DIR}/${CMAKE_CFG_INTDIR})
    ENDIF()
ENDIF()

# Default location of external binaries and packages

IF(WIN32)
    IF(RELEASE_MODE)
        SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR}/release)
        SET(LOCAL_EXTERNAL_BINARIES_DIR bin/release)

        SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR}/release)
        SET(LOCAL_EXTERNAL_PACKAGE_DIR ext/release)
    ELSE()
        SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR}/debug)
        SET(LOCAL_EXTERNAL_BINARIES_DIR bin/debug)

        SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR}/debug)
        SET(LOCAL_EXTERNAL_PACKAGE_DIR ext/debug)
    ENDIF()

    SET(DEST_EXTERNAL_BINARIES_DIR bin)
ELSE()
    SET(REMOTE_EXTERNAL_BINARIES_DIR ${PLATFORM_DIR})
    SET(LOCAL_EXTERNAL_BINARIES_DIR bin)

    SET(REMOTE_EXTERNAL_PACKAGE_DIR ${PLATFORM_DIR})
    SET(LOCAL_EXTERNAL_PACKAGE_DIR ext)

    IF(APPLE)
        SET(DEST_EXTERNAL_BINARIES_DIR ${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)
    ELSE()
        SET(DEST_EXTERNAL_BINARIES_DIR lib)
    ENDIF()
ENDIF()

SET(FULL_DEST_EXTERNAL_BINARIES_DIR ${PROJECT_BUILD_DIR}/${DEST_EXTERNAL_BINARIES_DIR})

IF(NOT EXISTS ${FULL_DEST_EXTERNAL_BINARIES_DIR})
    FILE(MAKE_DIRECTORY ${FULL_DEST_EXTERNAL_BINARIES_DIR})
ENDIF()

IF(APPLE)
    SET(FULL_DEST_EXTERNAL_BASE_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR})
ELSE()
    SET(FULL_DEST_EXTERNAL_BASE_DIR ${PROJECT_BUILD_DIR})
ENDIF()

# Some in-house CMake macros

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Make sure that we can use the ExternalProject module, and let it know where we
# want to build our external packages
# Note: indeed, otherwise on Windows we may end up with path names that are too
#       long...

INCLUDE(ExternalProject)

SET(EXTERNAL_PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR}/ext)

SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${EXTERNAL_PROJECT_BUILD_DIR})

# A few constants to help with building an external project

IF(APPLE)
    SET(DEFINE_CMAKE_APPLE_OPTIONS "-DCMAKE_MACOSX_RPATH=ON")
ELSE()
    SET(DEFINE_CMAKE_APPLE_OPTIONS)
ENDIF()

IF(WIN32)
    SET(MAKE_NMAKE_COMMAND nmake)
    SET(MAKE_JOM_COMMAND jom)
ELSE()
    SET(MAKE_NMAKE_COMMAND make)
    SET(MAKE_JOM_COMMAND ${MAKE_NMAKE_COMMAND})

    INCLUDE(ProcessorCount)

    ProcessorCount(PROCESSOR_COUNT)

    IF(NOT PROCESSOR_COUNT EQUAL 0)
        SET(MAKE_NMAKE_COMMAND ${MAKE_NMAKE_COMMAND} -j ${PROCESSOR_COUNT})
        SET(MAKE_JOM_COMMAND ${MAKE_JOM_COMMAND} -j ${PROCESSOR_COUNT})
    ENDIF()
ENDIF()

STRING(REPLACE ";" " " MAKE_NMAKE_COMMAND_AS_STRING "${MAKE_NMAKE_COMMAND}")
STRING(REPLACE ";" " " MAKE_JOM_COMMAND_AS_STRING "${MAKE_JOM_COMMAND}")

# A couple of variables that make it easier to specify library file names with a
# version (e.g. to be able to reference libz.so.1 and libz.1.dylib, we could
# simply use libz${PRE}.1${POST})

IF(APPLE)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX_PRE)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX_POST ${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSEIF(NOT WIN32)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX_PRE ${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CMAKE_SHARED_LIBRARY_SUFFIX_POST)
ENDIF()

# Version of ICU that we are using on Windows

IF(WIN32)
    SET(ICU_VERSION 57)
ENDIF()

# Retrieve or build our copy of QtWebKit

ADD_SUBDIRECTORY(src/3rdparty/QtWebKit)

# Required Qt modules

IF(ENABLE_TESTS)
    SET(TEST Test)
ELSE()
    SET(TEST)
ENDIF()

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    SET(WEBKIT WebKit)
    SET(WEBKITWIDGETS WebKitWidgets)
ELSE()
    SET(WEBKIT)
    SET(WEBKITWIDGETS)
ENDIF()

SET(REQUIRED_QT_MODULES
    Network
    ${TEST}
    Widgets
)

IF(WIN32)
    LIST(APPEND REQUIRED_QT_MODULES ${WEBKIT})
ENDIF()

FOREACH(REQUIRED_QT_MODULE ${REQUIRED_QT_MODULES})
    FIND_PACKAGE(Qt5${REQUIRED_QT_MODULE} REQUIRED)
ENDFOREACH()

# Make sure that anyone can access diff-match-patch

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/3rdparty/diff_match_patch/src)

# On macOS, keep track of the Qt libraries against which we need to link

IF(APPLE)
    IF(ENABLE_TESTS)
        SET(QTTEST QtTest)
    ELSE()
        SET(QTTEST)
    ENDIF()

    IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
        SET(QTWEBKIT QtWebKit)
        SET(QTWEBKITWIDGETS QtWebKitWidgets)
    ELSE()
        SET(QTWEBKIT)
        SET(QTWEBKITWIDGETS)
    ENDIF()

    SET(MACOS_QT_LIBRARIES
        QtCLucene
        QtConcurrent
        QtCore
        QtGui
        QtHelp
        QtMacExtras
        QtMultimedia
        QtMultimediaWidgets
        QtNetwork
        QtOpenGL
        QtPositioning
        QtPrintSupport
        QtQml
        QtQuick
        QtSensors
        QtSql
        QtSvg
        ${QTTEST}
        QtWebChannel
        ${QTWEBKIT}
        ${QTWEBKITWIDGETS}
        QtWidgets
        QtXml
        QtXmlPatterns
    )
ENDIF()

# Some general build settings
# Note: MSVC enables C++11 support by default, so we just need to enable it on
#       Linux and macOS...

IF(WIN32)
    SET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /WX /GR /EHsc")
    # Note: MSVC has a /Wall flag, but it results in MSVC being very pedantic,
    #       so instead we use what MSVC recommends for production code, which is
    #       /W3 and which is also what CMake uses by default...

    SET(LINK_FLAGS_PROPERTIES "/STACK:10000000")
ELSE()
    SET(CMAKE_CXX_FLAGS "-Wall -W -Werror -std=c++11")

    IF(APPLE)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        SET(LINK_FLAGS_PROPERTIES "-stdlib=libc++")
    ELSE()
        SET(LINK_FLAGS_PROPERTIES)
    ENDIF()
ENDIF()

# On macOS, we want to be able to access Cocoa

IF(APPLE)
    SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -framework AppKit")
ENDIF()

# Some build settings that depend on whether we want a release or a debug
# version of OpenCOR

IF(RELEASE_MODE)
    # Default release compiler and linker settings

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNDEBUG /MD /O2 /Ob2")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
    ENDIF()

    IF(NOT WIN32 AND NOT APPLE)
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
        # Note #1: -Wl,-s strips all the symbols, thus reducing the final size
        #          of OpenCOR or one its shared libraries...
        # Note #2: the above linking option has become obsolete on macOS...
    ENDIF()

    # Make sure that debugging is disabled in Qt

    ADD_DEFINITIONS(-DQT_NO_DEBUG)
ELSE()
    # Default debug compiler and linker settings

    IF(WIN32)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
        SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} /DEBUG")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    ENDIF()

    # Make sure that debugging is enabled in Qt

    ADD_DEFINITIONS(-DQT_DEBUG)
ENDIF()

# Ask for Qt deprecated uses to be reported

ADD_DEFINITIONS(-DQT_DEPRECATED_WARNINGS)

# Let OpenCOR know about some of the options with which it was built

IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
    ADD_DEFINITIONS(-DUSE_PREBUILT_QTWEBKIT_PACKAGE)
ENDIF()

IF(ENABLE_SAMPLE_PLUGINS)
    ADD_DEFINITIONS(-DENABLE_SAMPLE_PLUGINS)
ENDIF()

IF(ENABLE_TEST_PLUGINS)
    ADD_DEFINITIONS(-DENABLE_TEST_PLUGINS)
ENDIF()

# On macOS, make sure that we support 10.10 and later, unless a specific
# deployment target has been specified

IF(APPLE)
    IF("${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
        SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
    ENDIF()

    IF(CMAKE_OSX_DEPLOYMENT_TARGET VERSION_LESS "10.12")
        SET(SYSTEM_NAME "OS X")
    ELSE()
        SET(SYSTEM_NAME "macOS")
    ENDIF()

    SET(BUILD_INFORMATION "${BUILD_INFORMATION} for ${SYSTEM_NAME} ${CMAKE_OSX_DEPLOYMENT_TARGET} and later")
ENDIF()

# Destination of our plugins so that we don't have to deploy OpenCOR on Windows
# and Linux before being able to test it

IF(APPLE)
    SET(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/PlugIns/${CMAKE_PROJECT_NAME})
ELSE()
    SET(DEST_PLUGINS_DIR ${PROJECT_BUILD_DIR}/plugins/${CMAKE_PROJECT_NAME})
ENDIF()

# Set the RPATH (and RPATH link, if needed) information on Linux and macOS

IF(APPLE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks;@executable_path/../PlugIns/${CMAKE_PROJECT_NAME}")
ELSEIF(NOT WIN32)
    SET(CMAKE_SKIP_RPATH TRUE)

    SET(LINK_RPATH_FLAG "-Wl,-rpath,'$ORIGIN/../lib'")
    SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-rpath-link,${QT_LIBRARY_DIR} ${LINK_RPATH_FLAG}")
ENDIF()

# Show what we are about to build

MESSAGE("${BUILD_INFORMATION} using Qt ${QT_VERSION}...")

# Keep track of our source and build directories (needed to run our tests)

SET(SOURCE_DIRECTORY_FILENAME ${PROJECT_BUILD_DIR}/sourcedirectory.txt)
SET(BUILD_DIRECTORY_FILENAME ${PROJECT_BUILD_DIR}/builddirectory.txt)

FILE(WRITE ${SOURCE_DIRECTORY_FILENAME} "${CMAKE_SOURCE_DIR}")
FILE(WRITE ${BUILD_DIRECTORY_FILENAME} "${PROJECT_BUILD_DIR}")

KEEP_TRACK_OF_FILE(${SOURCE_DIRECTORY_FILENAME})
KEEP_TRACK_OF_FILE(${BUILD_DIRECTORY_FILENAME})

# Version/snapshot of OpenCOR

IF("${PROJECT_VERSION}" STREQUAL "")
    SET(SNAPSHOT ON)
ELSE()
    SET(SNAPSHOT OFF)
ENDIF()

STRING(TIMESTAMP DATE "%Y-%m-%d")

IF(SNAPSHOT)
    SET(VERSION ${DATE})
ELSE()
    SET(VERSION ${PROJECT_VERSION})
ENDIF()

SET(VERSION_DATE_FILENAME ${PROJECT_BUILD_DIR}/versiondate.txt)

FILE(WRITE ${VERSION_DATE_FILENAME} "${VERSION}\n${DATE}")

KEEP_TRACK_OF_FILE(${VERSION_DATE_FILENAME})

# Configure our help file

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp.in
               ${PROJECT_BUILD_DIR}/doc/${CMAKE_PROJECT_NAME}.qhcp)

# Configure our QRC files

SET(COMMON_QRC_FILENAME ${PROJECT_BUILD_DIR}/res/common.qrc)
SET(I18N_QRC_FILENAME ${PROJECT_BUILD_DIR}/res/i18n.qrc)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/res/common.qrc.in
               ${COMMON_QRC_FILENAME})
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/res/i18n.qrc.in
               ${I18N_QRC_FILENAME})

# Files that make up the GUI version of OpenCOR

SET(SOURCES
    src/checkforupdatesdialog.cpp
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
    src/plugins/solverinterface.cpp
    src/plugins/viewinterface.cpp
    src/plugins/windowinterface.cpp
)

SET(HEADERS_MOC
    src/checkforupdatesdialog.h
    src/mainwindow.h
    src/pluginsdialog.h
    src/preferencesdialog.h
    src/splashscreenwindow.h

    src/misc/cliutils.h
    src/misc/guiapplication.h
    src/misc/guiutils.h

    src/plugins/datastoreinterface.h
    src/plugins/plugin.h
    src/plugins/pluginmanager.h
    src/plugins/preferencesinterface.h
    src/plugins/solverinterface.h
)

SET(UIS
    src/checkforupdatesdialog.ui
    src/mainwindow.ui
    src/pluginsdialog.ui
    src/preferencesdialog.ui
    src/splashscreenwindow.ui
)

SET(RESOURCES
    ${COMMON_QRC_FILENAME}
    ${I18N_QRC_FILENAME}
    res/translations.qrc
    res/ui.qrc
)

# Files that make up the CLI version of OpenCOR (Windows specific)

IF(WIN32)
    SET(WINDOWS_CLI_SOURCES
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
        src/plugins/solverinterface.cpp

        src/windows/main.cpp
    )

    SET(WINDOWS_CLI_HEADERS_MOC
        src/misc/cliutils.h

        src/plugins/datastoreinterface.h
        src/plugins/plugin.h
        src/plugins/pluginmanager.h
        src/plugins/solverinterface.h
    )

    SET(WINDOWS_CLI_RESOURCES
        ${COMMON_QRC_FILENAME}
    )
ENDIF()

# Various include directories
# Note: access to the Core plugin's source folder is needed so that we can build
#       OpenCOR on its own, i.e. without any plugins (due to the global CLI/GUI
#       utilities needing access to some features that are in common with the
#       Core CLI/GUI utilities)...

INCLUDE_DIRECTORIES(
    src/misc
    src/plugins
    src/plugins/miscellaneous/Core/src
)

# Update the translation (.ts) files and generate the language (.qm) files
# that will later on be embedded in the OpenCOR executable as resources

UPDATE_LANGUAGE_FILES(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS_MOC} ${UIS})

# Third-party library that must be directly embedded in the GUI version of
# OpenCOR

INCLUDE(${CMAKE_SOURCE_DIR}/src/3rdparty/QtSingleApplication/QtSingleApplication.cmake)

# Set the application icon, but only for Windows and macOS, since in the case of
# Linux, it's done through the use of app_icon (see res/ui.qrc) when we register
# our URL scheme (see MainWindow::registerOpencorUrlScheme())
# Note: on Windows, we set a bit more than just the application icon. We also
#       set its product name, version, copyright, etc.

SET(PROJECT_DESCRIPTION "A cross-platform modelling environment")

IF(SNAPSHOT)
    SET(FILE_VERSION 0)
    SET(PRODUCT_VERSION "Snapshot ${VERSION}")
ELSE()
    STRING(REPLACE "." ","
           FILE_VERSION "${VERSION}")

    SET(PRODUCT_VERSION "Version ${VERSION}")
ENDIF()

STRING(TIMESTAMP YEAR "%Y")

IF(WIN32)
    SET(RC_FILENAME ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.rc)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/res/${CMAKE_PROJECT_NAME}.rc.in
                   ${RC_FILENAME})

    LIST(APPEND SOURCES ${RC_FILENAME})
ELSEIF(APPLE)
    SET(ICNS_FILENAME ${CMAKE_PROJECT_NAME}.icns)
    SET(MACOSX_BUNDLE_ICON_FILE ${ICNS_FILENAME})

    SET_SOURCE_FILES_PROPERTIES(res/${ICNS_FILENAME} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    LIST(APPEND SOURCES res/${ICNS_FILENAME})
ENDIF()

# Check whether tests are required and, if so, 'reset' our list of tests and set
# the destination tests directory and build our main test program

IF(ENABLE_TESTS)
    # 'Reset' our list of tests

    SET(TESTS_LIST_FILENAME ${PROJECT_BUILD_DIR}/tests.txt)

    FILE(WRITE ${TESTS_LIST_FILENAME})

    KEEP_TRACK_OF_FILE(${TESTS_LIST_FILENAME})

    # Destination tests directory
    # Note: DEST_TESTS_DIR isn't only used here, but also in our ADD_PLUGIN()
    #       macro...

    IF(APPLE)
        SET(DEST_TESTS_DIR ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS)
    ELSE()
        SET(DEST_TESTS_DIR ${PROJECT_BUILD_DIR}/bin)
    ENDIF()

    # Build our main test program

    SET(RUNTESTS_NAME runtests)

    SET(TESTS_QRC_FILENAME ${PROJECT_BUILD_DIR}/src/tests/res/tests.qrc)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/tests/res/tests.qrc.in
                   ${TESTS_QRC_FILENAME})

    QT5_ADD_RESOURCES(RUNTESTS_SOURCES_RCS ${TESTS_QRC_FILENAME})

    ADD_EXECUTABLE(${RUNTESTS_NAME}
        src/tests/src/main.cpp
        src/tests/src/testsutils.cpp

        ${RUNTESTS_SOURCES_RCS}
    )

    SET_TARGET_PROPERTIES(${RUNTESTS_NAME} PROPERTIES
        OUTPUT_NAME ${RUNTESTS_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    TARGET_LINK_LIBRARIES(${RUNTESTS_NAME}
        Qt5::Core
        Qt5::Network
    )

    # Copy our main test program to our tests directory

    SET(MAIN_TEST_FILENAME ${RUNTESTS_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${RUNTESTS_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BUILD_DIR}/${MAIN_TEST_FILENAME}
                                                        ${DEST_TESTS_DIR}/${MAIN_TEST_FILENAME})

    # Clean up our test program, if we are on macOS

    IF(APPLE)
        MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${RUNTESTS_NAME} ${DEST_TESTS_DIR} ${MAIN_TEST_FILENAME})
    ENDIF()
ENDIF()

# Build our plugins
# Note: plugins can be listed in any order (dependencies are determined
#       automatically). However, for this to work, all plugins must have
#       [PLUGIN_DIR]/src as an include directory. In addition, all plugins
#       linked to a packge (built using the ExternalProject module) must also
#       have [PLUGIN_DIR]/[LOCAL_EXTERNAL_PACKAGE_DIR]/include as an include
#       directory...

IF(ENABLE_SAMPLE_PLUGINS)
    SET(SAMPLE_PLUGINS
        sample/Sample
        sample/SampleTools
        sample/SampleView
        sample/SampleWindow
    )
ELSE()
    SET(SAMPLE_PLUGINS)
ENDIF()

IF(ENABLE_TEST_PLUGINS)
    SET(TEST_PLUGINS
        test/ZincWindow
    )
ELSE()
    SET(TEST_PLUGINS)
ENDIF()

SET(PLUGINS
    ${SAMPLE_PLUGINS}
    ${TEST_PLUGINS}

    api/BioSignalMLAPI
    api/CellMLAPI
    api/SBMLAPI
    api/SEDMLAPI

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
    solver/IDASolver
    solver/KINSOLSolver
    solver/SecondOrderRungeKuttaSolver

    support/CellMLSupport
    support/COMBINESupport
    support/PMRSupport
    support/QScintillaSupport
    support/SEDMLSupport
    support/SimulationSupport
    support/StandardSupport
    support/ZIPSupport

    thirdParty/libgit2
    thirdParty/LibXDiff
    thirdParty/LLVMClang
    thirdParty/OAuth
    thirdParty/OpenSSL
    thirdParty/QScintilla
    thirdParty/Qwt
    thirdParty/SUNDIALS
    thirdParty/Zinc
    thirdParty/zlib

    tools/CellMLTools

    widget/EditorWidget
    widget/GraphPanelWidget
    widget/MathMLViewerWidget
    widget/WebViewerWidget
    widget/ZincWidget
)

IF(NOT USE_PREBUILT_QTWEBKIT_PACKAGE)
    # We want to build our copy of the QtWebKit package, so don't build any
    # plugins since some of them may require access to QtWebKit

    SET(PLUGINS)
ENDIF()

FOREACH(PLUGIN ${PLUGINS})
    SET(PLUGIN_DIR src/plugins/${PLUGIN})

    IF(   "${PLUGIN}" MATCHES "^api/.*$"
       OR "${PLUGIN}" MATCHES "^thirdParty/.*$")
        INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/${PLUGIN_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR}/include)
    ENDIF()

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/${PLUGIN_DIR}/src)
    INCLUDE_DIRECTORIES(${PROJECT_BUILD_DIR}/${PLUGIN_DIR})
ENDFOREACH()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FOREACH(PLUGIN ${PLUGINS})
    ADD_SUBDIRECTORY(src/plugins/${PLUGIN})
ENDFOREACH()

# Build the GUI version of OpenCOR

QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT5_WRAP_UI(SOURCES_UIS ${UIS})
QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

IF(APPLE)
    LIST(APPEND SOURCES
        src/misc/macos.mm
    )
ENDIF()

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_UIS}
    ${SOURCES_RCS}
)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

IF(WIN32)
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}
        ${Qt5Core_QTMAIN_LIBRARIES}
    )
ENDIF()

FOREACH(REQUIRED_QT_MODULE ${REQUIRED_QT_MODULES})
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}
        Qt5::${REQUIRED_QT_MODULE}
    )
ENDFOREACH()

# Build the CLI version of OpenCOR (Windows specific)
# Note: when it comes to WINDOWS_CLI_PROJECT_NAME, we used to have it set to
#       ${CMAKE_PROJECT_NAME}.com, but Ninja found a duplicate rule (from a copy
#       command), so now we use '_' instead of '.'...

IF(WIN32)
    QT5_WRAP_CPP(WINDOWS_CLI_SOURCES_MOC ${WINDOWS_CLI_HEADERS_MOC})
    QT5_ADD_RESOURCES(WINDOWS_CLI_SOURCES_RCS ${WINDOWS_CLI_RESOURCES})

    SET(WINDOWS_CLI_PROJECT_NAME ${CMAKE_PROJECT_NAME}_com)

    ADD_EXECUTABLE(${WINDOWS_CLI_PROJECT_NAME}
        ${WINDOWS_CLI_SOURCES}
        ${WINDOWS_CLI_SOURCES_MOC}
        ${WINDOWS_CLI_SOURCES_RCS}
    )

    TARGET_COMPILE_DEFINITIONS(${WINDOWS_CLI_PROJECT_NAME}
                               PRIVATE CLI_VERSION)

    SET_TARGET_PROPERTIES(${WINDOWS_CLI_PROJECT_NAME} PROPERTIES
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    TARGET_LINK_LIBRARIES(${WINDOWS_CLI_PROJECT_NAME}
        Qt5::Core
        Qt5::Network
    )
ENDIF()

# Additional compiler and linker settings

ADD_DEFINITIONS(-D${CMAKE_PROJECT_NAME}_MAIN)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

IF(WIN32)
    SET_TARGET_PROPERTIES(${WINDOWS_CLI_PROJECT_NAME} PROPERTIES
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )
ENDIF()

# Some post-processing specific stuff

IF(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something that can't be done using CMake (or so it seems)...

    SET(INFO_PLIST_FILENAME ${PROJECT_BUILD_DIR}/Info.plist)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/macos/Info.plist.in
                   ${INFO_PLIST_FILENAME})

    SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST_FILENAME}
    )

    # Note: ideally, what follows would be done using macdeployqt, but it has
    #       regularly been causing us problems, so instead we have decided to do
    #       everything ourselves...

    # Clean up the OpenCOR executable

    MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${CMAKE_PROJECT_NAME} ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS ${CMAKE_PROJECT_NAME})

    # Deploy our Qt libraries
    # Note: the Qt Brew formula that we use on Travis CI somehow results in some
    #       of our tests generating messages about two frameworks being
    #       available and that one of them will be used, but that we don't know
    #       which one. So, to avoid those messages, we simply don't deploy the
    #       Qt frameworks...

    IF(NOT ENABLE_TRAVIS_CI)
        FOREACH(QT_LIBRARY ${MACOS_QT_LIBRARIES})
            MACOS_DEPLOY_QT_LIBRARY(${QT_LIBRARY})
        ENDFOREACH()
    ENDIF()

    # Deploy some plugins

    MACOS_DEPLOY_QT_PLUGIN(imageformats qjpeg)
    MACOS_DEPLOY_QT_PLUGIN(platforms qcocoa)
    MACOS_DEPLOY_QT_PLUGIN(printsupport cocoaprintersupport)
    MACOS_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)
ELSE()
    # Copy the GUI version of the OpenCOR executable to the build/bin folder

    COPY_FILE_TO_BUILD_DIR(${CMAKE_PROJECT_NAME} ${PROJECT_BUILD_DIR} bin
                           ${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    IF(WIN32)
        # Copy the CLI version of the OpenCOR executable to both the build and
        # build/bin folders
        # Note: the idea is that, on Windows, we can test both the CLI and the
        #       GUI versions of OpenCOR from either build (the case when running
        #       OpenCOR from within Qt Creator) or build/bin (the case if we
        #       want to test OpenCOR as if it was deployed on someone's
        #       computer)...

        SET(WINDOWS_CLI_COPY ${WINDOWS_CLI_PROJECT_NAME}_COPY)

        ADD_CUSTOM_TARGET(${WINDOWS_CLI_COPY} ALL)

        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_COPY} ${PROJECT_BUILD_DIR} .
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)
        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_COPY} ${PROJECT_BUILD_DIR} bin
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)

        ADD_DEPENDENCIES(${WINDOWS_CLI_COPY} ${WINDOWS_CLI_PROJECT_NAME})

        # Additional files required by OpenCOR
        # Note: these files may or not be needed in order to test OpenCOR
        #       locally. It all depends on the way the user's computer is set
        #       up. So, by copying them over, we are sure that the release/debug
        #       version of OpenCOR will work fine...

        SET(MSVC_DIRNAME "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist")

        IF(NOT RELEASE_MODE)
            SET(MSVC_DIRNAME "${MSVC_DIRNAME}/Debug_NonRedist")
        ENDIF()

        SET(MSVC_DIRNAME "${MSVC_DIRNAME}/x64")

        IF(RELEASE_MODE)
            SET(ADDITIONAL_FILES
                ${MSVC_DIRNAME}/Microsoft.VC140.CRT/msvcp140.dll
                ${MSVC_DIRNAME}/Microsoft.VC140.CRT/vcruntime140.dll
            )
        ELSE()
            SET(ADDITIONAL_FILES
                ${MSVC_DIRNAME}/Microsoft.VC140.DebugCRT/msvcp140d.dll
                ${MSVC_DIRNAME}/Microsoft.VC140.DebugCRT/vcruntime140d.dll
            )
        ENDIF()

        SET(WINDOWS_KIT_DIRNAME "C:/Program Files (x86)/Windows Kits/10/Redist/ucrt/DLLs/x64")

        LIST(APPEND ADDITIONAL_FILES
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-file-l1-2-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-file-l2-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-localization-l1-2-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-processthreads-l1-1-1.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-synch-l1-2-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-core-timezone-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-convert-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-environment-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-filesystem-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-heap-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-locale-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-math-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-multibyte-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-runtime-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-stdio-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-string-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-time-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/api-ms-win-crt-utility-l1-1-0.dll
            ${WINDOWS_KIT_DIRNAME}/ucrtbase.dll
        )

        FOREACH(ADDITIONAL_FILE ${ADDITIONAL_FILES})
            GET_FILENAME_COMPONENT(DIRNAME ${ADDITIONAL_FILE} DIRECTORY)
            GET_FILENAME_COMPONENT(FILENAME ${ADDITIONAL_FILE} NAME)

            COPY_FILE_TO_BUILD_DIR(DIRECT ${DIRNAME} . ${FILENAME})
            COPY_FILE_TO_BUILD_DIR(DIRECT ${DIRNAME} bin ${FILENAME})
        ENDFOREACH()

        # Finally, add Qt's version of the Mesa library so that we can run
        # OpenCOR on machines that don't have 'proper' OpenGL support (e.g. on a
        # Windows virtual machine in VirtualBox)

        SET(ORIG_MESA_FILENAME opengl32sw.dll)
        SET(DEST_MESA_FILENAME opengl32.dll)

        COPY_FILE_TO_BUILD_DIR(DIRECT ${QT_BINARY_DIR} . ${ORIG_MESA_FILENAME} ${DEST_MESA_FILENAME})
        COPY_FILE_TO_BUILD_DIR(DIRECT ${QT_BINARY_DIR} bin ${ORIG_MESA_FILENAME} ${DEST_MESA_FILENAME})
    ELSE()
        # Add our copy of the Mesa library so that we can run OpenCOR on
        # machines that don't have 'proper' OpenGL support (e.g. on a Linux
        # virtual machine in VirtualBox)

        ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/distrib/mesa)
    ENDIF()
ENDIF()

# Package OpenCOR

SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Team OpenCOR (team@opencor.ws)")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME}" "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VENDOR "Physiome Project")
SET(CPACK_PACKAGE_VERSION "${VERSION}")

IF(SNAPSHOT)
    SET(CPACK_PACKAGE_VERSION_PATCH "")
ELSE()
    SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
ENDIF()

IF(WIN32)
    # Select NSIS, if available, and ZIP as the packagers on Windows

    IF(   EXISTS "C:/Program\ Files/NSIS/makensis.exe"
       OR EXISTS "C:/Program\ Files\ (x86)/NSIS/makensis.exe")
        SET(NSIS_FOUND TRUE)
        SET(NSIS_GENERATOR NSIS)
    ELSE()
        SET(NSIS_FOUND FALSE)
        SET(NSIS_GENERATOR)
    ENDIF()

    SET(CPACK_GENERATOR ${NSIS_GENERATOR} ZIP)
    SET(CPACK_SYSTEM_NAME "Windows")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    # Specify the default installation directory

    IF(NSIS_FOUND)
        SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
        SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${CMAKE_PROJECT_NAME}.exe")
    ENDIF()

    # Install both the GUI and CLI versions of OpenCOR

    INSTALL(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
    INSTALL(FILES ${PROJECT_BUILD_DIR}/bin/${CMAKE_PROJECT_NAME}.com
            DESTINATION bin)

    # Additional files required by OpenCOR (incl. the Mesa library)

    INSTALL(FILES ${ADDITIONAL_FILES}
            DESTINATION bin)

    INSTALL(FILES ${QT_BINARY_DIR}/${ORIG_MESA_FILENAME}
            DESTINATION bin
            RENAME ${DEST_MESA_FILENAME})

    # Qt libraries required by OpenCOR

    FOREACH(QT_LIBRARY CLucene Core Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel ${WEBKIT} ${WEBKITWIDGETS} Widgets Xml XmlPatterns)
        WINDOWS_DEPLOY_QT_LIBRARY(Qt5${QT_LIBRARY})
    ENDFOREACH()

    IF(USE_PREBUILT_QTWEBKIT_PACKAGE)
        FOREACH(QT_LIBRARY icudt icuin icuuc)
            WINDOWS_DEPLOY_QT_LIBRARY(${QT_LIBRARY}${ICU_VERSION})
        ENDFOREACH()
    ENDIF()

    # Qt plugins required by OpenCOR

    WINDOWS_DEPLOY_QT_PLUGIN(imageformats qjpeg)
    WINDOWS_DEPLOY_QT_PLUGIN(platforms qwindows)
    WINDOWS_DEPLOY_QT_PLUGIN(printsupport windowsprintersupport)
    WINDOWS_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    # Batch and VBScript files to run OpenCOR (useful when downloading a ZIPped
    # version of OpenCOR)

    SET(BAT_FILENAME "${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.bat")
    SET(VBS_FILENAME "${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.vbs")

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.bat.in
                   ${BAT_FILENAME})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.vbs.in
                   ${VBS_FILENAME})

    INSTALL(FILES ${BAT_FILENAME}
            DESTINATION .)
    INSTALL(FILES ${VBS_FILENAME}
            DESTINATION .)

    # File type association
    # Note: the calls to SHChangeNotify are to ensure that Windows refreshes
    #       file icons (so that it is clear to the user that an extension has
    #       been (un)registered...

    IF(NSIS_FOUND)
        SET(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}\\\\distrib\\\\windows\\\\FileAssociation.nsh")

        SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
            \\\${RegisterExtension} \\\"\\\$INSTDIR\\\\bin\\\\${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\\\" \\\".cellml\\\" \\\"CellML File\\\"
            System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
        ")
        SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
            \\\${UnregisterExtension} \\\".cellml\\\" \\\"CellML File\\\"
            System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
        ")
    ENDIF()
ELSEIF(APPLE)
    # Select PackageMaker and ZIP as the packagers on macOS
    # Note: this is actually done within the package script, so that the ZIP
    #       package can be done 'properly', i.e. we don't have the full tree
    #       structure in the ZIP file...

    SET(CPACK_SYSTEM_NAME "macOS")

    SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/distrib/macos/welcome.txt")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    SET(CPACK_SET_DESTDIR TRUE)

    INSTALL(TARGETS ${CMAKE_PROJECT_NAME} BUNDLE
            DESTINATION .)

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    SET(SH_FILENAME ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/macos/application.sh.in
                   ${SH_FILENAME})

    INSTALL(FILES ${SH_FILENAME}
            DESTINATION .
            RENAME ${CMAKE_PROJECT_NAME}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)
    SET(CPACK_SYSTEM_NAME "Linux")

    # OpenCOR itself

    INSTALL(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

    # Libraries needed on (at least) Ubuntu 16.04 LTS, but not on Ubuntu 14.04
    # LTS

    FOREACH(LIBRARY app base interfaces pbutils reamer video)
        SET(FULL_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}gst${LIBRARY}-0.10${CMAKE_SHARED_LIBRARY_SUFFIX}.0)

        GET_FILENAME_COMPONENT(REAL_FULL_LIBRARY /usr/lib/x86_64-linux-gnu/${FULL_LIBRARY} REALPATH)

        INSTALL(FILES ${REAL_FULL_LIBRARY}
                DESTINATION lib
                RENAME ${FULL_LIBRARY})
    ENDFOREACH()

    # Library needed on Ubuntu 14.04 LTS, but not on Ubuntu 16.06 LTS
    # Note: this is due to our use of GCC/G++ 4.9+, which is not originally
    #       present on Ubuntu 14.04 LTS...

    SET(FULL_LIBRARY libstdc++.so.6)

    GET_FILENAME_COMPONENT(REAL_FULL_LIBRARY /usr/lib/x86_64-linux-gnu/${FULL_LIBRARY} REALPATH)

    INSTALL(FILES ${REAL_FULL_LIBRARY}
            DESTINATION lib
            RENAME ${FULL_LIBRARY})

    # Library needed on Fedora, but not on Ubuntu

    FOREACH(LIBRARY bz2.so.1.0)
        SET(FULL_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})

        GET_FILENAME_COMPONENT(REAL_FULL_LIBRARY /lib/x86_64-linux-gnu/${FULL_LIBRARY} REALPATH)

        INSTALL(FILES ${REAL_FULL_LIBRARY}
                DESTINATION lib
                RENAME ${FULL_LIBRARY})
    ENDFOREACH()

    # Mesa library

    INSTALL(FILES ${MESA_LIBRARY}
            DESTINATION lib)

    # Qt libraries required by OpenCOR
    # Note: unlike for Windows and macOS, we don't have a macro for deploying
    #       all of our Qt libraries at once. Indeed, on Linux, the version of a
    #       file is appended to its suffix, so unless making the macro
    #       unnecessarily cumbersome to use, it's better to deploy one Qt
    #       library at a time...

    FOREACH(QT_LIBRARY CLucene Core DBus Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel ${WEBKIT} ${WEBKITWIDGETS} Widgets XcbQpa Xml XmlPatterns)
        IF(   "${QT_LIBRARY}" STREQUAL "${WEBKIT}"
           OR "${QT_LIBRARY}" STREQUAL "${WEBKITWIDGETS}")
            SET(REAL_QT_LIBRARY_DIR ${QTWEBKIT_LIBRARIES_DIR})
        ELSE()
            SET(REAL_QT_LIBRARY_DIR ${QT_LIBRARY_DIR})
        ENDIF()

        LINUX_DEPLOY_QT_LIBRARY(${REAL_QT_LIBRARY_DIR} ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR})
    ENDFOREACH()

    IF(NOT ENABLE_TRAVIS_CI)
        # We are building OpenCOR locally, so we can deploy the ICU libraries
        # Note: indeed, on Travis CI, the PPA we use doesn't have those
        #       libraries...

        FOREACH(QT_FILE icudata icui18n icuuc)
            LINUX_DEPLOY_QT_LIBRARY(${QT_LIBRARY_DIR} ${CMAKE_SHARED_LIBRARY_PREFIX}${QT_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}.56)
        ENDFOREACH()
    ENDIF()

    # Qt plugins required by OpenCOR

    LINUX_DEPLOY_QT_PLUGIN(imageformats qjpeg)
    LINUX_DEPLOY_QT_PLUGIN(platforms qxcb)
    LINUX_DEPLOY_QT_PLUGIN(printsupport cupsprintersupport)
    LINUX_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    IF(NOT ENABLE_TRAVIS_CI)
        # We are building OpenCOR locally, so we can deploy the
        # xcbglintegrations plugins
        # Note: indeed, on Travis CI, the PPA we use doesn't have those
        #       plugins...

        LINUX_DEPLOY_QT_PLUGIN(xcbglintegrations qxcb-egl-integration qxcb-glx-integration)
    ENDIF()

    # Shell script to run OpenCOR

    SET(SH_FILENAME ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/linux/application.sh.in
                   ${SH_FILENAME})

    INSTALL(FILES ${SH_FILENAME}
            DESTINATION .
            RENAME ${CMAKE_PROJECT_NAME}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Specify the package file name, depending on whether we are generating a
# snapshot version of OpenCOR

IF(SNAPSHOT)
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${DATE}-${CPACK_SYSTEM_NAME})
ELSE()
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${PROJECT_VERSION_MAJOR}-${PROJECT_VERSION_MINOR})

    IF(NOT "${PROJECT_VERSION_PATCH}" STREQUAL "")
        SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${PROJECT_VERSION_PATCH})
    ENDIF()

    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME})
ENDIF()

# Notice and license files

SET(NOTICE_FILENAME ${PROJECT_BUILD_DIR}/NOTICE.txt)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/NOTICE.txt.in
               ${NOTICE_FILENAME})

SET(FILES
    ${CMAKE_SOURCE_DIR}/LICENSE.txt
    ${NOTICE_FILENAME}
)

FOREACH(FILE ${FILES})
    INSTALL(FILES ${FILE}
            DESTINATION .
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
ENDFOREACH()

# Some sample CellML files

SET(MODEL_FILES
    hodgkin_huxley_squid_axon_model_1952.cellml
    noble_model_1962.cellml
    van_der_pol_model_1928.cellml
)

FOREACH(MODEL_FILE ${MODEL_FILES})
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/${MODEL_FILE}
            DESTINATION models
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
ENDFOREACH()

# Some user-defined format files

SET(FORMAT_FILES
    C.xml
    F77.xml
    MATLAB.xml
    Python.xml
    README.txt
)

FOREACH(FORMAT_FILE ${FORMAT_FILES})
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/${FORMAT_FILE}
            DESTINATION formats
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
ENDFOREACH()

INCLUDE(CPack)
