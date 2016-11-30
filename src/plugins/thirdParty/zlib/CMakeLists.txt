PROJECT(zlibPlugin)

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
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 49568397c2ddfb5b73f054aa6ca0fed5b0bddb7f)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib e87ac18121c2dac64956b6a1616a94dc67de3933)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll f76a5bc74b281a1d607836aeb3b11723f890b45d)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 15d20ff05456948ee305c341133593a1cdf8317b)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.dylib 860f731791de32ca21c2e1cbc40ea6b66451192a)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so 12b481605083b4bb2ca4656e752e422c8ef7e1c5)
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
    # Retrieve zlib's definitions

    IF(WIN32)
        SET(DEFINITIONS
            ZLIB_DLL
        )
    ELSE()
        SET(DEFINITIONS)
    ENDIF()

    # Ignore some warnings on Windows

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
            ${DEFINITIONS}
        QT_MODULES
            Core
    )
ENDIF()
