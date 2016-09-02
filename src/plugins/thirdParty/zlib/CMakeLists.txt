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
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll b9e8c28e13b2499bf9d29f8e987e625b826c49df)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 38233d6be9ed6ce9277b4e151c27cf1e37d3e0db)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll b0c6def61c1249cac02ef888343ee723ce00dda6)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 923ab7a452996ee3c353cb7591bd1183d3abe527)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.dylib a0de3c6458eed5c0fcd4cf68e9e8953ee451a9f5)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so 002f9a12e81a1735fc831a4e4a62b53bc50847c5)
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
            ${DEFINITIONS}
        QT_MODULES
            Core
    )
ENDIF()
