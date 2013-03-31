MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve some LLVM settings

    IF(WIN32)
        SET(LLVM_DEFINITIONS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_NONSTDC_NO_WARNINGS

            _CRT_SECURE_NO_DEPRECATE
            _CRT_SECURE_NO_WARNINGS

            _SCL_SECURE_NO_DEPRECATE
            _SCL_SECURE_NO_WARNINGS
        )
    ELSEIF(APPLE)
        SET(LLVM_DEFINITIONS)
    ELSE()
        SET(LLVM_DEFINITIONS
            __STDC_FORMAT_MACROS
        )
    ENDIF()

    LIST(APPEND LLVM_DEFINITIONS
        __STDC_CONSTANT_MACROS
        __STDC_LIMIT_MACROS
    )

    # Ignore some warnings on Windows

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4291 /wd4351 /wd4355 /wd4624 /wd4722 /wd4805")
        # Note: on Windows, some warnings get generated. Yet, we have nothing to
        #       do with them, so we ignore them...
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
