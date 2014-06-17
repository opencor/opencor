PROJECT(OpenCOR)

# Minimum required version of CMake

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Our options

OPTION(ENABLE_SAMPLES "Enable the sample plugins to be built" OFF)
OPTION(ENABLE_TESTS "Enable the tests to be built" OFF)

OPTION(USE_PREBUILT_LLVM_PLUGIN "Use the pre-built version of the LLVM plugin" ON)
OPTION(USE_PREBUILT_QSCINTILLA_PLUGIN "Use the pre-built version of the QScintilla plugin" ON)
OPTION(USE_PREBUILT_QWT_PLUGIN "Use the pre-built version of the Qwt plugin" ON)
OPTION(USE_PREBUILT_SUNDIALS_PLUGIN "Use the pre-built version of the SUNDIALS plugin" ON)

# Initialise the project

INITIALISE_PROJECT()

# Version/snapshot of OpenCOR

SET(SNAPSHOT ON)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 2)
SET(VERSION_PATCH 0)

IF(SNAPSHOT)
    TRY_RUN(DATE_RUN DATE_COMPILE
            ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/cmake/date.c
            RUN_OUTPUT_VARIABLE DATE)

    SET(VERSION ${DATE})
ELSE()
    SET(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

    IF(NOT ${VERSION_PATCH} EQUAL 0)
        SET(VERSION ${VERSION}.${VERSION_PATCH})
    ENDIF()
ENDIF()

FILE(WRITE ${CMAKE_BINARY_DIR}/version.txt ${VERSION})

# Files that make up the GUI version of OpenCOR
# Note: when it comes to using some of the Core plugin features, we don't want
#       any of them to require a MOC file to be generated. Indeed, should this
#       happen, then some build systems (e.g. Ninja) will warn that there exist
#       multiple rules to generate it. In practice, it's fine, but it's still a
#       warning and we don't like (and want) warnings...

SET(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/pluginswindow.cpp
    src/preferenceswindow.cpp
    src/splashscreenwindow.cpp

    src/misc/cliapplication.cpp
    src/misc/common.cpp

    src/plugins/guiinterface.cpp
    src/plugins/i18ninterface.cpp
    src/plugins/plugin.cpp
    src/plugins/plugininfo.cpp
    src/plugins/pluginmanager.cpp

    src/plugins/miscellaneous/Core/src/cliutils.cpp
    src/plugins/miscellaneous/Core/src/commonwidget.cpp
    src/plugins/miscellaneous/Core/src/guiutils.cpp
)

SET(HEADERS_MOC
    src/mainwindow.h
    src/pluginswindow.h
    src/preferenceswindow.h
    src/splashscreenwindow.h

    src/plugins/plugin.h
    src/plugins/pluginmanager.h
)

SET(UIS
    src/mainwindow.ui
    src/pluginswindow.ui
    src/preferenceswindow.ui
    src/splashscreenwindow.ui
)

SET(RESOURCES
    res/common.qrc
    res/ui.qrc
)

# Files that make up the CLI version of OpenCOR (Windows specific)
# Note: see the node about using Core plugin features above...

IF(WIN32)
    SET(WINDOWS_CLI_SOURCES
        windows/main.cpp

        src/misc/cliapplication.cpp
        src/misc/common.cpp

        src/plugins/plugin.cpp
        src/plugins/plugininfo.cpp
        src/plugins/pluginmanager.cpp

        src/plugins/miscellaneous/Core/src/cliutils.cpp
    )

    SET(WINDOWS_CLI_HEADERS_MOC
        src/plugins/plugin.h
        src/plugins/pluginmanager.h
    )

    SET(WINDOWS_CLI_RESOURCES
        res/common.qrc
    )
ENDIF()

# Various include directories
# Note: the OpenCOR executable uses bits of the Core plugin, hence we include
#       its directory so that we can build OpenCOR without any plugin, should we
#       ever want/need to do that...

INCLUDE_DIRECTORIES(
    src/comp
    src/misc
    src/plugins
    src/plugins/miscellaneous/Core/src
)

# Update the translation (.ts) files and generate the language (.qm) files
# which will later on be embedded in the OpenCOR executable as resources

UPDATE_LANGUAGE_FILES(${PROJECT_NAME} FALSE)

# Third-party library which must be directly embedded in the GUI version of
# OpenCOR

INCLUDE(src/3rdparty/QtSingleApplication/QtSingleApplication.cmake)

# Set the application icon, but only for Windows and OS X, since in the case of
# Linux, it's done through the use of app_icon (see res/ui.qrc) in
# src/mainwindow.ui

IF(WIN32)
    LIST(APPEND SOURCES ${CMAKE_SOURCE_DIR}/res/${PROJECT_NAME}.rc)
ELSEIF(APPLE)
    SET(IconFile ${PROJECT_NAME}.icns)

    SET(MACOSX_BUNDLE_ICON_FILE ${IconFile})

    SET_SOURCE_FILES_PROPERTIES(res/boomy/${IconFile} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    LIST(APPEND SOURCES res/boomy/${IconFile})
ENDIF()

# Check whether tests are required and, if so, set the destination tests
# directory and build our main test program

IF(ENABLE_TESTS)
    # Destination tests directory
    # Note: DEST_TESTS_DIR isn't only used here, but also in our ADD_PLUGIN()
    #       macro...

    SET(DEST_TESTS_DIR ${PROJECT_BUILD_DIR}/tests)

    # Build our main test program

    SET(RUNTESTS_NAME runtests)

    ADD_EXECUTABLE(${RUNTESTS_NAME}
        tests/main.cpp
    )

    QT5_USE_MODULES(${RUNTESTS_NAME}
        Core
    )

    # Copy our main test program to our tests directory

    SET(MAIN_TEST_FILENAME ${RUNTESTS_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${RUNTESTS_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${MAIN_TEST_FILENAME}
                                                        ${DEST_TESTS_DIR}/${MAIN_TEST_FILENAME})
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

    thirdParty/LLVM
    thirdParty/QScintilla
    thirdParty/Qwt
    thirdParty/SUNDIALS

    api/CellMLAPI

    miscellaneous/Core

    solver/CoreSolver

    # Plugins that build on top of some third-party libraries

    miscellaneous/Compiler

    # Plugins that provide support for some third-party libraries

    support/CellMLSupport
    support/QScintillaSupport

    # Plugins that extend some libraries (incl. third-party ones)

    widget/Editor
    widget/Viewer

    # Plugins that have dependencies on others

    editing/CoreCellMLEditing
    editing/CoreEditing

    # Selectable plugins

    editing/CellMLAnnotationView
#---GRY--- TEMPORARILY DISABLED...
#    editing/PrettyCellMLView
    editing/RawCellMLView
    editing/RawView

    miscellaneous/CellMLTools
    miscellaneous/HelpWindow

    organisation/CellMLModelRepositoryWindow
    organisation/FileBrowserWindow
    organisation/FileOrganiserWindow

    simulation/SingleCellView

    solver/CVODESolver
    solver/ForwardEulerSolver
    solver/FourthOrderRungeKuttaSolver
    solver/HeunSolver
    solver/IDASolver
    solver/KINSOLSolver
    solver/SecondOrderRungeKuttaSolver

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

    INCLUDE_DIRECTORIES(build/${PLUGIN_DIR})
ENDFOREACH()

# Build the GUI version of OpenCOR

QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT5_WRAP_UI(SOURCES_UIS ${UIS})
QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_UIS}
    ${SOURCES_RCS}
)

IF(APPLE)
    SET(QT_MODULES_EXTRAS MacExtras)
ELSE()
    SET(QT_MODULES_EXTRAS)
ENDIF()

QT5_USE_MODULES(${PROJECT_NAME}
    ${QT_MODULES_EXTRAS}
    Concurrent
    Help
    Network
    PrintSupport
    Svg
    WebKitWidgets
    Widgets
    Xml
    XmlPatterns
)

IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${Qt5Core_QTMAIN_LIBRARIES}
    )
ENDIF()

# Build the CLI version of OpenCOR (Windows specific)

IF(WIN32)
    QT5_WRAP_CPP(WINDOWS_CLI_SOURCES_MOC ${WINDOWS_CLI_HEADERS_MOC})
    QT5_ADD_RESOURCES(WINDOWS_CLI_SOURCES_RCS ${WINDOWS_CLI_RESOURCES})

    SET(WINDOWS_CLI_PROJECT_NAME ${PROJECT_NAME}.com)

    ADD_EXECUTABLE(${WINDOWS_CLI_PROJECT_NAME}
        ${WINDOWS_CLI_SOURCES}
        ${WINDOWS_CLI_SOURCES_MOC}
        ${WINDOWS_CLI_SOURCES_RCS}
    )

    QT5_USE_MODULES(${WINDOWS_CLI_PROJECT_NAME}
        Core
        XmlPatterns
    )
ENDIF()

# Additional compiler and linker settings

ADD_DEFINITIONS(-D${PROJECT_NAME}_MAIN)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

IF(WIN32)
    SET_TARGET_PROPERTIES(${WINDOWS_CLI_PROJECT_NAME} PROPERTIES
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )
ENDIF()

# Some post-processing specific

IF(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something that can't be done using CMake (or so it seems)...

    SET(InfoPlistFile ${CMAKE_BINARY_DIR}/Info.plist)

    EXECUTE_PROCESS(COMMAND date "+%Y"
                    OUTPUT_VARIABLE YEAR
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/osx/Info.plist.in ${InfoPlistFile})

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${InfoPlistFile}
    )

    # Note: ideally, what follows would be done using macdeployqt, but it has
    #       regularly been causing us problems, so instead we have decided to do
    #       everything ourselves...

    # Copy our qt.conf file over

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/distrib/osx/qt.conf
                                                        ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Resources/qt.conf)

    # Clean up the OpenCOR executable

    IF(APPLE)
        SET(QT_LIBRARIES_EXTRAS QtMacExtras)
    ELSE()
        SET(QT_LIBRARIES_EXTRAS)
    ENDIF()

    SET(QT_LIBRARIES
        ${QT_LIBRARIES_EXTRAS}
        QtConcurrent
        QtCore
        QtGui
        QtHelp
        QtMultimedia
        QtMultimediaWidgets
        QtNetwork
        QtOpenGL
        QtPositioning
        QtPrintSupport
        QtQml
        QtQuick
        QtSensors
        QtSvg
        QtWebKit
        QtWebKitWidgets
        QtWidgets
        QtXml
        QtXmlPatterns
    )

    OS_X_CLEAN_UP_FILE_WITH_QT_LIBRARIES(${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/MacOS ${PROJECT_NAME}
                                         ${QT_LIBRARIES})

    # Deploy our Qt libraries, but only if we are to package OpenCOR
    # Note: see OS_X_CLEAN_UP_FILE_WITH_QT_LIBRARIES() for the reason...

    IF("$ENV{PACKAGE_OPENCOR}" STREQUAL "True")
        OS_X_DEPLOY_QT_LIBRARIES(${QT_LIBRARIES} QtCLucene QtSql)
    ENDIF()

    # Deploy some plugins

    OS_X_DEPLOY_QT_PLUGIN(accessible qtaccessiblewidgets)
    OS_X_DEPLOY_QT_PLUGIN(imageformats qjpeg)
    OS_X_DEPLOY_QT_PLUGIN(platforms qcocoa)
    OS_X_DEPLOY_QT_PLUGIN(printsupport cocoaprintersupport)
    OS_X_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)
