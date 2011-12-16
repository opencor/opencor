PROJECT(LLVMPlugin)

MACRO(RETRIEVE_LLVM_SETTINGS)
    # Retrieve LLVM settings

    SET(LLVM_DEFINITIONS
        __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS)

    IF(WIN32)
        SET(CMAKE_SHARED_LIBRARY_PREFIX)
    ENDIF()

    SET(LLVM_VERSION 3.0)
    SET(LLVM_EXTERNAL_DEPENDENCIES
        ${CMAKE_SHARED_LIBRARY_PREFIX}LLVM-${LLVM_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CMAKE_SHARED_LIBRARY_PREFIX}clang${CMAKE_SHARED_LIBRARY_SUFFIX})
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

# LLVM library

SET(LLVM_EXTERNAL_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/lib/${DISTRIB_DIR})

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(LLVM
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS_MOC
        src/llvmplugin.h
    INCLUDE_DIRS
        include
        src
    QT_DEPENDENCIES
        QtCore
    EXTERNAL_DEPENDENCIES_DIR
        ${LLVM_EXTERNAL_DEPENDENCIES_DIR}
    EXTERNAL_DEPENDENCIES
        ${LLVM_EXTERNAL_DEPENDENCIES}
)

# Deploy the plugin's external dependencies
# Note: it must be done here since ADD_PLUGIN doesn't support the fact that an
#       external dependency may have dependencies of its own

IF(WIN32)
    # LLVM library

    FOREACH(LLVM_EXTERNAL_DEPENDENCY ${LLVM_EXTERNAL_DEPENDENCIES})
        DEPLOY_WINDOWS_BINARY_FILE(${LLVM_EXTERNAL_DEPENDENCIES_DIR}/${LLVM_EXTERNAL_DEPENDENCY})
    ENDFOREACH()

    # The libstdc++-6.dll and pthreadGC2.dll files
    # Note: these DLLs come from the official version of MinGW (as opposed to
    #       the Qt patched version) and are required by LLVM, so...

    DEPLOY_WINDOWS_BINARY_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/x86/libstdc++-6.dll)
    DEPLOY_WINDOWS_BINARY_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/x86/pthreadGC2.dll)
ELSEIF(APPLE)
    # Deploy the LLVM libraries together with their respective LLVM
    # dependencies, if any

    DEPLOY_MAC_OS_X_LIBRARY(${CMAKE_SHARED_LIBRARY_PREFIX}LLVM-${LLVM_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${LLVM_EXTERNAL_DEPENDENCIES_DIR}
    )

    DEPLOY_MAC_OS_X_LIBRARY(${CMAKE_SHARED_LIBRARY_PREFIX}clang${CMAKE_SHARED_LIBRARY_SUFFIX}
        TYPE
            Library
        DIR
            ${LLVM_EXTERNAL_DEPENDENCIES_DIR}
        LIBRARIES
            ${CMAKE_SHARED_LIBRARY_PREFIX}LLVM-${LLVM_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ELSE()
    # LLVM library

    FOREACH(LLVM_EXTERNAL_DEPENDENCY ${LLVM_EXTERNAL_DEPENDENCIES})
        INSTALL(FILES ${LLVM_EXTERNAL_DEPENDENCIES_DIR}/${LLVM_EXTERNAL_DEPENDENCY} DESTINATION lib)
    ENDFOREACH()
ENDIF()
