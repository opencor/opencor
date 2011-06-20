PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Location of the built OpenCOR application on Mac OS X
# Note: this is, among other things, required to determine where plugins should
#       be copied

SET(MAC_OS_X_PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app)

# Location of our CMake files

SET(CMAKE_FILES_DIR ${CMAKE_SOURCE_DIR}/cmake)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

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

    src/misc/cellml.cpp
    src/misc/common.cpp
    src/misc/utils.cpp
    src/misc/versionmanager.cpp

    src/plugins/plugin.cpp
    src/plugins/pluginmanager.cpp

    src/ui/centralwidget.cpp
    src/ui/mainwindow.cpp
)

SET(HEADERS
    src/misc/cellml.h
    src/misc/common.h
    src/misc/utils.h
)

SET(HEADERS_MOC
    src/misc/versionmanager.h

    src/plugins/plugin.h
    src/plugins/pluginmanager.h

    src/ui/centralwidget.h
    src/ui/mainwindow.h
)

SET(UIS
    src/ui/centralwidget.ui
    src/ui/mainwindow.ui
)

SET(RESOURCES
    res/common.qrc
    res/ui.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(src/misc)
INCLUDE_DIRECTORIES(src/plugins)
INCLUDE_DIRECTORIES(src/ui)
INCLUDE_DIRECTORIES(src/widgets)

# Qt specific

SET(QT_USE_QTHELP TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSQL TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTXML TRUE)

IF(WIN32 OR APPLE)
    SET(QT_USE_PHONON TRUE)
ELSE()
    SET(QT_USE_QTDBUS TRUE)
ENDIF()

INCLUDE(${QT_USE_FILE})

# Generate the help files which will be embedded in the executable as a
# resource

EXECUTE_PROCESS(COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/${PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.qhc)

# Update the translation (.ts) files and generate the language (.qm) files
# which will later on be embedded in the executable as resources

UPDATE_LANGUAGE_FILES(${PROJECT_NAME})

# Third-party libraries which must be directly embedded in the GUI version of
# OpenCOR

INCLUDE_THIRD_PARTY_LIBRARIES(.
    LibQxt
    QtSingleApplication
)

# Set the application icon, but only for Windows and Mac OS X, since in the
# case of Linux, it's done through res/OpenCOR.qrc

IF(WIN32)
    SET(IconObjectFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_icon.o)

    ADD_CUSTOM_COMMAND(OUTPUT ${IconObjectFile}
                       COMMAND windres -I ${CMAKE_SOURCE_DIR} -i ${CMAKE_SOURCE_DIR}/res/${PROJECT_NAME}.rc
                                       -o ${IconObjectFile}
                      )

    SET(SOURCES ${SOURCES} ${IconObjectFile})
ELSEIF(APPLE)
    SET(IconFile ${PROJECT_NAME}.icns)

    SET(MACOSX_BUNDLE_ICON_FILE ${IconFile})

    SET_SOURCE_FILES_PROPERTIES(res/boomy/${IconFile} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    SET(SOURCES ${SOURCES} res/boomy/${IconFile})
ENDIF()

# Build the plugins used by either the GUI and/or console version of OpenCOR

SET(PLUGINS
    misc/Core
    # Note: the Core plugin must be built before any other plugin

    3rdparty/QScintilla
    3rdparty/QtMmlWidget

    edit/Viewer

    misc/Help

    organise/CellMLModelRepository
    organise/FileBrowser
    organise/FileOrganiser
)

FOREACH(PLUGIN ${PLUGINS})
    SET(PLUGIN_DIR src/plugins/${PLUGIN})
    SET(PLUGIN_INCLUDE_DIRS)
    # Note: the above variable is defined here, but is actually set when
    #       processing the CMakeLists.txt file of the plugin

    ADD_SUBDIRECTORY(${PLUGIN_DIR})

    FOREACH(PLUGIN_INCLUDE_DIR ${PLUGIN_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${PLUGIN_DIR}/${PLUGIN_INCLUDE_DIR})
    ENDFOREACH()
ENDFOREACH()

# Build the GUI version of OpenCOR

QT4_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT4_WRAP_UI(SOURCES_UIS ${UIS})
QT4_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_UIS}
    ${SOURCES_RCS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${QT_QTNETWORK_LIBRARY}

    #---GRY---
    # The following libraries are compulsory plugins, yet they shouldn't be
    # listed here, but for now they have to be since OpenCOR doesn't yet support
    # plugins as such, so...

    CorePlugin
)

IF(WIN32 OR APPLE)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_PHONON_LIBRARY}
    )
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_QTDBUS_LIBRARY}
    )
ENDIF()

# Additional compiler and linker settings

SET(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}")

# Some Mac OS X specific information, as well as some pre- and post-processing
# Note: the post-processing ensures that OpenCOR can be used on any Mac OS X
#       machine that doesn't have Qt and the SQLite plugin

