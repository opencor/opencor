PROJECT(BioSignalMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME BioSignalMLAPI)
SET(PACKAGE_VERSION 1.0)

# Version of our package's libraries

SET(MAJOR_BIOSIGNALML_LIBRARY_VERSION 0)
SET(BIOSIGNALML_LIBRARY_VERSION 0.8)

SET(MAJOR_TYPEDOBJECT_LIBRARY_VERSION 1)
SET(TYPEDOBJECT_LIBRARY_VERSION 1.1)

# Git tags for our libraries

SET(GIT_TAG_BIOSIGNALML v0.8.3)
SET(GIT_TAG_TYPEDOBJECT v1.1.1)

# Package repository and release tag

SET(PACKAGE_REPOSITORY libbiosignalml)
SET(RELEASE_TAG 0.8.3)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(BIOSIGNALML_SHARED_LIBRARY libbiosignalml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(BIOSIGNALML_IMPORT_LIBRARY libbiosignalml${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(TYPEDOBJECT_SHARED_LIBRARY typedobject${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(TYPEDOBJECT_IMPORT_LIBRARY typedobject${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(BIOSIGNALML_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_BIOSIGNALML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(BIOSIGNALML_IMPORT_LIBRARY ${BIOSIGNALML_SHARED_LIBRARY})

    SET(TYPEDOBJECT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}typedobject${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_TYPEDOBJECT_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(TYPEDOBJECT_IMPORT_LIBRARY ${TYPEDOBJECT_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${BIOSIGNALML_SHARED_LIBRARY}
    ${TYPEDOBJECT_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${BIOSIGNALML_SHARED_LIBRARY}
    lib/${TYPEDOBJECT_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${BIOSIGNALML_IMPORT_LIBRARY}
        lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_BIOSIGNALMLAPI_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            ##
        ELSE()
            ##
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 7b3df53034d4a556385c8d58b21772c779d601df
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 49cec96abbcc4556a407808e71dabfc49e89a2fa
                                          45c0881a12f28dc54dcbb80d586946aced288246)
    ELSE()
        ##
    ENDIF()
ELSE()

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    # First build typedobject library as an external project

    ExternalProject_Add(TypedObjectBuild
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/typedobject
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            git@github.com:opencor/typedobject.git
        GIT_TAG
            ${GIT_TAG_TYPEDOBJECT}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
    )


    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            TypedObjectBuild
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            git@github.com:opencor/libbiosignalml.git
        GIT_TAG
            ${GIT_TAG_BIOSIGNALML}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

            # Specify location of our newly built typedobject
            -Dtypedobject_INCLUDE_DIR=<INSTALL_DIR>/include
            -Dtypedobject_LIBRARY=<INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
            -Dtypedobject_DIR=<INSTALL_DIR>/lib/cmake/typedobject
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${BIOSIGNALML_IMPORT_LIBRARY}
    )

    IF(NOT WIN32)
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyAnnotoolsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${BIOSIGNALML_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${BIOSIGNALML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${BIOSIGNALML_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyCcgsLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${TYPEDOBJECT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}typedobject${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${TYPEDOBJECT_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${TYPEDOBJECT_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/biosignalmlapiplugin.cpp
    HEADERS_MOC
        src/biosignalmlapiplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
