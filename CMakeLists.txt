PROJECT(OpenCOR)

# Minimum required version of CMake

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

# Name of the main project and location of the built OpenCOR binary
# Note: these are required, on OS X, to determine where plugins should be
#       copied, so...

SET(MAIN_PROJECT_NAME ${PROJECT_NAME})
SET(OS_X_PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Initialise the project

INITIALISE_PROJECT()

# Version of OpenCOR

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

FILE(WRITE ${CMAKE_BINARY_DIR}/version.txt ${VERSION})

# Files that make up the GUI version of OpenCOR

SET(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/pluginswindow.cpp

    src/misc/common.cpp
    src/misc/utils.cpp

    src/plugins/coreinterface.cpp
    src/plugins/fileinterface.cpp
    src/plugins/guiinterface.cpp
    src/plugins/i18ninterface.cpp
    src/plugins/interface.cpp
    src/plugins/plugin.cpp
    src/plugins/plugininfo.cpp
    src/plugins/pluginmanager.cpp
    src/plugins/solverinterface.cpp

    src/plugins/misc/Core/src/commonwidget.cpp
    src/plugins/misc/Core/src/dockwidget.cpp
)

SET(HEADERS_MOC
    src/mainwindow.h
    src/pluginswindow.h

    src/plugins/plugin.h
    src/plugins/pluginmanager.h

    src/plugins/misc/Core/src/dockwidget.h
)

SET(UIS
    src/mainwindow.ui
    src/pluginswindow.ui
)

SET(RESOURCES
    res/common.qrc
    res/ui.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(
    build
    src
    src/misc
    src/plugins
    src/plugins/misc/Core/src
    src/ui
    src/widgets
)

# Update the translation (.ts) files and generate the language (.qm) files
# which will later on be embedded in the executable as resources

UPDATE_LANGUAGE_FILES(${PROJECT_NAME} FALSE)

# Third-party libraries which must be directly embedded in the GUI version of
# OpenCOR

INCLUDE_THIRD_PARTY_LIBRARY(. LibQxt)
INCLUDE_THIRD_PARTY_LIBRARY(. QtSingleApplication)

# Set the application icon, but only for Windows and OS X, since in the case of
# Linux, it's done through res/OpenCOR.qrc

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

IF(ENABLE_TESTING)
    # Destination tests directory

    SET(DEST_TESTS_DIR ${CMAKE_BINARY_DIR}/tests)

    # Build our main test program

    SET(RUNTESTS_NAME runtests)

    ADD_EXECUTABLE(${RUNTESTS_NAME}
        test/main.cpp
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

# Build the plugins used by either the GUI and/or console version of OpenCOR

SET(PLUGINS
    # Build the plugins which may be required by other plugins
    # Note: the build order is very important...

    misc/Core

    3rdparty/LLVM

    api/CellMLAPI

    editing/CoreEditing

    simulation/CoreSolver

    misc/Compiler

    misc/CellMLSupport

    editing/CoreCellMLEditing

    3rdparty/SUNDIALS

    simulation/CVODESolver
    simulation/ForwardEulerSolver
    simulation/IDASolver
    simulation/KINSOLSolver

    simulation/CoreSimulation

    3rdparty/QScintilla
    3rdparty/QtMmlWidget
    3rdparty/Qwt

    misc/QScintillaSupport

    # Build the remaining plugins

    misc/Help
    misc/Viewer

    organisation/CellMLModelRepository
    organisation/FileBrowser
    organisation/FileOrganiser

    editing/CellMLAnnotationView
    editing/RawCellMLView
    editing/RawView

    simulation/SingleCellSimulationView
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

QT5_USE_MODULES(${PROJECT_NAME}
    Concurrent
    Help
    Network
    PrintSupport
    Svg
    WebKitWidgets
    Widgets
    Xml
)

IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${Qt5Core_QTMAIN_LIBRARIES}
    )
ENDIF()

# Additional compiler and linker settings

ADD_DEFINITIONS(-D${PROJECT_NAME}_MAIN)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

# Some post-processing specific to Windows and Linux

IF(NOT APPLE)
    # Copy the OpenCOR executable to the build/bin folder

    COPY_FILE_TO_BUILD_DIR(${CMAKE_BINARY_DIR} bin
                           ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})

    IF(WIN32)
        # Copy the OpenCOR executable to the build/Release folder, so that
        # CPack can find it since this is where it expects it to be, except
        # that make.bat asks for the OpenCOR executable to be generated in
        # build to match what does Qt Creator, so...

        COPY_FILE_TO_BUILD_DIR(${CMAKE_BINARY_DIR} Release
                               ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
    ENDIF()
ENDIF()

# Get our application bundle ready

IF(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something which can't be done using CMake (or so it seems)...

    SET(InfoPlistFile ${CMAKE_BINARY_DIR}/Info.plist)

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
                                                        ${OS_X_PROJECT_BINARY_DIR}/Contents/Resources/qt.conf)

    # Clean up our main executable

    OS_X_CLEAN_UP_FILE_WITH_QT_DEPENDENCIES(${OS_X_PROJECT_BINARY_DIR}/Contents/MacOS ${PROJECT_NAME}
                                            QtConcurrent QtCore QtGui QtHelp QtNetwork QtOpenGL QtPrintSupport QtQml QtQuick QtSvg QtWebKit QtWebKitWidgets QtWidgets QtXml)

    # Deploy our Qt dependencies

    OS_X_DEPLOY_QT_LIBRARIES(QtCLucene QtConcurrent QtCore QtGui QtHelp QtNetwork QtOpenGL QtPrintSupport QtQml QtQuick QtSql QtSvg QtV8 QtWebKit QtWebKitWidgets QtWidgets QtXml)

    # Deploy some plugins

    OS_X_DEPLOY_QT_PLUGIN(accessible qtaccessiblequick qtaccessiblewidgets)
    OS_X_DEPLOY_QT_PLUGIN(imageformats qgif qico qjpeg qmng qsvg qtga qtiff qwbmp)
    OS_X_DEPLOY_QT_PLUGIN(platforms qcocoa)
    OS_X_DEPLOY_QT_PLUGIN(printsupport cocoaprintersupport)
    OS_X_DEPLOY_QT_PLUGIN(sqldrivers qsqlite qsqlodbc qsqlpsql)
ENDIF()

# Package OpenCOR (using CPack)

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Alan Garny (alan.garny@inria.fr)")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform CellML-based modelling environment")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

IF(WIN32)
    # Select NSIS and ZIP as the packagers on Windows

    SET(CPACK_GENERATOR NSIS ZIP)

    SET(CPACK_SYSTEM_NAME "Windows")

    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

    # Install both the GUI and consolve version of OpenCOR
    # Note: we would normally use
    #           INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    #       for the GUI version of OpenCOR, but for some reasons it doesn't
    #       work (anymore?) while it works fine on Linux and OS X. Could that
    #       have something to do with us now using MSVC?...

    INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
            DESTINATION bin)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.com
            DESTINATION bin)

    # MSVC files required by OpenCOR

    FOREACH(MSVC_LIBRARY msvcp100 msvcr100)
        INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/windows/x86/${CMAKE_SHARED_LIBRARY_PREFIX}${MSVC_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin)
    ENDFOREACH()

    # Qt libraries required by OpenCOR

    WINDOWS_DEPLOY_QT_LIBRARIES(Qt5CLucene Qt5Core Qt5Gui Qt5Help Qt5Network Qt5OpenGL Qt5PrintSupport Qt5Qml Qt5Quick Qt5Sql Qt5Svg Qt5V8 Qt5WebKit Qt5WebKitWidgets Qt5Widgets Qt5Xml)
    WINDOWS_DEPLOY_QT_LIBRARIES(icudt49 icuin49 icuuc49)
    WINDOWS_DEPLOY_QT_LIBRARIES(libEGL libGLESv2 D3DCompiler_43)

    # Qt plugins required by OpenCOR

    WINDOWS_DEPLOY_QT_PLUGIN(accessible qtaccessiblequick qtaccessiblewidgets)
    WINDOWS_DEPLOY_QT_PLUGIN(imageformats qgif qico qjpeg qmng qsvg qtga qtiff qwbmp)

    FOREACH(QT_PLUGIN_FILE qminimal qwindows)
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
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

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

    SET(CPACK_SYSTEM_NAME "Linux")

    IF(64BIT_MODE)
        SET(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}64")
    ELSE()
        SET(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}32")
    ENDIF()

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt libraries required by OpenCOR
    # Note: unlike for Windows and OS X, we don't have a macro for deploying our
    #       Qt libraries. Indeed, on Linux, the version of a file is appended to
    #       its suffix, so unless making the macro unnecessarily cumbersome to
    #       use, it's better to do things ourselves...

    FOREACH(QT_LIBRARY Qt5CLucene Qt5Core Qt5DBus Qt5Gui Qt5Help Qt5Network Qt5OpenGL Qt5PrintSupport Qt5Qml Qt5Quick Qt5Sql Qt5Svg Qt5V8 Qt5WebKit Qt5WebKitWidgets Qt5Widgets Qt5Xml)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR}
                DESTINATION lib)
    ENDFOREACH()

    FOREACH(QT_LIBRARY icudata icui18n icuuc)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.49
                DESTINATION lib)
    ENDFOREACH()

    # Additional libraries required by Qt, but which are not available on a
    # 'blank' version of Ubuntu or their version differs from one version of
    # Ubuntu to another
    # Note: it seems that, starting with Ubuntu 11.04, some libraries may end
    #       up in /usr/lib/i386-linux-gnu or /usr/lib/x86_64-linux-gnu,
    #       depending on whether the system is 32-bit or 64-bit, so we need to
    #       check those folders first...

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

    LINUX_DEPLOY_QT_PLUGIN(accessible qtaccessiblequick qtaccessiblewidgets)
    LINUX_DEPLOY_QT_PLUGIN(imageformats qgif qico qjpeg qmng qsvg qtga qtiff qwbmp)
    LINUX_DEPLOY_QT_PLUGIN(platforms qlinuxfb qminimal qxcb)
    LINUX_DEPLOY_QT_PLUGIN(printsupport cupsprintersupport)
    LINUX_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    # Shell script to run OpenCOR

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/linux/application.sh.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Some sample CellML files

INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/hodgkin_huxley_squid_axon_model_1952.cellml
        DESTINATION models
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/van_der_pol_model_1928.cellml
        DESTINATION models
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

INCLUDE(CPack)
