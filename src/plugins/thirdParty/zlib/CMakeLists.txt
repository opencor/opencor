PROJECT(zlibPlugin)

MACRO(RETRIEVE_ZLIB_SETTINGS)
    # Retrieve some zlib settings

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
    src/DISTRIB_DIR/zconf.h
)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_ZLIB_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}")

    IF(WIN32)
        IF(${ARCHITECTURE} EQUAL 32)
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll c6aa20cb03ae019c8b68e37defd8a1d01351f654)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib a622bb6927a46a6d0cc96d87e543819378f6588c)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 71839bccad5979fb38fcd2c7ec369ccfbdc67d13)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 297430f534f8c0c047e48ae550bdbf0010692c34)
            ENDIF()
        ELSE()
            IF(RELEASE_MODE)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll 2f0079f07ea13996c12606e724f4e965ca0e4226)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 4ab4e570b2ce7e53974d5eaf5371b373d3501718)
            ELSE()
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.dll ff3c293bbbef886c89b79d0f6f2e2149f59c5242)
                RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} zlib.lib 75d414b6a973483eae5a403ef18ea791497913e2)
            ENDIF()
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.dylib 8147b2aca9ccee3f7798ff457de0f3f08953159f)
    ELSE()
        IF(${ARCHITECTURE} EQUAL 32)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so a2b86edaf4fb00970e3d7cfa0b6a7df82bb1a2d5)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libzlib.so 3f943648daa8f6d0c010dbe98af6481b5c59b66b)
        ENDIF()
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(ZLIB_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/bin/${DISTRIB_BINARY_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}zlib${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(ZLIB_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}zlib${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(zlib
        INCLUDE_DIRS
            src
    )
ELSE()
    # Retrieve some zlib settings

    RETRIEVE_ZLIB_SETTINGS()

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
