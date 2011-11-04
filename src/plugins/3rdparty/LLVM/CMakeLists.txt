PROJECT(LLVMPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Retrieve some header files that were generated on each of our target
# platforms using CMake on the LLVM source code, but first determine the
# location of the files based on the architecture we are targetting

IF(WIN32)
    SET(DISTRIB_DIR windows/x86)
ELSEIF(APPLE)
    SET(DISTRIB_DIR macosx)
ELSE()
    IF(64BIT_MODE)
        SET(DISTRIB_DIR linux/x64)
    ELSE()
        SET(DISTRIB_DIR linux/x86)
    ENDIF()
ENDIF()

# Now, we can copy the Config header files

SET(LLVM_CONFIG_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Config)
SET(CONFIG_HEADER_FILES
    AsmParsers.def
    AsmPrinters.def
    config.h
    Disassemblers.def
    llvm-config.h
    Targets.def
)

FOREACH(CONFIG_HEADER_FILE ${CONFIG_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_CONFIG_HEADERS_DIR}/${DISTRIB_DIR}/${CONFIG_HEADER_FILE}
                   ${LLVM_CONFIG_HEADERS_DIR}/${CONFIG_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

# And now, the Support header file

SET(LLVM_SUPPORT_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include/llvm/Support)
SET(SUPPORT_HEADER_FILES
    DataTypes.h
)

FOREACH(SUPPORT_HEADER_FILE ${SUPPORT_HEADER_FILES})
    CONFIGURE_FILE(${LLVM_SUPPORT_HEADERS_DIR}/${DISTRIB_DIR}/${SUPPORT_HEADER_FILE}
                   ${LLVM_SUPPORT_HEADERS_DIR}/${SUPPORT_HEADER_FILE}
                   COPYONLY)
ENDFOREACH()

# A definition and flag specific to Windows

IF(WIN32)
    SET(NO_CTYPE_INLINE __NO_CTYPE_INLINE)
    SET(ALLOW_MULTIPLE_DEFINITION -Wl,--allow-multiple-definition)
ELSE()
    SET(NO_CTYPE_INLINE)
    SET(ALLOW_MULTIPLE_DEFINITION)
ENDIF()

# Add the plugin

ADD_PLUGIN(LLVM
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/llvmplugin.h
    DEFINITIONS
        _GNU_SOURCE
        __STDC_LIMIT_MACROS
        __STDC_CONSTANT_MACROS
        ${NO_CTYPE_INLINE}
    FLAGS
        ${ALLOW_MULTIPLE_DEFINITION}
    PLUGIN_INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
)
