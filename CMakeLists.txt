CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

# Project definition

PROJECT(OpenCOR)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Our options

OPTION(ENABLE_SAMPLES "Enable the sample plugins to be built" OFF)
OPTION(ENABLE_TESTS "Enable the tests to be built" OFF)

OPTION(USE_PREBUILT_LIBGIT2_PLUGIN "Use the pre-built version of the libgit2 plugin" ON)
OPTION(USE_PREBUILT_LLVM_PLUGIN "Use the pre-built version of the LLVM plugin" ON)
OPTION(USE_PREBUILT_OAUTH_PLUGIN "Use the pre-built version of the OAuth plugin" ON)
OPTION(USE_PREBUILT_QSCINTILLA_PLUGIN "Use the pre-built version of the QScintilla plugin" ON)
OPTION(USE_PREBUILT_QWT_PLUGIN "Use the pre-built version of the Qwt plugin" ON)
OPTION(USE_PREBUILT_SUNDIALS_PLUGIN "Use the pre-built version of the SUNDIALS plugin" ON)
OPTION(USE_PREBUILT_ZLIB_PLUGIN "Use the pre-built version of the zlib plugin" ON)

# Initialise the project

INITIALISE_PROJECT()

# Keep track of our source and build directories

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

INCLUDE(src/3rdparty/QtSingleApplication/QtSingleApplication.cmake)

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

    # Clean up our test program, but only if we are on macOS

    IF(APPLE)
        MACOS_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${RUNTESTS_NAME} ${DEST_TESTS_DIR} ${MAIN_TEST_FILENAME})
    ENDIF()
ENDIF()

# Build the OpenCOR plugins
# Note: the build order must be such that plugins needed by others are built
#       first...

IF(ENABLE_SAMPLES)
    SET(SAMPLE_PLUGINS
        sample/Sample
        sample/SampleTools
        sample/SampleView
        sample/SampleWindow
    )
ELSE()
    SET(SAMPLE_PLUGINS)
ENDIF()

SET(PLUGINS
    # Plugins that have no dependencies

    api/BioSignalMLAPI
    api/CellMLAPI
    api/SBMLAPI
    api/SEDMLAPI

    miscellaneous/Core

    support/StandardSupport

    thirdParty/libgit2
    thirdParty/LLVM
    thirdParty/OAuth
    thirdParty/QScintilla
    thirdParty/Qwt
    thirdParty/SUNDIALS
    thirdParty/zlib

    # Plugins that build on top of some third-party libraries

    miscellaneous/Compiler

    # Plugins that provide support for some third-party libraries

    support/CellMLSupport
    support/PMRSupport
    support/QScintillaSupport
    support/SEDMLSupport
    support/ZIPSupport

    # Plugins that provide support for some third-party libraries that have
    # dependencies on others

    support/COMBINESupport

    # Plugins that extend some libraries (incl. third-party ones)

    widget/EditorWidget
    widget/GraphPanelWidget
    widget/MathMLViewerWidget
    widget/WebViewerWidget

    # Plugins that provide some generic features

    dataStore/DataStore

    editing/EditingView

    # Plugins that have dependencies on others

    editing/CellMLEditingView
    editing/SEDMLEditingView

    # Selectable plugins

    dataStore/BioSignalMLDataStore
    dataStore/CSVDataStore

    editing/CellMLAnnotationView
    editing/CellMLTextView
    editing/RawCellMLView
    editing/RawSEDMLView
    editing/RawTextView

    miscellaneous/HelpWindow
    miscellaneous/WebBrowserWindow

    organisation/FileBrowserWindow
    organisation/FileOrganiserWindow
    organisation/PMRWindow
    organisation/PMRWorkspacesWindow

    simulation/SingleCellView

    solver/CVODESolver
    solver/ForwardEulerSolver
    solver/FourthOrderRungeKuttaSolver
    solver/HeunSolver
    solver/IDASolver
    solver/KINSOLSolver
    solver/SecondOrderRungeKuttaSolver

    tools/CellMLTools

    # Sample plugins

    ${SAMPLE_PLUGINS}
)

