PROJECT(OpenSSLPlugin)

# Name and version of our package

SET(PACKAGE_NAME OpenSSL)
SET(PACKAGE_VERSION 1.0.2l)

# Version of our library

SET(LIBRARY_VERSION 1.0.0)

# Git tag for our library

SET(GIT_TAG OpenSSL_1_0_2l)

# Release tag

SET(RELEASE_TAG OpenSSL_1_0_2l)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(CRYPTO_SHARED_LIBRARY libeay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_IMPORT_LIBRARY libeay32${CMAKE_IMPORT_LIBRARY_SUFFIX})

    SET(SSL_SHARED_LIBRARY ssleay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SSL_IMPORT_LIBRARY ssleay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(CRYPTO_IMPORT_LIBRARY ${CRYPTO_SHARED_LIBRARY})

    SET(SSL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl${CMAKE_SHARED_LIBRARY_SUFFIX_PRE}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX_POST})
    SET(SSL_IMPORT_LIBRARY ${SSL_SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${CRYPTO_SHARED_LIBRARY}
    ${SSL_SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${CRYPTO_SHARED_LIBRARY}
    lib/${SSL_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${CRYPTO_IMPORT_LIBRARY}
        lib/${SSL_IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_OPENSSL_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 13b5dcc3bc8c5bb2686280fc609e6d2bb297638e
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES ddd13b715b7abfeda1de8f464ea31724f8046117
                                              896d9da0f878f958aac2d95ce8792640d7c43e5a
                                              a7fecaeaa0be236527bc6080b743e774ee473001
                                              03a2c1afbdac56c3e40725a896fb93044b6a327c)
        ELSE()
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} e06ccb6060ee6950908d355ce782cfb8ad72745e
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 59d7afa7d88fd1e1b8b5f14a321786e287fa181c
                                              d3ac511176d10c7f36e560a91f7714f81efe5b37
                                              b4c5c148e699159b1c7250ade68214b1afac7435
                                              112c2ba0d367332266c68be31f02cfcfd5512386)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 30a9ff6a2c64fca5578ef1aa74476b04fa0a5a25
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 5a81e1885908990d38e9f8bf677ca29afba6d71a
                                          b4b0d19ed8ac439cc02eabcc0445b282e34689be)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} e6b2850c874a5fa600182756b3dd5c90252009d1
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES fd8a94f92f5cbdb9fdd09b47abcc022f84dde95c
                                          2d5d2a42e6bf83a14c30390a895ca351f70b6b19)
    ENDIF()
ELSE()
    # We require perl to build OpenSSL

    FIND_PACKAGE(Perl REQUIRED QUIET)

    # Set platform specific configuration options and tool names

    SET(CONFIG_OPTIONS "--prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            SET(PLATFORM VC-WIN64A)
        ELSE()
            SET(PLATFORM debug-VC-WIN64A)
        ENDIF()

        SET(MAKEFILE /f ms/ntdll.mak)
        SET(INSTALL_TARGET install)
    ELSE()
        LIST(APPEND CONFIG_OPTIONS shared)

        IF(APPLE)
            SET(PLATFORM darwin64-x86_64-cc)
        ELSE()
            SET(PLATFORM linux-x86_64)

            LIST(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        ENDIF()

        SET(MAKEFILE -f Makefile)
        SET(INSTALL_TARGET install_sw)
    ENDIF()

    # Build OpenSSL as an external project

    SET(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    SET(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

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

    IF(WIN32)
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
    ELSE()
        # Make our libraries writable
        # Note: indeed, the build process creates write-protected libraries...

        ExternalProject_Add_Step(${PACKAGE_BUILD} MakeWritable
                                 COMMAND chmod 755 ${CRYPTO_SHARED_LIBRARY}
                                 COMMAND chmod 755 ${SSL_SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                 DEPENDEES install)

        IF(APPLE)
            # Fix the RPATH information

            ExternalProject_Add_Step(${PACKAGE_BUILD} FixRPath
                                     COMMAND install_name_tool -id @rpath/${CRYPTO_SHARED_LIBRARY} ${CRYPTO_SHARED_LIBRARY}
                                     COMMAND install_name_tool -id @rpath/${SSL_SHARED_LIBRARY} ${SSL_SHARED_LIBRARY}
                                     COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DIR}/${CRYPTO_SHARED_LIBRARY}
                                                                       @rpath/${CRYPTO_SHARED_LIBRARY}
                                                                       ${SSL_SHARED_LIBRARY}
                                     WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                                     DEPENDEES MakeWritable)
        ENDIF()
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our OpenSSL package

SET(OPENSSL_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(OPENSSL_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(OPENSSL_CRYPTO_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_SSL_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY}
    PARENT_SCOPE
)

# Add the plugin

SET(OPENSSL_PLUGIN ${PACKAGE_NAME} PARENT_SCOPE)

ADD_PLUGIN(${PACKAGE_NAME}
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
        ${PLUGIN_DEPENDS_ON}
)
