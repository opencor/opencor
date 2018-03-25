project(OpenSSLPlugin)

# Name and version of our package

set(PACKAGE_NAME OpenSSL)
set(PACKAGE_VERSION 1.0.2n)

# Version of our library

set(LIBRARY_VERSION 1.0.0)

# Git tag for our library

set(GIT_TAG OpenSSL_1_0_2n)

# Release tag

set(RELEASE_TAG OpenSSL_1_0_2n)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(CRYPTO_SHARED_LIBRARY libeay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CRYPTO_IMPORT_LIBRARY libeay32${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SSL_SHARED_LIBRARY ssleay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SSL_IMPORT_LIBRARY ssleay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(CRYPTO_IMPORT_LIBRARY ${CRYPTO_SHARED_LIBRARY})

    set(SSL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    set(SSL_IMPORT_LIBRARY ${SSL_SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${CRYPTO_SHARED_LIBRARY}
    ${SSL_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${CRYPTO_SHARED_LIBRARY}
    lib/${SSL_SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${CRYPTO_IMPORT_LIBRARY}
        lib/${SSL_IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_OPENSSL_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 20865015a7c9ead9a8ed9dc820aeec5c68c212fa
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES e4fd30b1e55c6345653f34925b6da46c82add0d6
                                              a0dcaed04bbadda3f41cbf4b292780b6055459ff
                                              55782be904ac3ec25e73c22991b56275d8e9e3c6
                                              0c818c8ec7b7ccbd2a028f9b74876a7cb1424059)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 612b2162b1f8eb4702b7daf837e89cde4a5b782e
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 99516858f18c2a86d7345e94ebddfe1116ff5109
                                              4fd6efc20cab38a5ced215a34354f5124f6bd94e
                                              f5a4b2d495214420c9357e505afc0f98a599c586
                                              c8b210d87c34e146e2ae8b238b6a261f6170689f)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 27f6a323b429a1601d93382a517b21a2ae954331
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES fe24cd5bd70535964ca3cb9d2108a4a180b0439b
                                          5ff4f89e9810e417093f4dc724e78df7771ba53a)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} ca77cf7dec56a0a0c6b3c9d0f6321a1d246d9006
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 8e03d64248279b66158c331168f8a879a26adcb5
                                          3fc75655ac2578da79d4da7f4ecea3ce22bd1b0c)
    endif()
else()
    # We require Perl to build OpenSSL

    find_package(Perl REQUIRED QUIET)

    # Set platform specific configuration options and tool names

    set(CONFIG_OPTIONS "--prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            set(PLATFORM VC-WIN64A)
        else()
            set(PLATFORM debug-VC-WIN64A)
        endif()

        set(MAKEFILE /f ms/ntdll.mak)
        set(INSTALL_TARGET install)
    else()
        list(APPEND CONFIG_OPTIONS shared)

        if(APPLE)
            set(PLATFORM darwin64-x86_64-cc)
        else()
            set(PLATFORM linux-x86_64)

            list(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        endif()

        set(MAKEFILE -f Makefile)
        set(INSTALL_TARGET install_sw)
    endif()

    # Build OpenSSL as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    set(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        DOWNLOAD_DIR
            ${PACKAGE_BUILD_DIR}
        SOURCE_DIR
            ${PACKAGE_BUILD_DIR}
        BINARY_DIR
            ${PACKAGE_BUILD_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/openssl
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${PERL_EXECUTABLE} ./Configure ${PLATFORM} ${CONFIG_OPTIONS}
        BUILD_COMMAND
            ${MAKE_NMAKE_COMMAND} ${MAKEFILE}
        INSTALL_COMMAND
            ${MAKE_NMAKE_COMMAND} ${MAKEFILE} ${INSTALL_TARGET}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${CRYPTO_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SSL_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # Windows requires an additional configuration step

        ExternalProject_Add_Step(${PACKAGE_BUILD} PostConfigure
                                 COMMAND "ms/do_win64a"
                                 WORKING_DIRECTORY ${PACKAGE_BUILD_DIR}
                                 DEPENDEES configure
                                 DEPENDERS build)

        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLLs
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Make our libraries writable
        # Note: indeed, the build process creates write-protected libraries...

        ExternalProject_Add_Step(${PACKAGE_BUILD} MakeWritable
                                 COMMAND chmod 755 ${CRYPTO_SHARED_LIBRARY}
                                 COMMAND chmod 755 ${SSL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        if(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                     COMMAND install_name_tool -id @rpath/${CRYPTO_SHARED_LIBRARY} ${CRYPTO_SHARED_LIBRARY}
                                     COMMAND install_name_tool -id @rpath/${SSL_SHARED_LIBRARY} ${SSL_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DIR}/${CRYPTO_SHARED_LIBRARY}
                                                                       @rpath/${CRYPTO_SHARED_LIBRARY}
                                                                       ${SSL_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES MakeWritable)
        endif()
    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our OpenSSL package

set(OPENSSL_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
set(OPENSSL_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
set(OPENSSL_CRYPTO_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY} PARENT_SCOPE)
set(OPENSSL_SSL_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY} PARENT_SCOPE)
set(OPENSSL_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

set(OPENSSL_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

add_plugin(${PACKAGE_NAME}
    SOURCES
        ../../plugininfo.cpp

        src/opensslplugin.cpp
    HEADERS_MOC
        src/opensslplugin.h
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