ELSE()
    # Copy the GUI version of the OpenCOR executable to the build/bin folder

    COPY_FILE_TO_BUILD_DIR(${PROJECT_NAME} ${PROJECT_BUILD_DIR} bin
                           ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    IF(WIN32)
        # Copy the CLI version of the OpenCOR executable to both the build and
        # build/bin folders
        # Note: the idea is that, on Windows, we can test both the CLI and the
        #       GUI versions of OpenCOR from either build or build/bin...

        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_PROJECT_NAME} ${PROJECT_BUILD_DIR} .
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.com)
        COPY_FILE_TO_BUILD_DIR(${WINDOWS_CLI_PROJECT_NAME} ${PROJECT_BUILD_DIR} bin
                               ${WINDOWS_CLI_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.com)

        # Redistributable files required by OpenCOR
        # Note: these files may or not be needed in order to test OpenCOR
        #       locally. It all depends on how the user's computer is
        #       configured...

        RETRIEVE_BINARY_FILE(distrib/windows/x86 libeay32.dll 8e18f9f241ef04f41675934cac11ac98dc721a4b)
        RETRIEVE_BINARY_FILE(distrib/windows/x86 msvcp100.dll 79bcaad3714433e01c7f153b05b781f8d7cb318d)
        RETRIEVE_BINARY_FILE(distrib/windows/x86 msvcr100.dll 4e880fc7625ccf8d9ca799d5b94ce2b1e7597335)
        RETRIEVE_BINARY_FILE(distrib/windows/x86 ssleay32.dll 9b82650fbf2d51eea2af13a4cd85c88407e67559)

        FOREACH(REDISTRIBUTABLE_FILE libeay32 msvcp100 msvcr100 ssleay32)
            COPY_FILE_TO_BUILD_DIR(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/distrib/windows/x86 .
                                   ${REDISTRIBUTABLE_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX})
            COPY_FILE_TO_BUILD_DIR(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/distrib/windows/x86 bin
                                   ${REDISTRIBUTABLE_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ENDFOREACH()
    ENDIF()
ENDIF()

# Package OpenCOR

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Team OpenCOR (team@opencor.ws)")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform CellML-based modelling environment")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

IF(WIN32)
    # Select NSIS and ZIP as the packagers on Windows

    SET(CPACK_GENERATOR NSIS ZIP)

    SET(CPACK_SYSTEM_NAME "Windows")

    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    # Install both the GUI and consolve version of OpenCOR
    # Note: we would normally use
    #           INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    #       for the GUI version of OpenCOR, but for some reasons it doesn't
    #       work (anymore?) while it works fine on Linux and OS X. Could that
    #       have something to do with us now using MSVC?...

    INSTALL(FILES ${PROJECT_BUILD_DIR}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
            DESTINATION bin)
    INSTALL(FILES ${PROJECT_BUILD_DIR}/bin/${PROJECT_NAME}.com
            DESTINATION bin)

    # Redistributable files required by OpenCOR

    FOREACH(REDISTRIBUTABLE_FILE libeay32 msvcp100 msvcr100 ssleay32)
        INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/windows/x86/${REDISTRIBUTABLE_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin)
    ENDFOREACH()

    # Qt libraries required by OpenCOR

    WINDOWS_DEPLOY_QT_LIBRARIES(Qt5CLucene Qt5Core Qt5Gui Qt5Help Qt5Multimedia Qt5MultimediaWidgets Qt5Network Qt5OpenGL Qt5Positioning Qt5PrintSupport Qt5Qml Qt5Quick Qt5Sensors Qt5Sql Qt5Svg Qt5WebKit Qt5WebKitWidgets Qt5Widgets Qt5Xml Qt5XmlPatterns)
    WINDOWS_DEPLOY_QT_LIBRARIES(icudt51 icuin51 icuuc51)

    # Qt plugins required by OpenCOR

    WINDOWS_DEPLOY_QT_PLUGIN(accessible qtaccessiblewidgets)
    WINDOWS_DEPLOY_QT_PLUGIN(imageformats qjpeg)

    FOREACH(QT_PLUGIN_FILE qwindows)
        INSTALL(FILES ${QT_PLUGINS_DIR}/platforms/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_PLUGIN_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin/platforms)
        # Note: yes, the platforms plugin must be installed in the bin folder
        #       and not the plugins folder...
    ENDFOREACH()

    WINDOWS_DEPLOY_QT_PLUGIN(printsupport windowsprintersupport)
    WINDOWS_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    # Batch and VBScript files to run OpenCOR (useful when downloading a ZIPped
    # version of OpenCOR)

    SET(BatFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bat)
    SET(VbsFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.vbs)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.bat.in ${BatFile})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.vbs.in ${VbsFile})

    INSTALL(FILES ${BatFile}
            DESTINATION .)
    INSTALL(FILES ${VbsFile}
            DESTINATION .)

    # File type association
    # Note: the calls to SHChangeNotify are to ensure that Windows refreshes
    #       file icons (so that it is clear to the user that an extension has
    #       been (un)registered...

    SET(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}\\\\distrib\\\\windows\\\\FileAssociation.nsh")

    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        \\\${RegisterExtension} \\\"\\\$INSTDIR\\\\bin\\\\${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\\\" \\\".cellml\\\" \\\"CellML File\\\"
        System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
    ")
    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        \\\${UnregisterExtension} \\\".cellml\\\" \\\"CellML File\\\"
        System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
    ")
