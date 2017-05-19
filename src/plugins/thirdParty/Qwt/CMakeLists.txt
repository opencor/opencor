PROJECT(QwtPlugin)

# Name and version of our package

SET(PACKAGE_NAME Qwt)
SET(PACKAGE_VERSION 6.1.3)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 6)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.3)

# Git tag for our library

SET(GIT_TAG v6.1.3)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(QWT_SHARED_LIBRARY qwt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QWT_IMPORT_LIBRARY qwt${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(QWTMATHML_SHARED_LIBRARY qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QWTMATHML_IMPORT_LIBRARY qwtmathml${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(QWT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(QWT_IMPORT_LIBRARY ${QWT_SHARED_LIBRARY})

    SET(QWTMATHML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(QWTMATHML_IMPORT_LIBRARY ${QWTMATHML_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${QWT_SHARED_LIBRARY}
    ${QWTMATHML_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${QWT_SHARED_LIBRARY}
    lib/${QWTMATHML_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${QWT_IMPORT_LIBRARY}
        lib/${QWTMATHML_IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_QWT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                          XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                                          XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Set platform specific configuration options and tool names

    IF(WIN32)
        SET(MAKE_EXECUTABLE nmake)

        IF(RELEASE_MODE)
            SET(MAKEFILE_TARGET release)
        ELSE()
            SET(MAKEFILE_TARGET debug)
        ENDIF()

        SET(QUIET_MAKE -c)
    ELSE()
        SET(MAKE_EXECUTABLE make)
        SET(QUIET_MAKE --quiet)
    ENDIF()

    # Build Qwt as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    GET_PROPERTY(EP_BASE DIRECTORY PROPERTY EP_BASE)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qwt
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QT_QMAKE} <SOURCE_DIR>/qwt.pro CONFIG+=qt_no_framework OBJECTS_DIR=${EP_BASE}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_EXECUTABLE} ${MAKEFILE_TARGET} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} install ${QUIET_MAKE}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${QWT_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${QWTMATHML_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyQwtLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwt${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyQwtmathmlLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${QWTMATHML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}qwtmathml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${QWTMATHML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        IF(APPLE)
            # Set the RPATH information to enable linking independent of library
            # location

            ExternalProject_Add_Step(${PACKAGE_BUILD} SetRPath
                                     COMMAND install_name_tool -change ${QWT_SHARED_LIBRARY}
                                                                       @rpath/${QWT_SHARED_LIBRARY}
                                                                       ${QWTMATHML_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES CopyQwtmathmlLibrary)
        ENDIF()
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our Qwt package

SET(QWT_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(QWT_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(QWT_QWT_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QWT_QWTMATHML_LIBRARY ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY} PARENT_SCOPE)
SET(QWT_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${QWT_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${QWTMATHML_IMPORT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

SET(QWT_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qwtplugin.cpp
    HEADERS_MOC
        src/qwtplugin.h
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
