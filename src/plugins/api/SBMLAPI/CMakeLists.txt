PROJECT(SBMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME SBMLAPI)
SET(PACKAGE_VERSION 5.15.0)

# Version of our package's libraries

SET(MAJOR_LIBRARY_VERSION 5)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.15.0)

# Git tag for our library

SET(GIT_TAG v5.15.0)

# Package repository and release tag

SET(PACKAGE_REPOSITORY libsbml)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY libsbml${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY libsbml${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
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

IF(USE_PREBUILT_SBMLAPI_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 3dc200a077c28701942b13fdabb5c0c043c68ed9
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES e58822c306fab96deca6fd93389e418e0bdbcfd5
                                              eb10bf3c7b87da6329d02c4530ad2d5330e2d9b7)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 2782400a91302875a897cd713bb4f385f15dd733
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES ad5c68fc1e4b68092eb7ebde1817b0ff7aa7bd41
                                              46c944193e41a9e7f4a866203e57b264b929f1cc)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 451c3ced1bf216343ae3ee062cafc80f7782ad64
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d6db9d28fa73c4121bae4d28e5f75b46f8050210)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} d2b0aee93a8aa74a4a15ae95e6090162eff7e7c9
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES ef54aa2cb375db442f2c1b4a0416b1822d8b1c51)
    ENDIF()
ELSE()
    # Download some dependencies for Windows, if needed

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(MODE release)
        ELSE()
            SET(MODE debug)
        ENDIF()

        SET(DEPENDENCIES_NAME libSBML_dependencies_vs14_${MODE}_x64)
        SET(DEPENDENCIES_FILENAME ${DEPENDENCIES_NAME}.zip)
        SET(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/src/plugins/api/${PACKAGE_NAME}/ext/dependencies)
        SET(DEPENDENCIES_MODE_DIR ${DEPENDENCIES_DIR}/${MODE})

        IF(NOT EXISTS ${DEPENDENCIES_MODE_DIR})
            SET(REAL_DEPENDENCIES_FILENAME ${DEPENDENCIES_DIR}/${DEPENDENCIES_FILENAME})

            FILE(DOWNLOAD "https://github.com/opencor/libsbml/releases/download/${GIT_TAG}/${DEPENDENCIES_FILENAME}" ${REAL_DEPENDENCIES_FILENAME}
                 SHOW_PROGRESS STATUS STATUS)

            LIST(GET STATUS 0 STATUS_CODE)

            IF(${STATUS_CODE} EQUAL 0)
                EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar -xzf ${REAL_DEPENDENCIES_FILENAME}
                                WORKING_DIRECTORY ${DEPENDENCIES_DIR}
                                OUTPUT_QUIET)
                EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E rename "libSBML Dependencies-1.0.0-b1-win64" ${MODE}
                                WORKING_DIRECTORY ${DEPENDENCIES_DIR}
                                OUTPUT_QUIET)

                FILE(REMOVE ${REAL_DEPENDENCIES_FILENAME})
            ELSE()
                FILE(REMOVE ${REAL_DEPENDENCIES_FILENAME})
                # Note: this is in case we had an HTTP error of sorts, in which case we
                #       would end up with an empty file...

                MESSAGE(FATAL_ERROR "The libSBML depencies could not be retrieved...")
            ENDIF()
        ENDIF()
    ENDIF()

    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings

    IF(APPLE)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-switch")
    ENDIF()

    # Build the SBML API as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libsbml
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            -DLIBSBML_DEPENDENCY_DIR=${DEPENDENCIES_MODE_DIR}
            -DWITH_CPP_NAMESPACE=ON
            -DWITH_SWIG=OFF
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
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
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}sbml${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
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

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our SBML API package

SET(SBMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(SBMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(SBMLAPI_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(SBMLAPI_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/sbmlapiplugin.cpp
    HEADERS_MOC
        src/sbmlapiplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include

        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    TESTS
        tests
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
