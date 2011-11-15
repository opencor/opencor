PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of the main project and location of the built OpenCOR binary
# Note: these are required, on Mac OS X, to determine where plugins should be
#       copied

SET(MAIN_PROJECT_NAME ${PROJECT_NAME})
SET(MAC_OS_X_PROJECT_LIBRARY_DIR ${CMAKE_BINARY_DIR}/lib)
SET(MAC_OS_X_PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app)

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
    src/checkforupdateswindow.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/pluginswindow.cpp
    src/preferenceswindow.cpp

    src/misc/common.cpp
    src/misc/utils.cpp

    src/plugins/coreinterface.cpp
    src/plugins/fileinterface.cpp
    src/plugins/guiinterface.cpp
    src/plugins/i18ninterface.cpp
    src/plugins/plugin.cpp
    src/plugins/plugininfo.cpp
    src/plugins/pluginmanager.cpp

    src/plugins/application/Core/src/commonwidget.cpp
)

SET(HEADERS
    src/misc/common.h
    src/misc/utils.h

    src/plugins/coreinterface.h
    src/plugins/fileinterface.h
    src/plugins/guiinterface.h
    src/plugins/i18ninterface.h
    src/plugins/plugininfo.h

    src/plugins/application/Core/src/commonwidget.h
    src/plugins/application/Core/src/coreglobal.h
)

SET(HEADERS_MOC
    src/checkforupdateswindow.h
    src/mainwindow.h
    src/pluginswindow.h
    src/preferenceswindow.h

    src/plugins/plugin.h
    src/plugins/pluginmanager.h
)

SET(UIS
    src/checkforupdateswindow.ui
    src/mainwindow.ui
    src/pluginswindow.ui
    src/preferenceswindow.ui
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
    src/ui
    src/widgets
)

# Qt specific

SET(QT_USE_PHONON TRUE)
SET(QT_USE_QTHELP TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTSQL TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTXML TRUE)

IF(NOT WIN32 AND NOT APPLE)
    SET(QT_USE_QTDBUS TRUE)
ENDIF()

INCLUDE(${QT_USE_FILE})

# Generate the help files which will be embedded in the executable as a
# resource

EXECUTE_PROCESS(COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/${PROJECT_NAME}.qhcp
                                                           -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.qhc)

# Update the translation (.ts) files and generate the language (.qm) files
# which will later on be embedded in the executable as resources

UPDATE_LANGUAGE_FILES(${PROJECT_NAME} FALSE)

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
    # Build the plugins which may be required by other plugins
    # Note: the build order is important...

    application/Core

    3rdparty/LLVM

    api/CellML

    editing/CoreEditing
    editing/CoreCellMLEditing

    simulation/CoreSimulation

    3rdparty/QJson
    3rdparty/QScintilla
    3rdparty/QtMmlWidget
    3rdparty/Qwt

    # Build the remaining plugins

    application/Help

    organisation/CellMLModelRepository
    organisation/FileBrowser
    organisation/FileOrganiser

    editing/RawCellMLView
    editing/RawView
    editing/Viewer

    simulation/CellMLSimulation
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
    ${QT_PHONON_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

IF(NOT WIN32 AND NOT APPLE)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_QTDBUS_LIBRARY}
    )
ENDIF()

# Additional compiler and linker settings

