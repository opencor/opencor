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

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    IF(RELEASE_MODE)
        SET(DEBUG_TAG)
    ELSE()
        SET(DEBUG_TAG d)
    ENDIF()

    SET(QWT_SHARED_LIBRARY qwt${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QWT_IMPORT_LIBRARY qwt${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(QWTMATHML_SHARED_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(QWTMATHML_IMPORT_LIBRARY qwtmathml${DEBUG_TAG}${CMAKE_IMPORT_LIBRARY_SUFFIX})
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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} d35a7ee6b343db879953658a4faab6a50aa3d030
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES ec304b302f2daf62b7ea44185a5461b2f9bd7b9a
                                              4ba1e9eaa888b79c25dc0f2d1403cf6dff0ce292
                                              61fcf47e838787e6225d94c7dfca093c16d0fa1a
                                              99fd797350511d3f793ce560886deb30c83dd23f)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b828771e900c071b28fbf0c03b665d665a97b284
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 1a09e586dfad979a4048520211f0a7a76056b390
                                              fa054ae8fe0eb9fdb83d6dc0fb80adf9cc818322
                                              84a034e96ae14a916f63949a1d0349eb93781e58
                                              2ecb9acbcd715f73eba5c11e7b5cdaafb812d28e)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 65704e0fd730903b7ba31651094550f4bcc2557f
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2f2341decf3a24faedb6f4e83165226266eba5f9
                                          d5787ce81a8958a4a7453cc9a95cf77222d08a49)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 494191814485a53d2b70619d575f1da23cb0ef73
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 98c1869712832f83bff34774bc8c9d1dddec8fc7
                                          eb652a5804420c45fe9ed843c0312f1eb1144ce5)
    ENDIF()
ELSE()
    # Set platform specific configuration options

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(MAKEFILE_TARGET release)
        ELSE()
            SET(MAKEFILE_TARGET debug)
        ENDIF()
    ENDIF()

    # Build Qwt as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/qwt
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} <SOURCE_DIR> CONFIG+=qt_no_framework OBJECTS_DIR=${EXTERNAL_PROJECT_BUILD_DIR}/Build/${PACKAGE_BUILD} INSTALL_DIR=<INSTALL_DIR>
        BUILD_COMMAND
            ${MAKE_JOM_COMMAND} ${MAKEFILE_TARGET}
        INSTALL_COMMAND
            ${MAKE_JOM_COMMAND} install
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
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWT_SHARED_LIBRARY}
                                     COMMAND install_name_tool -add_rpath @executable_path/../Frameworks ${QWTMATHML_SHARED_LIBRARY}
                                     COMMAND install_name_tool -delete_rpath ${QT_LIBRARY_DIR} ${QWTMATHML_SHARED_LIBRARY}
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

    SET(DEPENDS_ON ${PACKAGE_BUILD})
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

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/qwtplugin.cpp
    HEADERS_MOC
        src/qwtplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
