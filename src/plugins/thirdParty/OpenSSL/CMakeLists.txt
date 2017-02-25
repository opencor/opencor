PROJECT(OpenSSLPlugin)

# Increment this number before building a new package to a value larger
# larger than its maximum value over all git branches. This is to ensure
# that the correct package archive is retrieved when the build configuration
# is changed (e.g. by changing the git branch).

SET(BUILD_NUMBER 1)

# Specify the version of OpenSSL we want to build

SET(OPENSSL_VERSION "1.0.0")
SET(OPENSSL_PACKAGE_VERSION ${OPENSSL_VERSION}-${BUILD_NUMBER})

# Specify the git tag in the OpenSSL repository that we want to use

SET(OPENSSL_GIT_TAG "OpenSSL_1_0_2k")

# The names of the libraries built by OpenSSL's build process

IF(WIN32)
    SET(CRYPTO_SHARED_LIBRARY libeay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_LINK_LIBRARY libeay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
    SET(SSL_SHARED_LIBRARY ssleay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SSL_LINK_LIBRARY ssleay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto.${OPENSSL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_LINK_LIBRARY ${CRYPTO_SHARED_LIBRARY})
    SET(SSL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl.${OPENSSL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SSL_LINK_LIBRARY ${SSL_SHARED_LIBRARY})
ELSE()
    SET(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto${CMAKE_SHARED_LIBRARY_SUFFIX}.${OPENSSL_VERSION})
    SET(CRYPTO_LINK_LIBRARY ${CRYPTO_SHARED_LIBRARY})
    SET(SSL_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl${CMAKE_SHARED_LIBRARY_SUFFIX}.${OPENSSL_VERSION})
    SET(SSL_LINK_LIBRARY ${SSL_SHARED_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${CRYPTO_SHARED_LIBRARY} ${SSL_SHARED_LIBRARY})

# Specify where our local OpenSSL will be installed

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged

SET(CHECKED_FILES
    lib/${CRYPTO_SHARED_LIBRARY}
    lib/${SSL_SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND CHECKED_FILES
        lib/${CRYPTO_LINK_LIBRARY}
        lib/${SSL_LINK_LIBRARY}
    )
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_OPENSSL_PLUGIN)

    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                OpenSSL ${OPENSSL_PACKAGE_VERSION} 214d07f2b61c24018514213d4945883d0fbc32f9
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 97ee464e4c5c8a73e1f4530f009bee9792ce5ce8
                            eb96ee3dd2bf200cc8a322c29632ab91425b345e
                            427d0649f695509638939f2a34f315ef8a470c2b
                            6d304ee7cf50c8d3df2ce5bae44f68973757b0f5
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                OpenSSL ${OPENSSL_PACKAGE_VERSION} e99a1ac8ec6680d1375b9def950c18b59ac715ea
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 4af9b1613f3489a753df9690f01c7a16f883d40b
                            53240b1fc6d9ddcdacff79b1847c921c8e2673ce
                            45639be11e00eeb34c3890089d3ef218c0862301
                            10e33f1e800269c5136ce9f0b07deda72ff34513
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            OpenSSL ${OPENSSL_PACKAGE_VERSION} 5e0698b9c7ce2c3eeec841302ea3b04d77a924da
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 48b0abbc61b40a9e54a6196dab180322ec7f41a1
                        140ce0a6149e936aecdbd646272d34186ac782e9
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            OpenSSL ${OPENSSL_PACKAGE_VERSION} 50761a0b6f217710c9b0ef901ad59bdc89961e18
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES c448bb86fee3ccf8600061b74953b375f5ca2f6f
                        88583b407a4625ee157d25b35ce9328e023d8584
        )
    ENDIF()

ELSE()
    # Build and install OpenSSL as an external package

    INCLUDE(ExternalProject)

    # We required perl for OpenSSL's configuration tool

    INCLUDE(FindPerl)

    IF(NOT PERL_FOUND)
        message(FATAL_ERROR "Perl is missing -- required to configure OpenSSL")
    ENDIF()

## Note Windows requirements -- Activestate Perl and VC 2014 assembler
## Also need to patch crypto/x86_64cpuid.pl

    SET(CONFIG_OPTIONS "--prefix=${FULL_INSTALL_DIR}")
    IF(NOT WIN32)
        LIST(APPEND CONFIG_OPTIONS "shared")
    ENDIF()

## Not for OPENSSL_VERSION < 1.1
#    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#        SET(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--debug")
#    ELSE()
#        SET(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--release")
#    ENDIF()

    IF(WIN32)
        SET(PLATFORM "VC-WIN64A")
        SET(MAKE_EXECUTABLE nmake)
        SET(MAKEFILE ms/ntdll.mak)
        SET(QUIET_MAKE -c)
        SET(INSTALL_TARGET install)
    ELSE()
        IF(APPLE)
            SET(PLATFORM "darwin64-x86_64-cc")
        ELSE()
            SET(PLATFORM "linux-x86_64")
            LIST(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        ENDIF()
        SET(MAKE_EXECUTABLE make)
        SET(MAKEFILE Makefile)
        SET(QUIET_MAKE --quiet)
        SET(INSTALL_TARGET install_sw)
    ENDIF()

    SET(OPENSSL_BUILD_DIR ${PROJECT_SOURCE_DIR}/OpenSSL)

    ExternalProject_Add(OpenSSLBuild
        DOWNLOAD_DIR
            ${OPENSSL_BUILD_DIR}
        SOURCE_DIR
            ${OPENSSL_BUILD_DIR}
        BINARY_DIR
            ${OPENSSL_BUILD_DIR}
        INSTALL_DIR
            ${FULL_INSTALL_DIR}
        GIT_REPOSITORY
            https://github.com/openssl/openssl
        GIT_TAG
            ${OPENSSL_GIT_TAG}
        CONFIGURE_COMMAND
            ${PERL_EXECUTABLE} ./Configure ${PLATFORM} ${CONFIG_OPTIONS}
        BUILD_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE} ${INSTALL_TARGET}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${CRYPTO_LINK_LIBRARY}
            <INSTALL_DIR>/lib/${SSL_LINK_LIBRARY}
    )

    IF(WIN32)
        # Windows build has a post configuration step

        ExternalProject_Add_Step(OpenSSLBuild
            PostConfigure
            COMMAND "ms/do_win64a"
            COMMENT "Post configuration step for 64-bit Windows"
            WORKING_DIRECTORY ${OPENSSL_BUILD_DIR}
            DEPENDEES configure
            DEPENDERS build
        )

        # Our plugin code expects the DLLs in the same directory as the import libraries.

        ExternalProject_Add_Step(OpenSSLBuild CopyDLL
            COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
            COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}
            DEPENDEES install
        )
    ELSE()
        # Fix up the newly created libraries

        ExternalProject_Add_Step(OpenSSLBuild FixupLibrary
            # The build process creates write-protected libraries
            COMMAND chmod 755 ${CRYPTO_SHARED_LIBRARY}
            COMMAND chmod 755 ${SSL_SHARED_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
            DEPENDEES install
        )

        IF(APPLE)
            ExternalProject_Add_Step(OpenSSLBuild SetRPath
                # Set RPATH to enable linking independent of library location
                COMMAND install_name_tool -id @rpath/${CRYPTO_SHARED_LIBRARY} ${CRYPTO_SHARED_LIBRARY}
                COMMAND install_name_tool -id @rpath/${SSL_SHARED_LIBRARY} ${SSL_SHARED_LIBRARY}
                COMMAND install_name_tool -change ${FULL_INSTALL_DIR}/lib/${CRYPTO_SHARED_LIBRARY}
                                                  @rpath/${CRYPTO_SHARED_LIBRARY}
                                                  ${SSL_SHARED_LIBRARY}
                WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
                DEPENDEES FixupLibrary
            )
        ENDIF()
    ENDIF()

    # Package OpenSSL's include files and library

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        OpenSSL ${OPENSSL_PACKAGE_VERSION}
        PACKAGED_FILES include ${CHECKED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        TARGET OpenSSLBuild
    )

    SET(PLUGIN_DEPENDS_ON OpenSSLBuild)
ENDIF()

# Allow other external projects to find our OpenSSL

SET(OPENSSL_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)
SET(OPENSSL_INCLUDE_DIR ${FULL_INSTALL_DIR}/include PARENT_SCOPE)
SET(OPENSSL_CRYPTO_LIBRARY ${FULL_INSTALL_DIR}/lib/${CRYPTO_LINK_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_SSL_LIBRARY ${FULL_INSTALL_DIR}/lib/${SSL_LINK_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_LIBRARIES ${FULL_INSTALL_DIR}/lib/${CRYPTO_LINK_LIBRARY}
                      ${FULL_INSTALL_DIR}/lib/${SSL_LINK_LIBRARY} PARENT_SCOPE)

# Add the plugin

SET(OPENSSL_PLUGIN OpenSSL PARENT_SCOPE)

ADD_PLUGIN(OpenSSL
    SOURCES
        ../../plugininfo.cpp

        src/opensslplugin.cpp
    HEADERS_MOC
        src/opensslplugin.h
    INCLUDE_DIRS
        ${RELATIVE_INSTALL_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${FULL_INSTALL_DIR}/lib
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)
