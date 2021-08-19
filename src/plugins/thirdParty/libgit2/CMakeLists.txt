project(libgit2Plugin)

# Name and version of our package

set(PACKAGE_NAME libgit2)
set(PACKAGE_VERSION 1.1.1)

# Version of our library

set(MAJOR_LIBRARY_VERSION 1)
set(MINOR_LIBRARY_VERSION 1)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.${MINOR_LIBRARY_VERSION}.1)

# Git tag for our library

set(GIT_TAG v1.1.1-opencor)

# Release tag

set(RELEASE_TAG v1.1.1-opencor)

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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 2a999dd0d3fd48a5fc86902569b0324332e4ed58
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9fe9905d6a7c6c71a679bac54c165785bcd666bc
                                              85c2b37f1bfe21955bffeebdab1e5fc7742c9010)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 5673e9fc2f6a82fd71f0605a5f2dab86ae3d6257
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 9e34f5437920baf030683ec207465323dea2ac78
                                              6920b1fa6e49bcc6c217d86c4d281afb588191cc)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} c4eaeeb5f992bbe02eba298005dc583ee01de995
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES b585168ec2b2518fd44d62d9920c7786adbb04a4)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} caff511d70f4bc9c33cb3cb55aa8ab2b5d701a07
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e4229172d40d5d20e97b46900b4a560a1479d09f)
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
