project(libgit2Plugin)

# Name and version of our package

set(PACKAGE_NAME libgit2)
set(PACKAGE_VERSION 1.3.0)

# Version of our library

set(MAJOR_LIBRARY_VERSION 1)
set(MINOR_LIBRARY_VERSION 3)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.0)

# Git tag for our library

set(GIT_TAG v1.3.0-opencor)

# Release tag

set(RELEASE_TAG v1.3.0-opencor)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared library (and of its import library, if any)

if(WIN32)
    set(SHARED_LIBRARY git2${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY git2${CMAKE_IMPORT_LIBRARY_SUFFIX})
elseif(APPLE)
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2.${MINOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}git2${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION})
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

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 46159c49e10e7652240bcde1b84ed2c68d601e94
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES adbbfd9a2e441e97fcbbcfa656ef58af6afc276a
                                              a9caac225f74decfd327a95f27e86d1579c1d2b0)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} af94acf18c0a891b60de1b15a9a38212422d9b4d
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9dad74785f82437c55109480d2cee25b6a52b0b1
                                              452ae3fbe2680c87c104e6f8e0430001ed15dc36)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} d690fc3b7c19f00da1d27da03d788169c5e52063
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d11643bdac1d72387ae7f8f37f760e6f6e791ffc)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 34a64017a1d4ede8f46ca109a608b7dd6eef86c5
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 4835896484df08cb3fd1bab154dfa78f612ed59f)
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
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
        DEPENDS
            OpenSSLPlugin
            zlibPlugin
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
                        RELEASE_TAG ${RELEASE_TAG}
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
