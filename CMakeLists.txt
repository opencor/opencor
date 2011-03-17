PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Version of OpenCOR

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

FILE(WRITE ${CMAKE_BINARY_DIR}/version.txt ${VERSION})

# Type of build

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    SET(DEBUG_MODE ON)
ELSE()
    SET(DEBUG_MODE OFF)
ENDIF()

# Files that make up the GUI version of OpenCOR

SET(SOURCES
    src/main.cpp
    src/misc/cellml.cpp
    src/misc/common.cpp
    src/misc/documentmanager.cpp
    src/misc/utils.cpp
    src/ui/centralwidget.cpp
    src/ui/helpwindow.cpp
    src/ui/mainwindow.cpp
    src/ui/edit/viewerwindow.cpp
    src/ui/organise/cellmlmodelrepositorywindow.cpp
    src/ui/organise/filebrowserwindow.cpp
    src/ui/organise/fileorganiserwindow.cpp
    src/widget/docktoolbar.cpp
    src/widget/dockwidget.cpp
    src/widget/helpwidget.cpp
    src/widget/edit/mmlviewerwidget.cpp
    src/widget/misc/commonwidget.cpp
    src/widget/organise/cellmlmodelrepositorywidget.cpp
    src/widget/organise/filebrowserwidget.cpp
    src/widget/organise/fileorganiserwidget.cpp
)

SET(HEADERS
    src/misc/cellml.h
    src/misc/common.h
    src/misc/documentmanager.h
    src/misc/utils.h
    src/widget/misc/commonwidget.h
)

SET(HEADERS_MOC
    src/ui/centralwidget.h
    src/ui/helpwindow.h
    src/ui/mainwindow.h
    src/ui/edit/viewerwindow.h
    src/ui/organise/cellmlmodelrepositorywindow.h
    src/ui/organise/filebrowserwindow.h
    src/ui/organise/fileorganiserwindow.h
    src/widget/docktoolbar.h
    src/widget/dockwidget.h
    src/widget/helpwidget.h
    src/widget/edit/mmlviewerwidget.h
    src/widget/organise/cellmlmodelrepositorywidget.h
    src/widget/organise/filebrowserwidget.h
    src/widget/organise/fileorganiserwidget.h
)

SET(UIS
    src/ui/centralwidget.ui
    src/ui/helpwindow.ui
    src/ui/mainwindow.ui
    src/ui/edit/viewerwindow.ui
    src/ui/organise/cellmlmodelrepositorywindow.ui
    src/ui/organise/filebrowserwindow.ui
    src/ui/organise/fileorganiserwindow.ui
)

SET(RESOURCES
    res/${PROJECT_NAME}.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(src/misc)
INCLUDE_DIRECTORIES(src/ui)
INCLUDE_DIRECTORIES(src/ui/edit)
INCLUDE_DIRECTORIES(src/ui/organise)
INCLUDE_DIRECTORIES(src/widget)
INCLUDE_DIRECTORIES(src/widget/edit)
INCLUDE_DIRECTORIES(src/widget/misc)
INCLUDE_DIRECTORIES(src/widget/organise)

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

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

EXECUTE_PROCESS(COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} doc/${PROJECT_NAME}.qhcp
                                                           -o build/${PROJECT_NAME}.qhc
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Update the translation (.ts) files and generate the language (.qm) files
# which will be embedded in the executable as resources

SET(LANGUAGE_FILES
    ${PROJECT_NAME}_fr
)

FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
    SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

    EXECUTE_PROCESS(COMMAND ${QT_LUPDATE_EXECUTABLE} -no-obsolete
                                                     ${SOURCES} ${HEADERS} ${HEADERS_MOC} ${UIS}
                                                 -ts ${TS_FILE}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS_FILE}
                                                  -qm build/${LANGUAGE_FILE}.qm
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDFOREACH()

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

# Build the third-party libraries used by the GUI version of OpenCOR

SET(3RDPARTYLIBS LibQxt QScintilla QtMmlWidget QtSingleApplication)

FOREACH(3RDPARTYLIB ${3RDPARTYLIBS})
    SET(3RDPARTYLIB_DIR 3rdparty/${3RDPARTYLIB})
    SET(3RDPARTYLIB_SUBDIRS)
    # Note: the above variable is defined here, but is actually set when
    #       processing the CMakeLists.txt file of the third-party library

    ADD_SUBDIRECTORY(${3RDPARTYLIB_DIR})

    IF("${3RDPARTYLIB_SUBDIRS}" STRGREATER "")
        # The third-party library has at least one sub-directory, so add it/them

        FOREACH(3RDPARTYLIB_SUBDIR ${3RDPARTYLIB_SUBDIRS})
            INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR}/${3RDPARTYLIB_SUBDIR})
        ENDFOREACH()
    ELSE()
        # The third-party library doesn't have any sub-directory, so just add
        # its directory and that's it

        INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR})
    ENDIF()
