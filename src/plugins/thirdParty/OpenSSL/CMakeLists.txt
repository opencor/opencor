project(OpenSSLPlugin)

# Name and version of our package

set(PACKAGE_NAME OpenSSL)
set(PACKAGE_VERSION 1.1.1f)

# Version of our library

if(WIN32)
    set(LIBRARY_VERSION 1_1)
else()
    set(LIBRARY_VERSION 1.1)
endif()

# Git tag for our library

set(GIT_TAG OpenSSL_1_1_1f)

# Release tag

set(RELEASE_TAG OpenSSL_1_1_1f)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(CRYPTO_SHARED_LIBRARY libcrypto-${LIBRARY_VERSION}-x64${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(CRYPTO_IMPORT_LIBRARY libcrypto-${LIBRARY_VERSION}-x64${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(SSL_SHARED_LIBRARY libssl-${LIBRARY_VERSION}-x64${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(SSL_IMPORT_LIBRARY libssl-${LIBRARY_VERSION}-x64${CMAKE_IMPORT_LIBRARY_SUFFIX})

    set(REAL_CRYPTO_IMPORT_LIBRARY libcrypto${CMAKE_IMPORT_LIBRARY_SUFFIX})
    set(REAL_SSL_IMPORT_LIBRARY libssl${CMAKE_IMPORT_LIBRARY_SUFFIX})
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

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 488e9123c182d5d34258feb4163746c99b9450b7
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 0e5a90403d9306339e95809e8ac135419d40a7cf
                                              bb183ea42a9e1c8984abfcfab2251bde5ff94d95
                                              400cc6624174c02f876ff8fb40a71d0716903f59
                                              1f962949bf8a1b67959c3a8476ff786ca109bbad)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} f988cc7c51bbba79ff2c88609693ee2e094bc5bb
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES d1cedfb6504a78a38c88cd7856d9cc1df0e6f0ab
                                              aef59b5bd77c06da3495c09b7f5808807ec63da7
                                              400cc6624174c02f876ff8fb40a71d0716903f59
                                              1f962949bf8a1b67959c3a8476ff786ca109bbad)
        endif()

        # Make a copy of our import libraries
        # Note: our package contains both libcrypto-1_1-x64.lib and
        #       libssl-1_1-x64.lib, but when building OpenSSL, we actually end
        #       up with both libcrypto.lib and libssl.lib. Now, as expected, our
        #       DLLs need the import libraries in our package, but CMake looks
        #       for the import libraries that were originally built, hence we
        #       must have both versions...

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy lib/${CRYPTO_IMPORT_LIBRARY} lib/${REAL_CRYPTO_IMPORT_LIBRARY}
                        COMMAND ${CMAKE_COMMAND} -E copy lib/${SSL_IMPORT_LIBRARY} lib/${REAL_SSL_IMPORT_LIBRARY}
                        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} b7b654376354872456125cc26464eca8c37b3c85
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 8043da5a5c354b290b1f8217f8a2f999bb3e5520
                                          8032507b3ff5c1b00da6dace1eee11e8ca30549a)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} b8fd3bd9146566c11fdd634eee1e4623ba35ffab
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 061315c0b205f268a0bce5ee6aaef265cc8e0761
                                          768d99be03034e98acd55e0ba876327d5164f984)
    endif()
else()
    # We require Perl to build OpenSSL

    find_package(Perl REQUIRED QUIET)

    # Set platform specific configuration options

    set(CONFIG_OPTIONS --prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

    if(WIN32)
        list(APPEND CONFIG_OPTIONS no-asm)

        if(RELEASE_MODE)
            set(PLATFORM VC-WIN64A)
        else()
            set(PLATFORM debug-VC-WIN64A)
        endif()
    else()
        list(APPEND CONFIG_OPTIONS shared)

        if(APPLE)
            set(PLATFORM darwin64-x86_64-cc)
        else()
            set(PLATFORM linux-x86_64)

            list(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        endif()
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
            ${MAKE_NMAKE_COMMAND}
        INSTALL_COMMAND
            ${MAKE_NMAKE_COMMAND} install_sw
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${CRYPTO_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SSL_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    if(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDllsAndDuplicateLibs
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
                                 COMMAND ${CMAKE_COMMAND} -E copy lib/${REAL_CRYPTO_IMPORT_LIBRARY} lib/${CRYPTO_IMPORT_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy lib/${REAL_SSL_IMPORT_LIBRARY} lib/${SSL_IMPORT_LIBRARY}
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
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)
