PROJECT(zlibPlugin)

# Version that we want to use

SET(PACKAGE_MAJOR_VERSION "1")
SET(PACKAGE_VERSION "${PACKAGE_MAJOR_VERSION}.2.11")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    IF(RELEASE_MODE)
        SET(PACKAGE_SHARED_LIBRARY zlib${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(PACKAGE_IMPORT_LIBRARY zlib${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSE()
        SET(PACKAGE_SHARED_LIBRARY zlibd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(PACKAGE_IMPORT_LIBRARY zlibd${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ENDIF()
ELSEIF(APPLE)
    SET(PACKAGE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${PACKAGE_MAJOR_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PACKAGE_IMPORT_LIBRARY ${PACKAGE_SHARED_LIBRARY})
ELSE()
    SET(PACKAGE_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${PACKAGE_MAJOR_VERSION})
    SET(PACKAGE_IMPORT_LIBRARY ${PACKAGE_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${PACKAGE_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES lib/${PACKAGE_SHARED_LIBRARY})

IF(WIN32)
    LIST(APPEND SHA1_FILES lib/${PACKAGE_IMPORT_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(zlib ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 46f64af9fea228a3afa6d64bad1a0700ba20c67b
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 6a8408008702971d5cbf5e6f49953273d6e2f249
                            63892d9f0e54248c4c6f97ca0ba5342938b52d0e
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(zlib ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} b3695becf1f964b1207dc2371aa1978973d92459
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 2121f4b71cab6e851b2a01ac241a684a75ee06ad
                            0ef60637c15c35954fe4ca17be1837cd72bd8f69
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(zlib ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 4de712a64a9121424aaf23a9683165c6af349098
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 2da040b5883c4a00166e9a22cc49c5400d09d025
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(zlib ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} f9934d635b4713cdceab4aa751745029fab4f82d
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES e7a94eaf5cd6bfa099ffc700ea81f26e3030a065
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Ignore some warnings

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4267")
    ENDIF()

    # Build zlib as an external project

    SET(PACKAGE_BUILD zlibBuild)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/zlib.git
        GIT_TAG
            v${PACKAGE_VERSION}
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${PACKAGE_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/${PACKAGE_SHARED_LIBRARY}
                                                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib
                                 DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        SET(FULL_PACKAGE_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${PACKAGE_SHARED_LIBRARY})

        IF(APPLE)
            SET(REAL_FULL_PACKAGE_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}z.${PACKAGE_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ELSE()
            SET(REAL_FULL_PACKAGE_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${PACKAGE_VERSION})
        ENDIF()

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_PACKAGE_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${REAL_FULL_PACKAGE_SHARED_LIBRARY}
                                                                  ${FULL_PACKAGE_SHARED_LIBRARY}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(zlib ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES include ${SHA1_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
    )

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our zlib

SET(ZLIB_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(ZLIB_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(ZLIB_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${PACKAGE_IMPORT_LIBRARY} PARENT_SCOPE)

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
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
