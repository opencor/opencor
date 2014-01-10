MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve some LLVM settings

    IF(WIN32)
        SET(LLVM_DEFINITIONS)
    ELSE()
        SET(LLVM_DEFINITIONS
            __STDC_CONSTANT_MACROS
            __STDC_LIMIT_MACROS
        )

        IF(NOT APPLE)
            LIST(APPEND LLVM_DEFINITIONS
                __STDC_FORMAT_MACROS
                # Note: this is to address a couple of issues with PRIx64 in
                #       [LLVM]/lib/CodeGen/AsmPrinter/AsmPrinter.cpp...
            )
        ENDIF()
    ENDIF()

    # Ignore some warnings on Windows
    # Note: we have nothing to do with them, so...

    IF(WIN32)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4180 /wd4244 /wd4355 /wd4291 /wd4351 /wd4624 /wd4996")
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
