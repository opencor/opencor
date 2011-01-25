PROJECT(OpenCOR)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Version of OpenCOR

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 1)

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/build/version.txt ${VERSION})

# Files that make up OpenCOR

SET(SOURCES
    src/main.cpp
    src/misc/utils.cpp
    src/ui/mainwindow.cpp
    src/ui/helpwindow.cpp
    src/widget/helpwidget.cpp
)

SET(HEADERS
    src/misc/utils.h
)

SET(HEADERS_MOC
    src/ui/mainwindow.h
    src/ui/helpwindow.h
    src/widget/helpwidget.h
)

SET(UIS
    src/ui/mainwindow.ui
    src/ui/helpwindow.ui
)

SET(RESOURCES
    res/${PROJECT_NAME}.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(src/misc)
INCLUDE_DIRECTORIES(src/ui)
INCLUDE_DIRECTORIES(src/widget)

# Qt specific

FIND_PACKAGE(Qt4 4.6.0 REQUIRED)

IF(WIN32)
    SET(QT_USE_QTMAIN TRUE)
    # Note: the above ensures that winmain@16 is available on Windows
ENDIF()

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
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

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
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${TS_FILE}
                                                  -qm build/${LANGUAGE_FILE}.qm
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDFOREACH()

# Set the application icon, but only for Windows and Mac OS X, since in the
# case of Linux, it's done through res/OpenCOR.qrc

IF(WIN32)
    SET(IconObjectFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_icon.o)


    ADD_CUSTOM_COMMAND(OUTPUT ${IconObjectFile}
                       COMMAND windres -I ${CMAKE_CURRENT_SOURCE_DIR} -i ${CMAKE_CURRENT_SOURCE_DIR}/res/${PROJECT_NAME}.rc
                                       -o ${IconObjectFile}
                      )

    SET(SOURCES ${SOURCES} ${IconObjectFile})
ELSEIF(APPLE)
    SET(IconFile ${PROJECT_NAME}.icns)

    SET(MACOSX_BUNDLE_ICON_FILE ${IconFile})

    SET_SOURCE_FILES_PROPERTIES(res/${IconFile} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    SET(SOURCES ${SOURCES} res/${IconFile})
ENDIF()

# Build the third-party libraries used by OpenCOR

SET(3RDPARTYLIBS LibQxt QtSingleApplication)

FOREACH(3RDPARTYLIB ${3RDPARTYLIBS})
    SET(3RDPARTYLIB_DIR 3rdparty/${3RDPARTYLIB})
    SET(3RDPARTYLIB_SUBDIRS)

    ADD_SUBDIRECTORY(${3RDPARTYLIB_DIR})

    IF("${3RDPARTYLIB_SUBDIRS}" STRGREATER "")
        # The third-party library has at least one sub-directory, so add
        # it/them

        FOREACH(3RDPARTYLIB_SUBDIR ${3RDPARTYLIB_SUBDIRS})
            INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR}/${3RDPARTYLIB_SUBDIR})
        ENDFOREACH()
    ELSE()
        # The third-party library doesn't have any sub-directory, so just add
        # the third-party library directory and that's it

        INCLUDE_DIRECTORIES(${3RDPARTYLIB_DIR})
    ENDIF()
ENDFOREACH()

# Rules to build OpenCOR

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

ADD_DEFINITIONS(-Wall -O2 -ffast-math)

IF(WIN32)
    ADD_DEFINITIONS(-enable-auto-import)

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--enable-auto-import")
ENDIF()

# Some post-processing specific to Mac OS X which ensures that OpenCOR can be
# used on any Mac OS X machine that doesn't have Qt

IF(APPLE)
    # Use macdeployqt to have all the Qt files required by OpenCOR added to the
    # bundle

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${QT_BINARY_DIR}/macdeployqt ${PROJECT_NAME}.app
                       WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # Add the SQLite plugin to the bundle (required to see the help file)

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND mkdir -p ${PROJECT_NAME}.app/Contents/plugins/sqldrivers
                       COMMAND cp ${QT_BINARY_DIR}/../../Applications/Qt/plugins/sqldrivers/libqsqlite.dylib ${PROJECT_NAME}.app/Contents/plugins/sqldrivers
                       WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
ENDIF()

# Rules to package OpenCOR (using CPack)

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Alan Garny (alan.garny@dpag.ox.ac.uk)")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform CellML-based modelling environment")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/distrib/readMe.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

IF(WIN32)
    # Select NSIS and ZIP as the packagers on Windows

    SET(CPACK_GENERATOR NSIS ZIP)

    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/distrib/licensing.txt")

    # OpenCOR itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # MinGW files required by OpenCOR

    FOREACH(DLL mingwm10 libgcc_s_dw2-1)
        INSTALL(FILES ${QT_BINARY_DIR}/../../mingw/bin/${DLL}.dll DESTINATION bin)
    ENDFOREACH()

    # Qt files required by OpenCOR

    FOREACH(DLL phonon QtCLucene QtCore QtGui QtHelp QtNetwork QtSql QtWebKit QtXml)
        INSTALL(FILES ${QT_BINARY_DIR}/${DLL}${QT_VERSION_MAJOR}.dll DESTINATION bin)
    ENDFOREACH()

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_BINARY_DIR}/../plugins/sqldrivers/qsqlite${QT_VERSION_MAJOR}.dll DESTINATION bin/sqldrivers)

    # VBScript file to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/distrib/${PROJECT_NAME}.vbs DESTINATION .)
ELSEIF(APPLE)
    # Select PackageMaker and ZIP as the packagers on Mac OS X

    SET(CPACK_GENERATOR PackageMaker ZIP)

    SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/distrib/welcomeForMacOSXInstaller.txt")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/distrib/readMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/distrib/licensing.txt")

    SET(INSTALL_DIR "/Applications/${PROJECT_NAME}")

    SET(CPACK_SET_DESTDIR TRUE)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_DIR})

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${INSTALL_DIR})

    # Shell script to run OpenCOR (useful when downloading a ZIPped version of
    # OpenCOR)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/distrib/${PROJECT_NAME} DESTINATION . PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
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

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/distrib/qt.conf DESTINATION bin)

    # SQLite plugin (required to see the help file)

    INSTALL(FILES ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.so DESTINATION plugins/sqldrivers)

    # Shell script to run OpenCOR

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/distrib/${PROJECT_NAME} DESTINATION . PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

INCLUDE(CPack)