SET(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
ADD_DEFINITIONS(-D${PROJECT_NAME}_MAIN)

SET_TARGET_PROPERTIES(${PROJECT_NAME}
    PROPERTIES LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

# Some post-processing specific to Windows and Linux

IF(NOT APPLE)
    # Copy the OpenCOR executable to the build/bin folder

    COPY_FILE_TO_BUILD_DIR(bin ${CMAKE_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
ENDIF()

# Some Mac OS X specific information, as well as some pre- and post-processing
# Note: the post-processing ensures that OpenCOR can be used on any Mac OS X
#       machine that doesn't have Qt and the SQLite plugin

IF(APPLE)
    # Configure and use our own Info.plist file
    # Note: the reason for using our own Info.plist file is that it contains
    #       some information about associating .cellml files to OpenCOR,
    #       something which can't be done using CMake (or so it seems)...

    SET(InfoPlistFile ${CMAKE_BINARY_DIR}/Info.plist)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/macosx/Info.plist.in ${InfoPlistFile})

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${InfoPlistFile})

    # Remove any qt.conf file left from a previous build
    # Note: this prevents us from getting a warning when everything is actually
    #       OK

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E remove -f ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/Resources/qt.conf)

    # Remove the PlugIns directory left from a previous build
    # Note: this prevents us from getting error messages

    FOREACH(QT_FOLDER accessible bearer codecs graphicssystems imageformats
                      phonon_backend qmltooling)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} PRE_BUILD
                           COMMAND ${CMAKE_COMMAND} -E remove_directory ${MAC_OS_X_PROJECT_BINARY_DIR}/Contents/PlugIns/${QT_FOLDER})
    ENDFOREACH()

    # Use macdeployqt to have all the Qt files required by OpenCOR added to the
    # bundle

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${QT_BINARY_DIR}/macdeployqt ${MAC_OS_X_PROJECT_BINARY_DIR})

    # Some Qt libraries/frameworks don't get deployed by macdeployqt since they
    # are not used by OpenCOR directly anymore but by some plugins. We must
    # therefore deploy them ourselves and make sure that their Qt dependencies
    # refer to our embedded version of the Qt libraries

    DEPLOY_MAC_OS_X_LIBRARY(libQtCLucene
        TYPE
            Library
        FRAMEWORKS
            QtCore
    )

    DEPLOY_MAC_OS_X_LIBRARY(QtHelp
        TYPE
            Framework
        FRAMEWORKS
            QtCore
            QtGui
            QtNetwork
            QtSql
            QtXml
        LIBRARIES
            libQtCLucene
    )

    DEPLOY_MAC_OS_X_LIBRARY(QtOpenGL
        TYPE
            Framework
        FRAMEWORKS
            QtCore
            QtGui
    )

    DEPLOY_MAC_OS_X_LIBRARY(QtSvg
        TYPE
            Framework
        FRAMEWORKS
            QtCore
            QtGui
    )

    DEPLOY_MAC_OS_X_LIBRARY(QtWebKit
        TYPE
            Framework
        FRAMEWORKS
            phonon
            QtCore
            QtDBus
            QtGui
            QtNetwork
            QtXml
    )

    DEPLOY_MAC_OS_X_LIBRARY(QtXml
        TYPE
            Framework
        FRAMEWORKS
            QtCore
    )

    # Clean up the plugins that are deployed by mmacdeployqt
    # Note: it's not essential, it's merely to have 'clean' plugins

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(accessible qtaccessiblewidgets)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(bearer qgenericbearer)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(codecs qcncodecs)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(codecs qjpcodecs)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(codecs qkrcodecs)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(codecs qtwcodecs)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(graphicssystems qtracegraphicssystem)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(imageformats qgif)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(imageformats qico)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(imageformats qjpeg)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(imageformats qmng)
    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(imageformats qtiff)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(phonon_backend phonon_qt7)

    CLEAN_MAC_OS_X_PLUGIN_DEPLOYMENT(qmltooling qmldbg_tcp)

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

    # Make sure that the SQLite plugin refers to our embedded version

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND install_name_tool -id @rpath/PlugIns/sqldrivers/libqsqlite.dylib
                                                     ${SQLDRIVERS_DIR}/libqsqlite.dylib)

    # Make sure that the SQLite plugin refers to our embedded version of QtCore
    # and QtSql on which it depends

    FOREACH(QT_LIBRARY QtCore QtSql)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -change ${QT_LIBRARY_DIR}/${QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_LIBRARY}
                                                             @executable_path/../Frameworks/${QT_LIBRARY}.framework/Versions/${QT_VERSION_MAJOR}/${QT_LIBRARY}
                                                             ${SQLDRIVERS_DIR}/libqsqlite.dylib)
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

    INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.com DESTINATION bin)

    # MinGW files required by OpenCOR

    FOREACH(MINGW_LIBRARY mingwm10 libgcc_s_dw2-1)
        INSTALL(FILES ${QT_BINARY_DIR}/../../mingw/bin/${MINGW_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION bin)
    ENDFOREACH()

    # Qt files required by OpenCOR

    FOREACH(QT_LIBRARY phonon QtCLucene QtCore QtGui QtHelp QtNetwork QtOpenGL QtSql QtSvg QtWebKit QtXml)
        INSTALL(FILES ${QT_BINARY_DIR}/${QT_LIBRARY}${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION bin)
    ENDFOREACH()

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_PLUGINS_DIR}/sqldrivers/qsqlite${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION plugins/sqldrivers)

    # Batch and VBScript files to run OpenCOR (useful when downloading a ZIPped
    # version of OpenCOR)

    SET(BatFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bat)
    SET(VbsFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.vbs)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.bat.in ${BatFile})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.vbs.in ${VbsFile})

    INSTALL(FILES ${BatFile} DESTINATION .)
    INSTALL(FILES ${VbsFile} DESTINATION .)

    # File type association
    # Note: the calls to SHChangeNotify are to ensure that Windows refreshes
    #       file icons (so that it is clear to the user that an extension has
    #       been (un)registered...

    SET(CPACK_NSIS_DEFINES "!include ${CMAKE_SOURCE_DIR}\\\\distrib\\\\windows\\\\FileAssociation.nsh")

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

    SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/distrib/macosx/welcome.txt")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

    SET(INSTALL_DIR "/Applications/${PROJECT_NAME}")

    SET(CPACK_SET_DESTDIR TRUE)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_DIR})

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${INSTALL_DIR})

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/macosx/application.sh.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME} DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt files required by OpenCOR

    INSTALL(FILES ${QT_LIBRARY_DIR}/libphonon.so.4.4.0 RENAME libphonon.so.4 DESTINATION lib)

    FOREACH(QT_LIBRARY QtCLucene QtCore QtDBus QtGui QtHelp QtNetwork QtOpenGL QtSql QtSvg QtWebKit QtXml)
        INSTALL(FILES
                ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}
                RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR}
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

            INSTALL(FILES ${REAL_CORRECT_LIBRARY} RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY} DESTINATION lib)
        ENDIF()
    ENDFOREACH()

    # Qt configuration file to tell OpenCOR where to look for plugins

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/linux/qt.conf DESTINATION bin)

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.so DESTINATION plugins/sqldrivers)

    # Shell script to run OpenCOR

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/linux/application.sh.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME} DESTINATION . PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Some files common to all three platforms

INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/hodgkin_huxley_squid_axon_model_1952.cellml DESTINATION models PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/models/van_der_pol_model_1928.cellml DESTINATION models PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

INCLUDE(CPack)