ENDFOREACH()

# Rules to build the GUI version of OpenCOR

#SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

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
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}

    ${QT_QTHELP_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTXML_LIBRARY}

    ${3RDPARTYLIBS}
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

# Compiler and linker options

SET(CMAKE_CXX_FLAGS "-Wall")
SET(LINK_FLAGS_PROPERTIES)

IF(${DEBUG_MODE})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math")
ENDIF()

IF(WIN32)
    SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-enable-auto-import")
    # Note: -Wl,-enable-auto-import allows us to use our various third-party
    #       libraries without having to worry about importing their functions,
    #       etc.
ENDIF()

IF(NOT APPLE AND NOT ${DEBUG_MODE})
    SET(LINK_FLAGS_PROPERTIES "${LINK_FLAGS_PROPERTIES} -Wl,-s")
    # Note #1: -Wl,-s strips all the symbols, thus reducing the final size of
    #          OpenCOR
    # Note #2: the above linking option has become obsolete on Mac OS X, so...
ENDIF()

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
    # Note: this prevents us from getting a warning when everything is actually OK

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} PRE_BUILD
                       COMMAND rm -f ${PROJECT_NAME}.app/Contents/Resources/qt.conf
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Use macdeployqt to have all the Qt files required by OpenCOR added to the
    # bundle

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${QT_BINARY_DIR}/macdeployqt ${PROJECT_NAME}.app
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Add the SQLite plugin to the bundle (required to see the help file)
    # Note: this would, ideally, be done as part of the packaging, but we do it
    #       here, since it allows us to test things as if Qt wasn't installed
    #       on our development Mac OS X machine (which is useful since we
    #       cannot have a virtual machine with a 'blank' Mac OS X system on it
    #       unlike with Windows and Linux)

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND mkdir -p ${PROJECT_NAME}.app/Contents/PlugIns/sqldrivers
                       COMMAND cp ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.dylib
                                  ${PROJECT_NAME}.app/Contents/PlugIns/sqldrivers
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Make sure that the SQLite plugin refers to our embedded version of QtCore
    # and QtSql which it needs

    FOREACH(LIB QtCore QtSql)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND install_name_tool -change ${LIB}.framework/Versions/${QT_VERSION_MAJOR}/${LIB}
                                                             @executable_path/../Frameworks/${LIB}.framework/Versions/${QT_VERSION_MAJOR}/${LIB}
                                                             ../PlugIns/sqldrivers/libqsqlite.dylib
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Frameworks)
    ENDFOREACH()
ENDIF()

# Rules to package OpenCOR (using CPack)

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

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME} DESTINATION . PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt files required by OpenCOR

    FOREACH(LIB QtCLucene QtCore QtDBus QtGui QtHelp QtNetwork QtSql QtWebKit QtXml)
        INSTALL(FILES
            ${QT_LIBRARY_DIR}/lib${LIB}.so.${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}
            RENAME lib${LIB}.so.${QT_VERSION_MAJOR}
            DESTINATION lib
        )
    ENDFOREACH()

    # Additional files required by Qt, but not available on a 'blank' version
    # of Ubuntu

    INSTALL(FILES /usr/lib/libaudio.so.2.4 RENAME libaudio.so.2 DESTINATION lib)
    INSTALL(FILES /usr/lib/libphonon.so.4.4.0 RENAME libphonon.so.4 DESTINATION lib)

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
