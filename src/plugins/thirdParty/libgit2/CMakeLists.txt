project(libgit2Plugin)

# Name and version of our package

set(PACKAGE_NAME libgit2)
set(PACKAGE_VERSION 0.27.2)

# Version of our library

set(MAJOR_LIBRARY_VERSION 0)
set(MINOR_LIBRARY_VERSION 27)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.2)

# Git tag for our library

set(GIT_TAG v0.27.2)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    set(SHARED_LIBRARY git2${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY git2${CMAKE_IMPORT_LIBRARY_SUFFIX})
elseif(APPLE)
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2.${MINOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2${CMAKE_SHARED_LIBRARY_SUFFIX}.${MINOR_LIBRARY_VERSION})
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

if(USE_PREBUILT_LIBGIT2_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 32b5d1db682cbe22c1e5544f7d26c7ff89f5e509
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 0c26c8c8330d39a7f32a4617400c17aeb3298a4d
                                              10f3731c87f61fbfabcd39e8106f89db17ba6f68)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 5aa32bc2618e3cd7433b2498f084cc1b5da3bb1a
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 514fcc0d8cd764b35de979ded7333b889d613e21
                                              3879d35d63a6cdcab61e5f744a68a5bf9d5e192d)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 0923135ad1ebb056575eb0dc7c4fc51d2af657d8
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e6dcf93ae0f0188de38da2e64d31d1a20d83b9e1)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} d1eebc951f7558368db6c9871b3e8cab5700a67d
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f78b913e961a8e0f40d9394413012ac44870ea40)
    endif()
else()
    # Ignore some warnings

    if(WIN32)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4267 /wd4311 /wd4312")
    elseif(APPLE)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format")
    endif()

    # Some specific arguments

    if(WIN32)
        set(DEFINE_STATIC_CRT_ARG -DSTATIC_CRT=OFF)
    endif()

    # Build libgit2 as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/libgit2
        GIT_TAG
            ${GIT_TAG}
        CMAKE_CACHE_ARGS
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DBUILD_CLAR=OFF
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            ${DEFINE_STATIC_CRT_ARG}
            -DOPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}
            -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
            -DOPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}
            -DUSE_HTTPS=OpenSSL
            -DUSE_SSH=OFF
            -DWINHTTP=OFF
            -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
            -DZLIB_LIBRARY=${ZLIB_LIBRARY}
            -DZLIB_ROOT=${ZLIB_ROOT_DIR}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDll
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SHARED_LIBRARY_PREFIX}git2${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST}
                                                                  ${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our libgit2 package

set(LIBGIT2_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(LIBGIT2_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(LIBGIT2_LIBRARY ${EXTERNAL_BINARIES_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

# Add the plugin

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/libgit2plugin.cpp
    HEADERS_MOC
        src/libgit2plugin.h
    PLUGINS
        OpenSSL
        zlib
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
