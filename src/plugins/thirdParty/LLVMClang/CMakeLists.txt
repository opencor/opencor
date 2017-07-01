PROJECT(LLVMClangPlugin)

# Name and version of our package

SET(PACKAGE_NAME LLVMClang)
SET(PACKAGE_VERSION 4.0.1)

# Version of our library

SET(MAJOR_LIBRARY_VERSION 4)
SET(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0.1)

# Git tag for our library

SET(GIT_TAG v4.0.1)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY llvmclang.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY llvmclang.${MAJOR_LIBRARY_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}llvmclang${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_LLVMCLANG_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 1eee742a00a29d128c9ca53828737347be0b18e0
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 6fff4a5ce1b911e1600e5e242548b7cd43a86e00
                                              1c8e4b02d728ce1f195374064caef2af15f5bc54)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 67a93b88b013ce4d04d8fb6c759dababf7b1bfb9
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES cb8fb742ec4ef0ce369482a10c96f52608040a11
                                              b11b2b52ce741344d5ad028a9b6b60d144262d9a)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 7afc1f57275425757b70f04c50803c570d4b0e97
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 8f4569fcd1b4d4b04150963bb07c8a828e70fc80)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} cb7c61ec33f552deced5d8ad2b7381dd1108bfe3
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 300e2922feb89fdc179e175e8e69d44601673d3b)
    ENDIF()
ELSE()
    # Allow for big objects and ignore some warnings

    IF(WIN32)
        SET(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /wd4251 /wd4275 /wd4530")
    ENDIF()

    # Build LLVM+Clang as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/llvmclang
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_CXX_FLAGS:STRING=${PACKAGE_CXX_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DCLANG_BUILD_TOOLS=OFF
            -DCLANG_ENABLE_ARCMT=OFF
            -DCLANG_ENABLE_STATIC_ANALYZER=OFF
            -DCLANG_INCLUDE_DOCS=OFF
            -DCLANG_INCLUDE_TESTS=OFF
            ${DEFINE_CMAKE_APPLE_OPTIONS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DLLVMCLANG_BUILD_SHARED_LIB=ON
            -DLLVM_BUILD_RUNTIME=OFF
            -DLLVM_BUILD_TOOLS=OFF
            -DLLVM_BUILD_UTILS=OFF
            -DLLVM_ENABLE_DIA_SDK=OFF
            -DLLVM_INCLUDE_DOCS=OFF
            -DLLVM_INCLUDE_EXAMPLES=OFF
            -DLLVM_INCLUDE_GO_TESTS=OFF
            -DLLVM_INCLUDE_TESTS=OFF
            -DLLVM_INCLUDE_UTILS=OFF
            -DLLVM_TARGETS_TO_BUILD=X86
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our LLVM+Clang package

SET(LLVMCLANG_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(LLVMCLANG_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(LLVMCLANG_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/llvmclangplugin.cpp
    HEADERS_MOC
        src/llvmclangplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
