MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve some LLVM settings

    LIST(APPEND LLVM_DEFINITIONS
        __STDC_CONSTANT_MACROS
        __STDC_LIMIT_MACROS
    )

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4138 /wd4244 /wd4291 /wd4351 /wd4355 /wd4551 /wd4624 /wd4722")
        # Note: on Windows, some warnings get generated. Yet, we have nothing to
        #       do with them, so...
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target platforms

SET(CONFIG_FILES
    include/llvm/Config/DISTRIB_DIR/config.h
    include/llvm/Config/DISTRIB_DIR/llvm-config.h

    include/llvm/Support/DISTRIB_DIR/DataTypes.h
)

FOREACH(CONFIG_FILE ${CONFIG_FILES})
    STRING(REPLACE "DISTRIB_DIR/" "${DISTRIB_DIR}/" CONFIG_FILE_ORIG "${CONFIG_FILE}")
    STRING(REPLACE "DISTRIB_DIR/" "" CONFIG_FILE_DEST "${CONFIG_FILE}")

    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/${CONFIG_FILE_ORIG}
                   ${PROJECT_SOURCE_DIR}/${CONFIG_FILE_DEST}
                   COPYONLY)
ENDFOREACH()
