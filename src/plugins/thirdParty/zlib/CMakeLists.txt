PROJECT(zlibPlugin)

# Version that we want to use

SET(ZLIB_FULL_VERSION "1.2.11")
SET(ZLIB_SHORT_VERSION "1")
SET(ZLIB_PACKAGE_VERSION ${ZLIB_SHORT_VERSION})

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
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_PACKAGE_VERSION} 0a25227010050d08fb540c90c4cdd563970cb10f
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 4ffbb89d03c0e6eb406a48388c3150822d575adb
                            e99a86117b9b108f292fbaeb7a5de42382439896
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                zlib ${ZLIB_PACKAGE_VERSION} 0f54b7b334982ce88e3a841ff108a9dc19c99da4
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 1be6ee880ddcfe59f55cc954f5d6c915a3d7bb35
                            f62633335a92e9c79a6a8607bdc648283df24722
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_PACKAGE_VERSION} 619df9fa803b362e5952ba1462a5d2538bee72f2
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 2da040b5883c4a00166e9a22cc49c5400d09d025
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            zlib ${ZLIB_PACKAGE_VERSION} 64f4cdc837ac972c61a26b5ad0e1bd144ddb1daa
            CHECKED_FILES ${CHECKED_FILES}
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
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(zlibBuild CopyDLL
            COMMAND ${CMAKE_COMMAND} -E copy ${FULL_INSTALL_DIR}/bin/${ZLIB_SHARED_LIBRARY}
                                             ${FULL_INSTALL_DIR}/lib
            DEPENDEES install)
    ELSE()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

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
