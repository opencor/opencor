PROJECT(SBMLAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME libSBML)
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

# Name of our package's shared library (and of its import library, if any)

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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 6cbb5a68c917a1a2d9dfcb2b6cbb0257046565f0
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b3c8451b899bf46bfc207d1f64c8edddd6a6c9a8
                                              17c1bde40c640ba944ab32d4cccf00c77de8bd12)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 4236a08135c221cfb42bb268fe440a16b165122a
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b30f42855059678ae2f0e3c085b3ce6fb95d6e6c
                                              bef49ff254f04225c57bf621e62254549e445b07)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} fd9b682bc8ff97b2f411f0efd5ba2b92b6167635
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2dc2138e7eccfc1750bf8815e76205a15d7e8f07)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 66904b058ae231720603c640ef375a7e08edd573
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES a7358ac3d067bcd921ec19caff383b62f4a8b86f)
    ENDIF()
ELSE()
    # Download some dependencies for Windows, if needed

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(MODE release)
        ELSE()
            SET(MODE debug)
        ENDIF()

        SET(DEPENDENCIES_FILENAME libSBML_dependencies_vs14_${MODE}_x64.zip)
        SET(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/src/plugins/api/SBMLAPI/ext/dependencies)
        SET(DEPENDENCIES_MODE_DIR ${DEPENDENCIES_DIR}/${MODE})

        IF(NOT EXISTS ${DEPENDENCIES_MODE_DIR})
            MESSAGE("Retrieving the 'libSBML' dependencies...")

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

    # Ignore some warnings

    IF(APPLE)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-switch")
    ENDIF()

    # Build libSBML as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libsbml
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${DEFINE_CMAKE_APPLE_OPTIONS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
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

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our libSBML package

SET(SBMLAPI_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(SBMLAPI_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(SBMLAPI_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin
# Note: we would normally use '${PACKAGE_NAME}' rather than 'SBMLAPI', but it
#       can't be done here since the name of our package is different from that
#       of our plugin...

ADD_PLUGIN(SBMLAPI
    SOURCES
        ../../plugininfo.cpp

        src/sbmlapiplugin.cpp
    HEADERS_MOC
        src/sbmlapiplugin.h
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
