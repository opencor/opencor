project(OpenSSLPlugin)

# Name and version of our package

set(PACKAGE_NAME OpenSSL)
set(PACKAGE_VERSION 1.0.2q)

# Version of our library

set(LIBRARY_VERSION 1.0.0)

# Git tag for our library

set(GIT_TAG OpenSSL_1_0_2q)

# Release tag

set(RELEASE_TAG OpenSSL_1_0_2q)

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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} a3b24105ab6ba3b8a74434460e799d95c5d513cc
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES c170f0f31fc5bde5496b0f596aa8771561b5e819
                                              8fa77f472630094f386853e33349ce4a78742ce0
                                              3559885c89b474ade491c222d2786446a21c20a2
                                              a734c55cdc35a906c864b632d9adeaba5324d190)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} f80cb28ebc232ad068ed18339440f70c300268a0
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 77a1d947db15718b39d9ffa46d7ba797aaac8c4a
                                              d6132b57aaea8af26e194dbf1d0347b234cbe82d
                                              27e893da9735f11242462163172c30938f0cc216
                                              461097aedef36af517a1f0ec029d1894266f9da3)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} ee81c49cd534de74e2627319d627d085dd7c0002
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c879ee62c28d11ba172f5d89e172d4cd8fa76c12
                                          7b7b49c075f5370fb9f828c0cc8480fcb713ea6b)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} b5a2070f1ada81f0f57e5d37bffa5a04b581fbba
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES a8bb4edec4d31aebfe0e5607a4c4d8b372eae331
                                          81a39771af67648662599f758cee5e2d0ba1fe32)
    endif()
else()
    # We require Perl to build OpenSSL

    find_package(Perl REQUIRED QUIET)

    # Set platform specific configuration options

    set(CONFIG_OPTIONS --prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

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
            ${PERL_EXECUTABLE} Configure ${PLATFORM} ${CONFIG_OPTIONS}
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
