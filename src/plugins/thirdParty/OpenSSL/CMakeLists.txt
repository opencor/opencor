PROJECT(OpenSSLPlugin)

# Specify the version of OpenSSL we want to build

SET(OPENSSL_VERSION "1.0.0")

# Specify the git tag in the OpenSSL repository that we want to use

SET(OPENSSL_GIT_TAG "OpenSSL_1_0_2j")

# The names of the libraries built by OpenSSL's build process

IF(WIN32)
    IF(RELEASE_MODE)
        SET(CRYPTO_LIBRARY libeay32${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(CRYPTO_GENERATED_LIBRARY libeay32.lib)
        SET(OPENSSL_LIBRARY ssleay32${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(OPENSSL_GENERATED_LIBRARY ssleay32.lib)
    ELSE()
        SET(OPENSSL_LIBRARY OpenSSLd${CMAKE_SHARED_LIBRARY_SUFFIX})
        SET(OPENSSL_GENERATED_LIBRARY OpenSSLd.lib)
    ENDIF()
ELSEIF(APPLE)
    SET(CRYPTO_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto.${OPENSSL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_GENERATED_LIBRARY ${CRYPTO_LIBRARY})
    SET(OPENSSL_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl.${OPENSSL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(OPENSSL_GENERATED_LIBRARY ${OPENSSL_LIBRARY})
ELSE()
    SET(CRYPTO_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto${CMAKE_SHARED_LIBRARY_SUFFIX}.${OPENSSL_VERSION})
    SET(CRYPTO_GENERATED_LIBRARY ${CRYPTO_LIBRARY})
    SET(OPENSSL_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}ssl${CMAKE_SHARED_LIBRARY_SUFFIX}.${OPENSSL_VERSION})
    SET(OPENSSL_GENERATED_LIBRARY ${OPENSSL_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${CRYPTO_LIBRARY} ${OPENSSL_LIBRARY})

# Specify where our local OpenSSL will be installed

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged

SET(CHECKED_FILES lib/${CRYPTO_LIBRARY})
LIST(APPEND CHECKED_FILES lib/${OPENSSL_LIBRARY})
IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${CRYPTO_GENERATED_LIBRARY})
    LIST(APPEND CHECKED_FILES lib/${OPENSSL_GENERATED_LIBRARY})
ENDIF()

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_OPENSSL_PLUGIN)

    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
                OpenSSL ${OPENSSL_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                    b9895617dc6c321eaaa7d5eef7e4a922f6fb226a
                    2efcf11e3e8a2a92280c2e267fc1ff3049cb577d
                    a18cbb0332fcef7ce3f3a73fbeb9b463a69632f7
                )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            OpenSSL ${OPENSSL_VERSION} 6fa6909703b2b7b6db26f08728a07e47fe8cb9b1
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 967fb3221bf5c6def0e2b82f1e4b7076f4db1931
                        9c6e9c4b7aa598c87082ecad09633a7c05883482
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
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
        SET(INSTALL_TARGET install_sw)
    ENDIF()

    SET(OPENSSL_BUILD_DIR ${PROJECT_SOURCE_DIR}/OpenSSL)

    ExternalProject_Add(
        OpenSSLBuild
        DOWNLOAD_DIR ${OPENSSL_BUILD_DIR}
        SOURCE_DIR ${OPENSSL_BUILD_DIR}
        BINARY_DIR ${OPENSSL_BUILD_DIR}
        INSTALL_DIR ${FULL_INSTALL_DIR}
        GIT_REPOSITORY https://github.com/openssl/openssl
        GIT_TAG ${OPENSSL_GIT_TAG}
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ${PERL_EXECUTABLE} ./Configure ${PLATFORM} ${CONFIG_OPTIONS}
        BUILD_COMMAND ${MAKE_EXECUTABLE} -f ${MAKEFILE} --quiet
        INSTALL_COMMAND ${MAKE_EXECUTABLE} -f ${MAKEFILE} --quiet ${INSTALL_TARGET}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${CRYPTO_GENERATED_LIBRARY}
            <INSTALL_DIR>/lib/${OPENSSL_GENERATED_LIBRARY}
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
            COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_LIBRARY} lib
            COMMAND ${CMAKE_COMMAND} -E copy bin/${OPENSSL_LIBRARY} lib
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}
            DEPENDEES install
            )

    ELSEIF(APPLE)
        # Fix up the newly created libraries

        ExternalProject_Add_Step(OpenSSLBuild FixupLibrary
            # The build process creates write-protected libraries
            COMMAND chmod 755 ${CRYPTO_LIBRARY}
            COMMAND chmod 755 ${OPENSSL_LIBRARY}
            # Set RPATH to enable linking independent of library location
            COMMAND install_name_tool -id @rpath/${CRYPTO_LIBRARY} ${CRYPTO_LIBRARY}
            COMMAND install_name_tool -id @rpath/${OPENSSL_LIBRARY} ${OPENSSL_LIBRARY}
            COMMAND install_name_tool -change ${FULL_INSTALL_DIR}/lib/${CRYPTO_LIBRARY}
                                              @rpath/${CRYPTO_LIBRARY}
                                              ${OPENSSL_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
            DEPENDEES install
            )
    ENDIF()

    # Package OpenSSL's include files and library

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        OpenSSL ${OPENSSL_VERSION}
        PACKAGED_FILES include ${CHECKED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        DEPENDENCY OpenSSLBuild
        )
ENDIF()

# Allow other external projects to find our OpenSSL

SET(OPENSSL_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)

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
    )

# We depend on OpenSSL being built if we are not using the prebuilt version

IF(NOT USE_PREBUILT_OPENSSL_PLUGIN)
    ADD_DEPENDENCIES(OpenSSLPlugin OpenSSLBuild)
ENDIF()
