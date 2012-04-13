MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve LLVM settings

    IF(MSVC)
        SET(LLVM_MSVC_DEFINITIONS
            _CRT_NONSTDC_NO_DEPRECATE
            _CRT_NONSTDC_NO_WARNINGS

            _CRT_SECURE_NO_DEPRECATE
            _CRT_SECURE_NO_WARNINGS

            _SCL_SECURE_NO_DEPRECATE
            _SCL_SECURE_NO_WARNINGS
        )
    ELSE()
        SET(LLVM_MSVC_DEFINITIONS)
    ENDIF()

    SET(LLVM_DEFINITIONS
        ${LLVM_MSVC_DEFINITIONS}

        __STDC_CONSTANT_MACROS
        __STDC_LIMIT_MACROS
    )

    IF(MSVC)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4146 /wd4244 /wd4355 /wd4503 /wd4624 /wd4805")
        # Note: to build LLVM generates quite a few warnings from MSVC, so
        #       disable them since we have nothing to do with them...
    ENDIF()
ENDMACRO()

# Retrieve some header files that were generated on each of our target
# platforms

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Config)
SET(CONFIG_HEADER_FILES
    config.h
    llvm-config.h
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Support)
SET(CONFIG_HEADER_FILES
    DataTypes.h
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()