IF(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something which can't be done using CMake (or so it seems)...

    SET(InfoPlistFile ${CMAKE_BINARY_DIR}/Info.plist)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/res/Info.plist.in ${InfoPlistFile})

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${InfoPlistFile})

    # Remove any qt.conf file left from a previous build
    # Note: this prevents us from getting a warning when everything is actually
    #       OK

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E remove -f ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Resources/qt.conf)

    # Use macdeployqt to have all the Qt files required by OpenCOR added to the
    # bundle

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${QT_BINARY_DIR}/macdeployqt ${MAC_OS_X_PROJECT_BINARY_DIR})

    # Add the SQLite plugin to the bundle (required by the Help plugin)
    # Note: this would, ideally, be done as part of the packaging, but we do it
    #       here, since it allows us to test things as if Qt wasn't installed
    #       on our development Mac OS X machine (which is useful since we
    #       cannot have a virtual machine with a 'blank' Mac OS X system on it
    #       unlike with Windows and Linux)

    SET(SQLDRIVERS_DIR ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/sqldrivers)

    IF(NOT EXISTS ${SQLDRIVERS_DIR})
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory ${SQLDRIVERS_DIR})
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.dylib ${SQLDRIVERS_DIR})

    # Make sure that the SQLite plugin refers to our embedded version of QtCore
    # and QtSql which it needs

    FOREACH(LIBRARY QtCore QtSql)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -change ${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                             @executable_path/../Frameworks/${LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${LIBRARY}
                                                             ../PlugIns/sqldrivers/libqsqlite.dylib
                           WORKING_DIRECTORY ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Frameworks)
    ENDFOREACH()
ENDIF()

# Package OpenCOR (using CPack)

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Alan Garny (alan.garny@dpag.ox.ac.uk)")
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

    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # The console version of OpenCOR

    INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.com DESTINATION bin)

    # MinGW files required by OpenCOR

    FOREACH(DLL mingwm10 libgcc_s_dw2-1)
        INSTALL(FILES ${QT_BINARY_DIR}/../../mingw/bin/${DLL}.dll DESTINATION bin)
    ENDFOREACH()

    # Qt files required by OpenCOR

    FOREACH(DLL phonon QtCLucene QtCore QtGui QtHelp QtNetwork QtSql QtWebKit QtXml)
        INSTALL(FILES ${QT_BINARY_DIR}/${DLL}${QT_VERSION_MAJOR}.dll DESTINATION bin)
    ENDFOREACH()

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_PLUGINS_DIR}/sqldrivers/qsqlite${QT_VERSION_MAJOR}.dll DESTINATION bin/sqldrivers)

    # Batch and VBScript files to run OpenCOR (useful when downloading a ZIPped
    # version of OpenCOR)

    SET(BatFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bat)
    SET(VbsFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.vbs)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/application.bat.in ${BatFile})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/application.vbs.in ${VbsFile})

    INSTALL(FILES ${BatFile} DESTINATION .)
    INSTALL(FILES ${VbsFile} DESTINATION .)

    # File type association
    # Note: the calls to SHChangeNotify are to ensure that Windows refreshes
    #       file icons (so that it is clear to the user that an extension has
    #       been (un)registered...

    SET(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}/distrib\\\\FileAssociation.nsh")

    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        \\\${RegisterExtension} \\\"\\\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\" \\\".cellml\\\" \\\"CellML File\\\"
        System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
    ")
    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        \\\${UnregisterExtension} \\\".cellml\\\" \\\"CellML File\\\"
        System::Call \\\"Shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)\\\"
    ")
ELSEIF(APPLE)
    # Select PackageMaker and ZIP as the packagers on Mac OS X

    SET(CPACK_GENERATOR PackageMaker ZIP)

    SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/distrib/welcomeForMacOSXInstaller.txt")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

    SET(INSTALL_DIR "/Applications/${PROJECT_NAME}")

    SET(CPACK_SET_DESTDIR TRUE)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_DIR})

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${INSTALL_DIR})

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/application.sh.macosx.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME} DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt files required by OpenCOR

    FOREACH(LIBRARY QtCLucene QtCore QtDBus QtGui QtHelp QtNetwork QtSql QtWebKit QtXml)
        INSTALL(FILES
                ${QT_LIBRARY_DIR}/lib${LIBRARY}.so.${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}
                RENAME lib${LIBRARY}.so.${QT_VERSION_MAJOR}
                DESTINATION lib
        )
    ENDFOREACH()

    # Additional libraries required by Qt, but not available on a 'blank'
    # version of Ubuntu or their version differs from one version of Ubuntu to
    # another
    # Note: it seems that, starting with Ubuntu 11.04, some libraries may end
    #       up in /usr/lib/i386-linux-gnu or /usr/lib/x86_64-linux-gnu,
    #       depending on whether the system is 32-bit or 64-bit, so we need to
    #       check for these folders first

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
        # Note: stdc++.so.6 is required by QtCLucene and may be needed when
        #       packaging on a particular version of Ubuntu and deploying on
        #       another (e.g. packaging on Ubuntu 11.04 and deploying on Ubuntu
        #       10.04)

        IF(EXISTS "${LIBRARY_FOLDER}/lib${LIBRARY}")
            SET(CORRECT_LIBRARY ${LIBRARY_FOLDER}/lib${LIBRARY})
        ELSEIF(EXISTS "${DEFAULT_LIBRARY_FOLDER}/lib${LIBRARY}")
            SET(CORRECT_LIBRARY ${DEFAULT_LIBRARY_FOLDER}/lib${LIBRARY})
        ELSE()
            SET(CORRECT_LIBRARY)
        ENDIF()

        IF("${CORRECT_LIBRARY}" STREQUAL "")
            MESSAGE(FATAL_ERROR "The '${LIBRARY}' could not be found...")
        ELSE()
            GET_FILENAME_COMPONENT(REAL_CORRECT_LIBRARY ${CORRECT_LIBRARY} REALPATH)

            INSTALL(FILES ${REAL_CORRECT_LIBRARY} RENAME lib${LIBRARY} DESTINATION lib)
        ENDIF()
    ENDFOREACH()

    # Qt configuration file to tell OpenCOR where to look for plugins

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/qt.conf DESTINATION bin)

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.so DESTINATION plugins/sqldrivers)

    # Shell script to run OpenCOR

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/application.sh.linux.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME} DESTINATION . PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Some files common to all three platforms

INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/hodgkin_huxley_squid_axon_model_1952.cellml DESTINATION models PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/van_der_pol_model_1928.cellml DESTINATION models PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

INCLUDE(CPack)