FOREACH(PLUGIN ${PLUGINS})
    SET(PLUGIN_DIR src/plugins/${PLUGIN})

    ADD_SUBDIRECTORY(${PLUGIN_DIR})
    # Note: this will process the CMakeLists.txt file of the plugin resulting in
    #       PLUGIN_INCLUDE_DIRS being set, hence we can use it below to update
    #       our include directories

    FOREACH(PLUGIN_INCLUDE_DIR ${PLUGIN_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${PLUGIN_DIR}/${PLUGIN_INCLUDE_DIR})
    ENDFOREACH()

    INCLUDE_DIRECTORIES(${PROJECT_BUILD_DIR}/${PLUGIN_DIR})
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

IF(WIN32)
    QT5_WRAP_CPP(WINDOWS_CLI_SOURCES_MOC ${WINDOWS_CLI_HEADERS_MOC})
    QT5_ADD_RESOURCES(WINDOWS_CLI_SOURCES_RCS ${WINDOWS_CLI_RESOURCES})

    SET(WINDOWS_CLI_PROJECT_NAME ${CMAKE_PROJECT_NAME}.com)

    ADD_EXECUTABLE(${WINDOWS_CLI_PROJECT_NAME}
        ${WINDOWS_CLI_SOURCES}
        ${WINDOWS_CLI_SOURCES_MOC}
        ${WINDOWS_CLI_SOURCES_RCS}
    )

    TARGET_COMPILE_DEFINITIONS(${WINDOWS_CLI_PROJECT_NAME}
                               PRIVATE CLI_VERSION)

    SET_TARGET_PROPERTIES(${WINDOWS_CLI_PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${WINDOWS_CLI_PROJECT_NAME}
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

    # Deploy the OpenSSL libraries

    FOREACH(OPENSSL_LIBRARY ${OPENSSL_LIBRARIES})
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY ${OPENSSL_LIBRARY} REALPATH)
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY_DIRNAME ${REAL_OPENSSL_LIBRARY} DIRECTORY)
        GET_FILENAME_COMPONENT(REAL_OPENSSL_LIBRARY_FILENAME ${REAL_OPENSSL_LIBRARY} NAME)

        MACOS_DEPLOY_LIBRARY(${REAL_OPENSSL_LIBRARY_DIRNAME} ${REAL_OPENSSL_LIBRARY_FILENAME})
    ENDFOREACH()

    # Deploy our Qt libraries
    # Note: the Qt Brew formula that we use on Travis CI misses QtDBus, which
    #       results in some of our tests generating messages about two
    #       frameworks being available and that one of them will be used, but
    #       that we don't know which one. So, to avoid those messages, we simply
    #       don't deploy the Qt frameworks...

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

        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_PROJECT_NAME} ${PROJECT_BUILD_DIR} .
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)
        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_PROJECT_NAME} ${PROJECT_BUILD_DIR} bin
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_PROJECT_NAME}.com)

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

        RETRIEVE_BINARY_FILE(distrib/windows libeay32.dll 58f3cc096cfc002f575fb9041de757da8af6fd8d)
        RETRIEVE_BINARY_FILE(distrib/windows ssleay32.dll beca4818b830b2d9a0727b12df4794a1dfdb8cfd)

        LIST(APPEND ADDITIONAL_FILES
            ${CMAKE_SOURCE_DIR}/distrib/bin/libeay32.dll
            ${CMAKE_SOURCE_DIR}/distrib/bin/ssleay32.dll
        )

        FOREACH(ADDITIONAL_FILE ${ADDITIONAL_FILES})
            GET_FILENAME_COMPONENT(DIRNAME ${ADDITIONAL_FILE} DIRECTORY)
            GET_FILENAME_COMPONENT(FILENAME ${ADDITIONAL_FILE} NAME)

            COPY_FILE_TO_BUILD_DIR(DIRECT ${DIRNAME} . ${FILENAME})
            COPY_FILE_TO_BUILD_DIR(DIRECT ${DIRNAME} bin ${FILENAME})
        ENDFOREACH()
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

    # Additional files required by OpenCOR

    INSTALL(FILES ${ADDITIONAL_FILES}
            DESTINATION bin)

    # Qt libraries required by OpenCOR

    IF(ENABLE_TESTS)
        SET(TEST Test)
    ELSE()
        SET(TEST)
    ENDIF()

    FOREACH(QT_LIBRARY CLucene Core Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel WebKit WebKitWidgets Widgets Xml XmlPatterns)
        WINDOWS_DEPLOY_QT_LIBRARY(Qt5${QT_LIBRARY})
    ENDFOREACH()

    FOREACH(QT_LIBRARY icudt icuin icuuc)
        WINDOWS_DEPLOY_QT_LIBRARY(${QT_LIBRARY}57)
    ENDFOREACH()

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

    # System libraries required by OpenCOR

    FOREACH(SYSTEM_LIBRARY app base interfaces pbutils reamer video)
        # Library not needed on Ubuntu 14.04 LTS, but it is on (at least) Ubuntu
        # 16.04 LTS

        SET(FULL_SYSTEM_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}gst${SYSTEM_LIBRARY}-0.10${CMAKE_SHARED_LIBRARY_SUFFIX}.0)

        GET_FILENAME_COMPONENT(REAL_FULL_SYSTEM_LIBRARY /usr/lib/x86_64-linux-gnu/${FULL_SYSTEM_LIBRARY} REALPATH)

        INSTALL(FILES ${REAL_FULL_SYSTEM_LIBRARY}
                DESTINATION lib
                RENAME ${FULL_SYSTEM_LIBRARY})
    ENDFOREACH()

    # Library needed on Ubuntu 14.04 LTS, but not on Ubuntu 16.06 LTS
    # Note: this is due to our use of GCC/G++ 4.9+, which is not originally
    #       present on Ubuntu 14.04 LTS...

    INSTALL(FILES /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.22
            DESTINATION lib
            RENAME libstdc++.so.6)

    # Qt libraries required by OpenCOR
    # Note: unlike for Windows and macOS, we don't have a macro for deploying
    #       all of our Qt libraries at once. Indeed, on Linux, the version of a
    #       file is appended to its suffix, so unless making the macro
    #       unnecessarily cumbersome to use, it's better to deploy one Qt
    #       library at a time...

    IF(ENABLE_TESTS)
        SET(TEST Test)
    ELSE()
        SET(TEST)
    ENDIF()

    FOREACH(QT_LIBRARY CLucene Core DBus Gui Help Multimedia MultimediaWidgets Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg ${TEST} WebChannel WebKit WebKitWidgets Widgets XcbQpa Xml XmlPatterns)
        IF(   "${QT_LIBRARY}" STREQUAL "WebKit"
           OR "${QT_LIBRARY}" STREQUAL "WebKitWidgets")
            SET(REAL_QT_LIBRARY_DIR ${QT_WEBKIT_LIBRARIES_DIR})
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
