project(LLVMClangPlugin)

# Name and version of our package

set(PACKAGE_NAME LLVMClang)
set(PACKAGE_VERSION 6.0.0)

# Version of our library

set(MAJOR_LIBRARY_VERSION 6)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.0.0)

# Git tag for our library

set(GIT_TAG v6.0.0)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(SHARED_LIBRARY llvmclang.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY llvmclang.${MAJOR_LIBRARY_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}llvmclang${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_LLVMCLANG_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 54c2aac1d8f4e1ff2c00d8659691112444f86bb6
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b992e8ea7d5eecb6848f8f306aeb2a8984cc0251
                                              95a28ad9c7655b58dff64c9d2059b146d97cbc51)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b0cac351bbd5a2b35343dd516f53496c5d4348ef
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 746c9587ad7893b911dcdff310dd98b8e524f93f
                                              1e56d89302590938d3cde9593e175e631ed2fcd1)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 8978c11462d76f607f86f53fcdd9ba4e58940814
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 1adac16d0e20f006a9bc9a1608671f975bf96db2)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} bf1bf90cf7ee72a42e512e0d7972f4a6cde584dc
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 24bba92cb8adf0d7cbb56d71a56396a821438101)
    endif()
else()
    # Allow for big objects and ignore some warnings

    if(WIN32)
        set(PACKAGE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /wd4251 /wd4275 /wd4530")
    endif()

    # Build LLVM+Clang as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

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
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DLLVMCLANG_BUILD_SHARED_LIB=ON
            -DLLVM_BUILD_RUNTIME=OFF
            -DLLVM_ENABLE_ASSERTIONS=OFF
            -DLLVM_ENABLE_BACKTRACES=OFF
            -DLLVM_ENABLE_CRASH_OVERRIDES=OFF
            -DLLVM_ENABLE_DIA_SDK=OFF
            -DLLVM_ENABLE_LIBEDIT=OFF
            -DLLVM_ENABLE_LIBXML2=OFF
            -DLLVM_ENABLE_MODULE_DEBUGGING=OFF
            -DLLVM_ENABLE_OCAMLDOC=OFF
            -DLLVM_ENABLE_PEDANTIC=OFF
            -DLLVM_ENABLE_TERMINFO=OFF
            -DLLVM_ENABLE_WARNINGS=OFF
            -DLLVM_INCLUDE_DOCS=OFF
            -DLLVM_INCLUDE_EXAMPLES=OFF
            -DLLVM_INCLUDE_GO_TESTS=OFF
            -DLLVM_INCLUDE_RUNTIMES=OFF
            -DLLVM_INCLUDE_TESTS=OFF
            -DLLVM_INCLUDE_TOOLS=OFF
            -DLLVM_INCLUDE_UTILS=OFF
            -DLLVM_TARGETS_TO_BUILD=X86
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our LLVM+Clang package

set(LLVMCLANG_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(LLVMCLANG_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(LLVMCLANG_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
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