ELSEIF(APPLE)
    # Select PackageMaker and ZIP as the packagers on OS X

    SET(CPACK_GENERATOR PackageMaker ZIP)

    SET(CPACK_SYSTEM_NAME "OSX")

    SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/distrib/osx/welcome.txt")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    SET(INSTALL_DIR "/Applications/${PROJECT_NAME}")

    SET(CPACK_SET_DESTDIR TRUE)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_DIR})

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${INSTALL_DIR})

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    SET(SHELL_FILENAME ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/osx/application.sh.in ${SHELL_FILENAME})

    INSTALL(FILES ${SHELL_FILENAME}
            RENAME ${PROJECT_NAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)

    SET(CPACK_SYSTEM_NAME "Linux${ARCHITECTURE}")

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt libraries required by OpenCOR
    # Note: unlike for Windows and OS X, we don't have a macro for deploying our
    #       Qt libraries. Indeed, on Linux, the version of a file is appended to
    #       its suffix, so unless making the macro unnecessarily cumbersome to
    #       use, it's better to do things ourselves...

    FOREACH(QT_LIBRARY CLucene Core DBus Gui Help Network OpenGL Positioning PrintSupport Qml Quick Sensors Sql Svg WebKit WebKitWidgets Widgets Xml XmlPatterns)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}
                DESTINATION lib
                RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR})
    ENDFOREACH()

    FOREACH(QT_FILE icudata icui18n icuuc)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}.51.1
                DESTINATION lib
                RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${QT_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}.51)
    ENDFOREACH()

    # Additional libraries required by Qt, but which are not available on a
    # 'blank' version of Ubuntu or their version differs from one version of
    # Ubuntu to another
    # Note: starting with Ubuntu 11.04, some system libraries are located in
    #       /usr/lib/i386-linux-gnu or /usr/lib/x86_64-linux-gnu rather than
    #       /usr/lib, so we need to check those folders first...

    SET(DEFAULT_LIBRARY_FOLDER /usr/lib)
    SET(X86_LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER}/i386-linux-gnu)
    SET(X64_LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER}/x86_64-linux-gnu)

    IF(EXISTS "${X86_LIBRARY_FOLDER}" AND IS_DIRECTORY "${X86_LIBRARY_FOLDER}")
        SET(LIBRARY_FOLDER ${X86_LIBRARY_FOLDER})
    ELSEIF(EXISTS "${X64_LIBRARY_FOLDER}" AND IS_DIRECTORY "${X64_LIBRARY_FOLDER}")
        SET(LIBRARY_FOLDER ${X64_LIBRARY_FOLDER})
    ELSE()
        SET(LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER})
    ENDIF()

    FOREACH(LIBRARY stdc++.so.6)
        # Note: stdc++.so.6 is required by a few Qt libraries and may not be
        #       present or different on the target machine, so...

        IF(EXISTS "${LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}")
            SET(CORRECT_LIBRARY ${LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})
        ELSEIF(EXISTS "${DEFAULT_LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}")
            SET(CORRECT_LIBRARY ${DEFAULT_LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})
        ELSE()
            SET(CORRECT_LIBRARY)
        ENDIF()

        IF("${CORRECT_LIBRARY}" STREQUAL "")
            MESSAGE(FATAL_ERROR "The '${LIBRARY}' could not be found...")
        ELSE()
            GET_FILENAME_COMPONENT(REAL_CORRECT_LIBRARY ${CORRECT_LIBRARY} REALPATH)

            INSTALL(FILES ${REAL_CORRECT_LIBRARY} RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}
                    DESTINATION lib)
        ENDIF()
    ENDFOREACH()

    # Qt configuration file to tell OpenCOR where to look for plugins

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/linux/qt.conf
            DESTINATION bin)

    # Qt plugins required by OpenCOR

    LINUX_DEPLOY_QT_PLUGIN(accessible qtaccessiblewidgets)
    LINUX_DEPLOY_QT_PLUGIN(imageformats qjpeg)
    LINUX_DEPLOY_QT_PLUGIN(platforms qxcb)
    LINUX_DEPLOY_QT_PLUGIN(printsupport cupsprintersupport)
    LINUX_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    # Shell script to run OpenCOR

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/linux/application.sh.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Specify the package file name, depending on whether we are generating a
# snapshot version of OpenCOR

IF(SNAPSHOT)
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${DATE}-${CPACK_SYSTEM_NAME})
ELSE()
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${VERSION_MAJOR}-${VERSION_MINOR})

    IF(NOT ${VERSION_PATCH} EQUAL 0)
        SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${VERSION_PATCH})
    ENDIF()

    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME})
ENDIF()

# Some sample CellML files

INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/hodgkin_huxley_squid_axon_model_1952.cellml
        DESTINATION models
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/noble_model_1962.cellml
        DESTINATION models
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/van_der_pol_model_1928.cellml
        DESTINATION models
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# Some user-defined format files

INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/C.xml
        DESTINATION formats
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/F77.xml
        DESTINATION formats
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/MATLAB.xml
        DESTINATION formats
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/Python.xml
        DESTINATION formats
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/formats/README.txt
        DESTINATION formats
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

INCLUDE(CPack)
