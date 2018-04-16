project(OpenSSLPlugin)

# Name and version of our package

set(PACKAGE_NAME OpenSSL)
set(PACKAGE_VERSION 1.0.2o)

# Version of our library

set(LIBRARY_VERSION 1.0.0)

# Git tag for our library

set(GIT_TAG opencor)

# Release tag

set(RELEASE_TAG OpenSSL_1_0_2o)

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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 0aa58b9342e18b4e00c4e63552240e743d906469
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES eedacf123ae9b21fea6f59c6751deb8c7742e317
                                              181c9a6b23ac7d12123c87b2c138f09ba53504f7
                                              cf5899b21b355322216d7ce44042aa2dc78d9fd8
                                              f818b017c32cccfaf69bbe5ee1fa6ce8cd8372f5)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 1bea160227049568481e231db0c54d87df6c756a
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 2463f03809546838345da290443a455c8e8b6caf
                                              487f8ecaf5ad841e80494335dea5ac39e9397b06
                                              cb6969245ee60547240d65a066de3a78ae804da2
                                              c3947b2ebd8f017c5c334717bcfd9bc276925ee1)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f6f3791a8689ba7224d1883406f48167d383cd32
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 9f0b67792077c5c2e4a2bf539e6a54d95b8fe2de
                                          98cfa6a0cdf645b6cde8467e01bde505a83147da)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 41798ddbb59c34035f95a24e52392ffdaa2bab41
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES ba0b0671ecd499db16024af1b48ea671ede61727
                                          0b52ee0dcdb5f9f4da001d0f3e6189a7309a8571)
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

        ExternalProject_Add_Step(${PACKAGE_BUILD} postConfigure
                                 COMMAND "ms/do_win64a"
                                 WORKING_DIRECTORY ${PACKAGE_BUILD_DIR}
                                 DEPENDEES configure
                                 DEPENDERS build)

        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDlls
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    else()
        # Make our libraries writable
        # Note: indeed, the build process creates write-protected libraries...

        ExternalProject_Add_Step(${PACKAGE_BUILD} makeWritable
                                 COMMAND chmod 755 ${CRYPTO_SHARED_LIBRARY}
                                 COMMAND chmod 755 ${SSL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        if(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} fixRpath
                                     COMMAND install_name_tool -id @rpath/${CRYPTO_SHARED_LIBRARY} ${CRYPTO_SHARED_LIBRARY}
                                     COMMAND install_name_tool -id @rpath/${SSL_SHARED_LIBRARY} ${SSL_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DIR}/${CRYPTO_SHARED_LIBRARY}
                                                                       @rpath/${CRYPTO_SHARED_LIBRARY}
                                                                       ${SSL_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES makeWritable)
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
