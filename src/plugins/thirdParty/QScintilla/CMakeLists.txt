PROJECT(QScintillaPlugin)

# Name and version of our package

SET(PACKAGE_NAME QScintilla)
SET(PACKAGE_VERSION 2.10)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 13)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0.0)

# Git tag for our library

SET(GIT_TAG v2.10)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QSCINTILLA_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 5fee3d470a94caf52a9a5845d9d68856850c791d
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 306e264145dbcbcc246775edbc16f38b37ec8aef
                                              590391bab7acc405ef470a12e0190b3411f3a1fb)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 2aaa8d468a60010a3f2be04a2826d1f4884b8fa0
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES cc1541c2177055970e59bc186f7ecd4488cd49db
                                              8d3d43de2213ac524ad8bb530e25d23b3ec05313)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 3736b481af073b9cf10f19e240c2d7c9fa97beae
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f181f3d0cc640b06ac8b630ee9150da299ecd25a)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} de155cb55fc3a89f402adf03ebd0e2df4db6d623
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES bbb1b66d8fa7a1e42a732aa32119f8cef173e5fd)
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Set platform specific configuration options and tool names

    IF(WIN32)
        SET(MAKE_EXECUTABLE nmake)

        IF(RELEASE_MODE)
            SET(MAKEFILE Makefile.Release)
        ELSE()
            SET(MAKEFILE Makefile.Debug)
        ENDIF()

        SET(QUIET_MAKE -c)
    ELSE()
        SET(MAKE_EXECUTABLE make)
        SET(MAKEFILE Makefile)
        SET(QUIET_MAKE --quiet)
    ENDIF()

    # Build QScintilla as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    SET(PACKAGE_BUILD_DIR ${PROJECT_SOURCE_DIR}/ext/proj)

    ExternalProject_Add(${PACKAGE_BUILD}
        DOWNLOAD_DIR
            ${PACKAGE_BUILD_DIR}
        SOURCE_DIR
            ${PACKAGE_BUILD_DIR}
        BINARY_DIR
            ${PACKAGE_BUILD_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qscintilla
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QT_QMAKE} Qt4Qt5/qscintilla.pro INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} install ${QUIET_MAKE}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our QScintilla package

SET(QSCINTILLA_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(QSCINTILLA_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(QSCINTILLA_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(QSCINTILLA_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qscintillaplugin.cpp
    HEADERS_MOC
        src/qscintillaplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include

        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
