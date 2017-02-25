PROJECT(zlibPlugin)

# Increment this number before building a new package to a value larger
# larger than its maximum value over all git branches. This is to ensure
# that the correct package archive is retrieved when the build configuration
# is changed (e.g. by changing the git branch).

SET(BUILD_NUMBER 1)

# Specify the version of zlib we want to build

SET(ZLIB_FULL_VERSION "1.2.11")
SET(ZLIB_SHORT_VERSION "1")
SET(ZLIB_PACKAGE_VERSION ${ZLIB_SHORT_VERSION}-${BUILD_NUMBER})

SET(ZLIB_GIT_TAG v${ZLIB_FULL_VERSION})

# The names of the libraries built by zlib's build process

IF(WIN32)
    IF(RELEASE_MODE)
        SET(ZLIB_SHARED_LIBRARY zlib${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_LINK_LIBRARY zlib${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSE()
        SET(ZLIB_SHARED_LIBRARY zlibd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(ZLIB_LINK_LIBRARY zlibd${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ENDIF()
ELSEIF(APPLE)
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(ZLIB_LINK_LIBRARY ${ZLIB_SHARED_LIBRARY})
ELSE()
    SET(ZLIB_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_SHORT_VERSION})
    SET(ZLIB_LINK_LIBRARY ${ZLIB_SHARED_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${ZLIB_SHARED_LIBRARY})

# Specify where our local zlib will be installed

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged

SET(CHECKED_FILES lib/${ZLIB_SHARED_LIBRARY})
IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${ZLIB_LINK_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_PACKAGE_VERSION} 7889eca3e358a6c54642f01483fa0999f9f3f717
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 03b94c543a4651377bd005a561ca853dab903c9f
                            f5b50d90adbb9086d01367200e455b45021c8256
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_PACKAGE_VERSION} 874238c4e904377e93e5d09a63381c0b8b117a4e
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 542a8569e38fdeff822089873de4b43860cf2aed
                            f9f37ea9e9e5f13ae47d86c094e36f0caae77755
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_PACKAGE_VERSION} 45a5b4c6aeadf704ce18337e46e3d0b9c6d54bb8
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES cd253f8540dda135d970a06b6315210b7b756a56
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_PACKAGE_VERSION} 1cf835c747e7cebe7cb814e79447b22684bc3458
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES d52b2dc7874ac142245dc254e7dee1e44d1d139a
        )
    ENDIF()

ELSE()
    INCLUDE(ExternalProject)

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    # Build zlib as an external project

    ExternalProject_Add(zlibBuild
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/zlib
        INSTALL_DIR
            ${FULL_INSTALL_DIR}
        GIT_REPOSITORY
            https://github.com/madler/zlib.git
        GIT_TAG
            ${ZLIB_GIT_TAG}
        PATCH_COMMAND
            ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${ZLIB_LINK_LIBRARY}
    )

    IF(WIN32)
        # Our plugin code expects the DLL in the same directory as the import library.
        ExternalProject_Add_Step(zlibBuild CopyDLL
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_INSTALL_DIR}/bin/${ZLIB_SHARED_LIBRARY}
                                             ${FULL_INSTALL_DIR}/lib
            DEPENDEES install)
    ELSE()
        # We need to dereference the symbolic link to the library as CMake's `tar`
        # can't do this...
        IF(APPLE)
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_FULL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
            )
        ELSE()
            SET(ACTUAL_ZLIB_SHARED_LIBRARY
                ${CMAKE_SHARED_LIBRARY_PREFIX}z${CMAKE_SHARED_LIBRARY_SUFFIX}.${ZLIB_FULL_VERSION}
            )
        ENDIF()
        ExternalProject_Add_Step(zlibBuild CopyLibrary
            COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_INSTALL_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_INSTALL_DIR}/lib/${ACTUAL_ZLIB_SHARED_LIBRARY}
                                             ${FULL_INSTALL_DIR}/lib/${ZLIB_SHARED_LIBRARY}
            DEPENDEES install)
    ENDIF()

    # Package zlib's include files and library

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        zlib ${ZLIB_PACKAGE_VERSION}
        PACKAGED_FILES include ${CHECKED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        TARGET zlibBuild
        )

    SET(PLUGIN_DEPENDS_ON zlibBuild)
ENDIF()

# Allow other external projects to find our zlib

SET(ZLIB_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)
SET(ZLIB_INCLUDE_DIR ${FULL_INSTALL_DIR}/include PARENT_SCOPE)
SET(ZLIB_LIBRARY ${FULL_INSTALL_DIR}/lib/${ZLIB_LINK_LIBRARY} PARENT_SCOPE)

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
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)
