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
    SET(IMPORT_LIBRARY ${CRYPTO_SHARED_LIBRARY})
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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} a3c108bc2c85756fd92c10b76a8e751ec3e53ac9
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 17f68b4ecad125c5fd499663086cd71a105e80e5
                                              6c4a87940e8e1dbed81f0fd664f6260a67354566)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e4ca2b71da3b3d4ec6015e76c99072da8f16afe0
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 2b13ca4e6ea6c1310039f5b63137ed2c8bf3e408
                                              3d72a1f4726ce07721b7e52bf815b6fec3f6429f)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f8e47fcabe1a8f46dd888f654cd31e825483d528
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f181f3d0cc640b06ac8b630ee9150da299ecd25a)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 03127ee34cd3c1b7b70cf27c4ebdfcbbeaaf7fce
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES bbb1b66d8fa7a1e42a732aa32119f8cef173e5fd)
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Set platform specific configuration options and tool names

    IF(WIN32)
        SET(MAKE_EXECUTABLE nmake)
        SET(QUIET_MAKE -c)
    ELSE()
        SET(MAKE_EXECUTABLE make)
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
            ${MAKE_EXECUTABLE} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} install ${QUIET_MAKE}
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
