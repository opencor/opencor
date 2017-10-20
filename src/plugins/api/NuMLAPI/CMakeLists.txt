PROJECT(NuMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME libNuML)
SET(PACKAGE_VERSION 1.1.1)

# Version of our package's libraries

SET(MAJOR_LIBRARY_VERSION 1)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.1.1)

# Git tag for our library

SET(GIT_TAG v1.1.1)

# Package repository

SET(PACKAGE_REPOSITORY NuML)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY libnuml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY libnuml${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}numl${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

IF(USE_PREBUILT_NUMLAPI_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} a39db4db954fa9d7dc4ebe30d9bf3483d210cdc5
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES d7fca96349c39fce1db4f0a87780d1b75014a624
                                              119099b65ce3d3dfa7ad3cc419bf7a963d508420)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 987483fe81c3f16b6b05914a6a488c3e8d85e9a1
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 60783f45ecb6eac9938351d4793fba79ceb3283f
                                              ce4925551c1759fa812b9441d37e4b331e2caf81)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} ee9401bf34b1ce8dcdf95beca1a88dee3ab34a65
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e915e99c817217be7a2562c6bc3656cc42558d6c)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 5cdf38db5546509d211bcb964224c7490c9ff97a
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 3b179071772ed7820eb0bf4346ba3e75fe82ef27)
    ENDIF()
ELSE()
    # Build libNuML as an external project
    # Note #1: we need libSBML to be built, so we can access it, as well as some
    #          of its dependencies...
    # Note #2: we rely on CONFIGURE_COMMAND rather than CMAKE_GENERATOR and
    #          CMAKE_ARGS in our call to ExternalProject_Add() since the actual
    #          code is not under SOURCE_DIR, but under SOURCE_DIR/libnuml...

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(MODE release)
        ELSE()
            SET(MODE debug)
        ENDIF()
    ENDIF()

    SET(SBMLAPI_EXT_DIR ${CMAKE_SOURCE_DIR}/src/plugins/api/SBMLAPI/ext)

    IF(WIN32)
        SET(LIBSBML_INCLUDE_DIR ${SBMLAPI_EXT_DIR}/${MODE}/include)
        SET(LIBSBML_LIBRARY ${SBMLAPI_EXT_DIR}/${MODE}/lib/libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSE()
        SET(LIBSBML_INCLUDE_DIR ${SBMLAPI_EXT_DIR}/include)
        SET(LIBSBML_LIBRARY ${SBMLAPI_EXT_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.5${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    ENDIF()

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/NuML
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR}
                             ${CMAKE_ARGS}
                             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                             -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                             -DLIBSBML_INCLUDE_DIR=${LIBSBML_INCLUDE_DIR}
                             -DLIBSBML_LIBRARY=${LIBSBML_LIBRARY}
                             -DWITH_CPP_NAMESPACE=ON
                             -DWITH_SWIG=OFF
                             <SOURCE_DIR>/libnuml
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
        DEPENDS
            SBMLAPIPlugin
    )

    # Do some post-processing that is specific to each platform

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the libraries since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}numl${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our libNuML package

SET(NUMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(NUMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(NUMLAPI_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin
# Note: we would normally use '${PACKAGE_NAME}' rather than 'NuMLAPI', but it
#       can't be done here since the name of our package is different from that
#       of our plugin...

ADD_PLUGIN(NuMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/numlapiplugin.cpp
    HEADERS_MOC
        src/numlapiplugin.h
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
