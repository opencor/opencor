PROJECT(zlibPlugin)

# Version that we want to use

SET(ZLIB_VERSION "1.2.11")
SET(ZLIB_MAJOR_VERSION "1")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    IF(RELEASE_MODE)
        SET(ZLIB_SHARED_LIBRARY zlib${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_IMPORT_LIBRARY zlib${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSE()
        SET(ZLIB_SHARED_LIBRARY zlibd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_IMPORT_LIBRARY zlibd${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ENDIF()
ELSEIF(APPLE)
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_MAJOR_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ZLIB_IMPORT_LIBRARY ${ZLIB_SHARED_LIBRARY})
ELSE()
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_MAJOR_VERSION})
    SET(ZLIB_IMPORT_LIBRARY ${ZLIB_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${ZLIB_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES lib/${ZLIB_SHARED_LIBRARY})

IF(WIN32)
    LIST(APPEND SHA1_FILES lib/${ZLIB_IMPORT_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 830936ffbf33364a7da626e69f046dce7f25bd9f
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 2b7d7495cdd48a42bb0a7dc6677a7a6e3f8d89de
                            18890190cbce3220afc0867043f2ca2a01a5aa2f
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 32261338679eab00ee6cd1f3f959b598168cd4da
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 828cfa533b41cc680450018a64c2e52c2e18c6d1
                            829175246be5ba335b4ff96f07f35c5010c98aff
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 129f2c968ae01f40f1360388b2c3da9fe178d740
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 2da040b5883c4a00166e9a22cc49c5400d09d025
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 3d65fca8231b9d29b96c3d6bbd41c6f2c0843722
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES e7a94eaf5cd6bfa099ffc700ea81f26e3030a065
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    # Build zlib as an external project

    ExternalProject_Add(zlibBuild
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/zlib.git
        GIT_TAG
            v${ZLIB_VERSION}
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ZLIB_IMPORT_LIBRARY}
    )

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(zlibBuild CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/${ZLIB_SHARED_LIBRARY}
                                                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        IF(APPLE)
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
            )
        ELSE()
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_VERSION}
            )
        ENDIF()

        ExternalProject_Add_Step(zlibBuild CopyLibrary
            COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ACTUAL_ZLIB_SHARED_LIBRARY}
                                             ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            DEPENDEES install)
    ENDIF()

    # Package zlib's include files and library

    CREATE_PACKAGE_FILE(zlib ${ZLIB_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES include ${SHA1_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET zlibBuild
    )

    SET(PLUGIN_DEPENDS_ON zlibBuild)
ENDIF()

# Allow other external projects to find our zlib

SET(ZLIB_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(ZLIB_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(ZLIB_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${ZLIB_IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(ZLIB_PLUGIN zlib PARENT_SCOPE)

ADD_PLUGIN(zlib
    SOURCES
        ../../plugininfo.cpp
        src/zlibplugin.cpp
    HEADERS_MOC
        src/zlibplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)
