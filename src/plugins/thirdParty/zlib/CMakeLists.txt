PROJECT(zlibPlugin)

MACRO(APPLY_ZLIB_BUILD_SETTINGS)
    # Apply zlib's build settings

    IF(WIN32)
        SET(ZLIB_DEFINITIONS
            ZLIB_DLL
        )
    ELSE()
        SET(ZLIB_DEFINITIONS)
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

RETRIEVE_CONFIG_FILES(
    src/PLATFORM_DIR/zconf.h
)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 1f4b8749f5fddb585bbf23efee85c54a84c4a9f2)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 38804ae83c5006e31cc55233d95d064882b0aba2)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll ce77a4dca42730f8798ae9540468138714ed8b22)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib d62dcfcf72e74b4d14f9e6dde95b307926a43346)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.dylib 052d743bfa2fadae21972d704e50c6057af59c9d)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so b855fec49a2553bf739d665e90416e58f2eb8622)
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

    ADD_PLUGIN_BINARY(zlib
        INCLUDE_DIRS
            src
    )
ELSE()
    # Apply zlib's build settings

    APPLY_ZLIB_BUILD_SETTINGS()

    # Ignore some warnings on Windows and OS X

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    ENDIF()

    # Add the plugin

    SET(ZLIB_PLUGIN zlib PARENT_SCOPE)

    ADD_PLUGIN(zlib
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/adler32.c
            src/compress.c
            src/crc32.c
            src/deflate.c
            src/gzclose.c
            src/gzlib.c
            src/gzread.c
            src/gzwrite.c
            src/infback.c
            src/inffast.c
            src/inflate.c
            src/inftrees.c
            src/trees.c
            src/uncompr.c
            src/zlibplugin.cpp
            src/zutil.c
        HEADERS_MOC
            src/zlibplugin.h
        INCLUDE_DIRS
            src
        DEFINITIONS
            ${ZLIB_DEFINITIONS}
        QT_MODULES
            Core
    )
ENDIF()
