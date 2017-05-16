PROJECT(LibXDiffPlugin)

# Name and version of our package

SET(PACKAGE_NAME LibXDiff)
SET(PACKAGE_VERSION "0.23")

# Version of our library

SET(MAJOR_LIBRARY_VERSION "0")
SET(LIBRARY_VERSION "${MAJOR_LIBRARY_VERSION}.23")

# Git tag for our library

SET(GIT_TAG "v0.23")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(SHARED_LIBRARY xdiff${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY xdiff${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}xdiff.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}xdiff${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
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

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LIBXDIFF_PACKAGE)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 5b0da2174aa1adbe444b81c208a963dc6d36a748
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES b3b38474a32cb9c6dae6f1ead272d50e3995169f
                            251408a73dfe5869f4f276c1bd1f960ae4575987
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 7badad3a17d4dcaaf433f6f45afa27de555dcded
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES edbd0889b522df147db280bed200e908782493e9
                            56b389809afb1b8812732053774077429e01faee
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 888dd1ad5bf3061b9b289c2a20a805def1f392b8
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 92a98280ed653e0450811200613ef97a920d9efc
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} e8ef3ff0a764c189f52340591c18efd7cd07820b
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 6f63edeea34975d9fc80aae3724ccbad1c37797b
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings

    IF(WIN32)
        SET(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4101 /wd4244 /wd4267")
    ENDIF()

    # Build LibXDiff as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libxdiff
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
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
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        SET(FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}xdiff${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_LIB_DIR}/${SHARED_LIBRARY}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our LibXDiff package

SET(LIBXDIFF_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(LIBXDIFF_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(LIBXDIFF_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(LIBXDIFF_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/libxdiffplugin.cpp
    HEADERS_MOC
        src/libxdiffplugin.h
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
