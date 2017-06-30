PROJECT(BioSignalMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME BioSignalMLAPI)
SET(PACKAGE_VERSION 1.0)

# Version of our package's libraries

SET(MAJOR_BIOSIGNALML_LIBRARY_VERSION 0)
SET(BIOSIGNALML_LIBRARY_VERSION ${MAJOR_BIOSIGNALML_LIBRARY_VERSION}.8)

SET(MAJOR_TYPEDOBJECT_LIBRARY_VERSION 1)
SET(TYPEDOBJECT_LIBRARY_VERSION ${MAJOR_TYPEDOBJECT_LIBRARY_VERSION}.1)

# Git tags for our libraries

SET(GIT_TAG_BIOSIGNALML v0.8.4)
SET(GIT_TAG_TYPEDOBJECT v1.1.2)

# Package repository and release tag

SET(PACKAGE_REPOSITORY libbiosignalml)
SET(RELEASE_TAG v0.8.4)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(BIOSIGNALML_SHARED_LIBRARY biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(BIOSIGNALML_IMPORT_LIBRARY biosignalml${CMAKE_IMPORT_LIBRARY_SUFFIX})

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
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e2ca2a020750760c611b72b35edfa956bc55c593
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 379e38afc9eb015b54cd327aac0d725f25fb95fc
                                              c18a129f49429083c40c3621f61256ddcbe83c05
                                              c4bf89a74703d73220e7b8be29944c409fca1a06
                                              a10d81c681fae3efc41d7855c4250f7333821256)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} f01b5c1219ab792476ae6eaadf3be0329784c948
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 6ffeea24b14d9769e5b50aed663ce5091cd43101
                                              136f63617ad96a272d56b0e13f3d0b090f096d75
                                              0dcde52f7037472d6d642e71f9e616f0fac18095
                                              556aa50cf56cd0e934a5cd7c392cef4be1d01d1c)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} a9842f187002c5b9bae273bc2cd3224592f87ef2
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 7141316b23d7cace02e72466808f6710ad40a4b8
                                          8e6ca96135ae670a53422a44827579740108d559)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 7e7879d462e34253d53f99a5fa7cca022349a7b7
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES ecb1f7171e724140b57114c5de6af8b0f36871de
                                          47f5d2e0769863d9038c35879e7febcd308e6e30)
    ENDIF()
ELSE()
    # Build typedobject as an external project

    ExternalProject_Add(TypedObjectBuild
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/typedobject
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/typedobject
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

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libbiosignalml
        GIT_TAG
            ${GIT_TAG_BIOSIGNALML}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -Dtypedobject_DIR=<INSTALL_DIR>/lib/cmake/typedobject
            -Dtypedobject_INCLUDE_DIR=<INSTALL_DIR>/include
            -Dtypedobject_LIBRARY=<INSTALL_DIR>/lib/${TYPEDOBJECT_IMPORT_LIBRARY}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${BIOSIGNALML_IMPORT_LIBRARY}
        DEPENDS
            TypedObjectBuild
    )

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLLs
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${BIOSIGNALML_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${TYPEDOBJECT_SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibraries
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${BIOSIGNALML_SHARED_LIBRARY}
                                                                       ${TYPEDOBJECT_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}biosignalml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${BIOSIGNALML_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${BIOSIGNALML_SHARED_LIBRARY}
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
    TESTS
        tests
    DEPENDS_ON
        ${DEPENDS_ON}
)
