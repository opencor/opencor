project(OpenSSLPlugin)

# Name and version of our package

set(PACKAGE_NAME OpenSSL)
set(PACKAGE_VERSION 1.1.0i)

# Version of our library

if(WIN32)
    set(LIBRARY_VERSION 1_1)
else()
    set(LIBRARY_VERSION 1.1)
endif()

# Git tag for our library

set(GIT_TAG OpenSSL_1_1_0i)

# Release tag

set(RELEASE_TAG OpenSSL_1_1_0i)

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(CRYPTO_SHARED_LIBRARY libcrypto-${LIBRARY_VERSION}-x64${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CRYPTO_IMPORT_LIBRARY libcrypto${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SSL_SHARED_LIBRARY libssl-${LIBRARY_VERSION}-x64${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SSL_IMPORT_LIBRARY libssl${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(EXPECTED_CRYPTO_IMPORT_LIBRARY libcrypto-${LIBRARY_VERSION}-x64${CMAKE_IMPORT_LIBRARY_SUFFIX})
    set(EXPECTED_SSL_IMPORT_LIBRARY libssl-${LIBRARY_VERSION}-x64${CMAKE_IMPORT_LIBRARY_SUFFIX})
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
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 7cb68b0e6703e79a86228e9051d7602a24acb1b9
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES cdc26b64eaa557554f3d630177cd70fdb70161c6
                                              c545414e53506365b83b22108152d9ebaeda07e5
                                              f88686ca575f45f2b8c96a4b9dcfa3317f59c6a1
                                              52979b22735b8e373f333b07e228500dc5cd237c)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} baf4a6c1f66c434f7b4644e0a5a431916f86c47b
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 026c6949d84afc438e789e893a338f8831169006
                                              95c197d2807cbe92c2f19f69ec1167593ddec254
                                              44966c89db1cb032b730993a97162f1b78d6a37b
                                              709e777def80e9c75b1f819b06f0e7fe643f2292)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} be3cd5a502b3ad4f44e62630251552bc7e31f71d
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2b92edc6a64e00279f4431236e1a646d0aef3a51
                                          98bdd6b52dc9be3f4c0085a23dc728a79b6c5503)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 1c4ecc8ea58d5d00e8787276bd3db72c0ee59388
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c34251763d15bea1ba9b52caaddad3d723654536
                                          39dd38510c4a2849c2995edc76b8c2d57a693b00)
    endif()
else()
    # We require Perl to build OpenSSL

    find_package(Perl REQUIRED QUIET)

    # Specify the platform on which we want to build OpenSSL

    if(WIN32)
        if(RELEASE_MODE)
            set(PLATFORM VC-WIN64A)
        else()
            set(PLATFORM debug-VC-WIN64A)
        endif()
    elseif(APPLE)
        set(PLATFORM darwin64-x86_64-cc)
    else()
        set(PLATFORM linux-x86_64)
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
            ${PERL_EXECUTABLE} Configure ${PLATFORM} --prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} no-asm
        BUILD_COMMAND
            ${MAKE_NMAKE_COMMAND}
        INSTALL_COMMAND
            ${MAKE_NMAKE_COMMAND} install_sw
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${EXPECTED_CRYPTO_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${EXPECTED_SSL_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDllsAndRenameLibs
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy lib/${CRYPTO_IMPORT_LIBRARY} lib/${EXPECTED_CRYPTO_IMPORT_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy lib/${SSL_IMPORT_LIBRARY} lib/${EXPECTED_SSL_IMPORT_LIBRARY}
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
