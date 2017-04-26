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
            HAVE_CONFIG_H
        QT_MODULES
            Core
    )
ENDIF()
