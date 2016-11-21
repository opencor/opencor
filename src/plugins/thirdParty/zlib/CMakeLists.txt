PROJECT(zlibPlugin)

SET(ZLIB_VERSION "1")
SET(ZLIB_GIT_TAG "v1.2.8")

IF(WIN32)
    SET(ZLIB_LIBRARY zlib${ZLIB_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
ELSE()
    SET(ZLIB_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}z.${ZLIB_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
ENDIF()
SET(EXTERNAL_BINARIES ${ZLIB_LIBRARY})

SET(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin/${REMOTE_EXTERNAL_BINARIES_DIR})


IF(WIN32)
     SET(ZLIB_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}zlib${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
ELSEIF(APPLE)
    SET(ZLIB_PLUGIN_BINARY ${PROJECT_BUILD_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/Frameworks/${ZLIB_LIBRARY} PARENT_SCOPE)
ELSE()
ENDIF()

#####
set(USE_PREBUILT_ZLIB_PLUGIN OFF)
#####

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 771a0784eec54a7f107a26ed93e336a92186f89a)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib d70666043587cf1a6a77f64ee2d3b73f7afe7e59)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 9c7637797bafdcdeaf6414115023adb7ca4b1782)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 382ca1b8f5ee26b3b21cf988ee9f87d09da5dd56)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.dylib 0cef02c11f578bc62003b0a2ba0d26e10b6dc229)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so 909cdf712c7c5490588c7b770d1241356181d3bf)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(ZLIB_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}zlib${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(ZLIB_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}zlib${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

ELSE()
    INCLUDE(ExternalProject)

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(ZLIB_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    ExternalProject_Add(
        zlibBuild
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/zlib
        INSTALL_DIR ${INSTALL_DIR}
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG ${ZLIB_GIT_TAG}
        PATCH_COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
        CMAKE_CACHE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_FLAGS:STRING=${ZLIB_C_FLAGS}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
    )

    IF(WINDOWS)
        # the zlib library should be named zlib1.lib not zlib.lib
        ExternalProject_Add_Step(zlib RenameLib
            COMMAND ${CMAKE_COMMAND} -E copy ${INSTALL_DIR}/lib/zlib.lib
                                             ${INSTALL_DIR}/lib/zlib${ZLIB_VERSION}.lib
            DEPENDEES install)
    ENDIF()

##    ExternalProject_Add_StepDependencies(zlibBuild install ${EXTERNAL_BINARIES})
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
        ${INSTALL_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${INSTALL_DIR}/lib
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
)

IF(NOT USE_PREBUILT_ZLIB_PLUGIN)
    ADD_DEPENDENCIES(zlibPlugin zlibBuild)
ENDIF()
