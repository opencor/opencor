PROJECT(LibXDiffPlugin)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_LIBXDIFF_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LibXDiff.dll XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LibXDiff.lib XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LibXDiff.dll XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} LibXDiff.lib XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLibXDiff.dylib XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libLibXDiff.so XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(LIBXDIFF_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}LibXDiff${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(LIBXDIFF_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}LibXDiff${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(LibXDiff
        INCLUDE_DIRS
            src
    )
ELSE()
    # Check whether some include files exist

    INCLUDE(CheckIncludeFiles)

    CHECK_INCLUDE_FILES(limits.h HAVE_LIMITS_H)
    CHECK_INCLUDE_FILES(stdio.h HAVE_STDIO_H)
    CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
    CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
    CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)

    # Check whether some functions exist

    INCLUDE(CheckFunctionExists)

    CHECK_FUNCTION_EXISTS(memchr HAVE_MEMCHR)
    CHECK_FUNCTION_EXISTS(memcmp HAVE_MEMCMP)
    CHECK_FUNCTION_EXISTS(memcpy HAVE_MEMCPY)
    CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)
    CHECK_FUNCTION_EXISTS(strlen HAVE_STRLEN)

    # Default definitions

    SET(DEFINITIONS HAVE_CONFIG_H)

    # Specify our include definitions

    IF(HAVE_LIMITS_H)
        LIST(APPEND DEFINITIONS HAVE_LIMITS_H)
    ENDIF()

    IF(HAVE_STDIO_H)
        LIST(APPEND DEFINITIONS HAVE_STDIO_H)
    ENDIF()

    IF(HAVE_STDLIB_H)
        LIST(APPEND DEFINITIONS HAVE_STDLIB_H)
    ENDIF()

    IF(HAVE_STRING_H)
        LIST(APPEND DEFINITIONS HAVE_STRING_H)
    ENDIF()

    IF(HAVE_UNISTD_H)
        LIST(APPEND DEFINITIONS HAVE_UNISTD_H)
    ENDIF()

    # Specify our function definitions

    IF(HAVE_MEMCHR)
        LIST(APPEND DEFINITIONS HAVE_MEMCHR)
    ENDIF()

    IF(HAVE_MEMCMP)
        LIST(APPEND DEFINITIONS HAVE_MEMCMP)
    ENDIF()

    IF(HAVE_MEMCPY)
        LIST(APPEND DEFINITIONS HAVE_MEMCPY)
    ENDIF()

    IF(HAVE_MEMSET)
        LIST(APPEND DEFINITIONS HAVE_MEMSET)
    ENDIF()

    IF(HAVE_STRLEN)
        LIST(APPEND DEFINITIONS HAVE_STRLEN)
    ENDIF()

    # Add the plugin

    SET(LIBXDIFF_PLUGIN LibXDiff PARENT_SCOPE)

    ADD_PLUGIN(LibXDiff
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/libxdiffplugin.cpp
            src/xadler32.c
            src/xalloc.c
            src/xbdiff.c
            src/xbpatchi.c
            src/xdiffi.c
            src/xemit.c
            src/xmerge3.c
            src/xmissing.c
            src/xpatchi.c
            src/xprepare.c
            src/xrabdiff.c
            src/xrabply.c
            src/xutils.c
            src/xversion.c
        HEADERS_MOC
            src/libxdiffplugin.h
        INCLUDE_DIRS
            src
        DEFINITIONS
            ${DEFINITIONS}
        QT_MODULES
            Core
    )
ENDIF()
