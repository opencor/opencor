PROJECT(zlibPlugin)

# Specify the version of zlib we want to build

SET(ZLIB_FULL_VERSION "1.2.8")
SET(ZLIB_SHORT_VERSION "1")

SET(ZLIB_GIT_TAG v${ZLIB_FULL_VERSION})

# The names of the libraries built by zlib's build process

IF(WIN32)
    IF(RELEASE_MODE)
        SET(ZLIB_LIBRARY zlib${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_GENERATED_LIBRARY zlib.lib)
    ELSE()
        SET(ZLIB_LIBRARY zlibd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_GENERATED_LIBRARY zlibd.lib)
    ENDIF()
ELSEIF(APPLE)
    SET(ZLIB_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ZLIB_GENERATED_LIBRARY ${ZLIB_LIBRARY})
ELSE()
    SET(ZLIB_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_SHORT_VERSION})
    SET(ZLIB_GENERATED_LIBRARY ${ZLIB_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${ZLIB_LIBRARY})

# Specify where our local zlib will be installed

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged

SET(CHECKED_FILES lib/${ZLIB_LIBRARY})
IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${ZLIB_GENERATED_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_SHORT_VERSION} 7194c3b2dd6bd7ff0527f2233d2af79b4a14e5ba
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 6a20d25d86b04f05ce3a7522b55e0aa72b371d67
                            44029c49007f9f4763cc24697f5a844703fc60e3
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_SHORT_VERSION} 247cc1f804aa2e39ca12639428cab963ed5a2288
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 28c247df6eb273f0054307a2e7eb8acf15b56509
                            7bca1863a471c08a27bca8d5a6739cc835966195
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_SHORT_VERSION} f025002ebc0426193d41888dc13565a5ee5864f4
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 49e9f0fabc7b5a7ebaa77491a27ba5d18cb3b328
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_SHORT_VERSION} bbd6b6a01573b53eaaf4452b9975206c61cb1115
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 6a319e02bc93b9f847982eea4c591087f3baa5e3
        )
    ENDIF()

ELSE()
    INCLUDE(ExternalProject)

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    # Build zlib as an external project

    ExternalProject_Add(
        zlibBuild
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/zlib
        INSTALL_DIR ${FULL_INSTALL_DIR}
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG ${ZLIB_GIT_TAG}
        PATCH_COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ZLIB_GENERATED_LIBRARY}
    )

    IF(WIN32)
        # Our plugin code expects the DLL in the same directory as the import library.
        ExternalProject_Add_Step(zlibBuild CopyDLL
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_INSTALL_DIR}/bin/${ZLIB_LIBRARY}
                                             ${FULL_INSTALL_DIR}/lib
            DEPENDEES install)
    ELSE()
        # We need to dereference the symbolic link to the library as CMake's `tar`
        # can't do this...
        IF(APPLE)
            SET(ACTUAL_ZLIB_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_FULL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
            )
        ELSE()
            SET(ACTUAL_ZLIB_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_FULL_VERSION}
            )
        ENDIF()
        ExternalProject_Add_Step(zlibBuild CopyLibrary
            COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_INSTALL_DIR}/lib/${ZLIB_LIBRARY}
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_INSTALL_DIR}/lib/${ACTUAL_ZLIB_LIBRARY}
                                             ${FULL_INSTALL_DIR}/lib/${ZLIB_LIBRARY}
            DEPENDEES install)
    ENDIF()

    # Package zlib's include files and library

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        zlib ${ZLIB_SHORT_VERSION}
        PACKAGED_FILES include ${CHECKED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        DEPENDENCY zlibBuild
        )
ENDIF()

# Add the plugin

SET(ZLIB_PLUGIN zlib PARENT_SCOPE)

ADD_PLUGIN(zlib
    SOURCES
        ../../plugininfo.cpp
        src/zlibplugin.cpp
    HEADERS_MOC
        src/zlibplugin.h
    INCLUDE_DIRS
        ${RELATIVE_INSTALL_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${FULL_INSTALL_DIR}/lib
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
)

# We depend on zlib being built if we are not using the prebuilt version

IF(NOT USE_PREBUILT_ZLIB_PLUGIN)
    ADD_DEPENDENCIES(zlibPlugin zlibBuild)
ENDIF()
