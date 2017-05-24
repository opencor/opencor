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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} ea83cfd4e3aa6a75ec778d16b48f3ab3663a0d96
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 441549bde810298381a7f7bf37af6c90521a7117
                                              c4612b483f4e7977f8ff1fc6466c6d368673d6c9)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 794f5640900618b1ec0fd615e2bab0248330d9af
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 02909bd0333869d1c2c6e7bd962e3496f49d5b5e
                                              8aa76a19c5dd301528aa7b92ed750b1c335c1014)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f1f7a1e33bc3502b577151b35589aa61c265b90c
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e264ff3c575eaa197963776a32e36f2b1842e2c3)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 55a45a3a724762e7a3eef0eab855d5a238e609db
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3d9d4c674d53825a97a934dfb282813a465b488d)
    ENDIF()
ELSE()
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

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qscintilla
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QT_QMAKE} <SOURCE_DIR>/Qt4Qt5/qscintilla.pro OBJECTS_DIR=${EP_BASE}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} install ${QUIET_MAKE}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qscintilla2_qt${QT_VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our QScintilla package

SET(QSCINTILLA_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(QSCINTILLA_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(QSCINTILLA_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qscintillaplugin.cpp
    HEADERS_MOC
        src/qscintillaplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
