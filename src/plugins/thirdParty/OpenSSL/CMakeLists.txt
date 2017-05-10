PROJECT(OpenSSLPlugin)

# Version that we want to use

SET(GIT_TAG "OpenSSL_1_0_2k")

# Version identifier for our created package

SET(PACKAGE_VERSION "1.0.2k")

# Library version numbers set by the build

SET(LIBRARY_VERSION "1.0.0")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared library (and of its import library, if any)

IF(WIN32)
    SET(CRYPTO_SHARED_LIBRARY libeay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_IMPORT_LIBRARY libeay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
    SET(SSL_SHARED_LIBRARY    ssleay32${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SSL_IMPORT_LIBRARY    ssleay32${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSEIF(APPLE)
    SET(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(CRYPTO_IMPORT_LIBRARY ${CRYPTO_SHARED_LIBRARY})
    SET(SSL_SHARED_LIBRARY    ${CMAKE_SHARED_LIBRARY_PREFIX}ssl.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(SSL_IMPORT_LIBRARY    ${SSL_SHARED_LIBRARY})
ELSE()
    SET(CRYPTO_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}crypto${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    SET(CRYPTO_IMPORT_LIBRARY ${CRYPTO_SHARED_LIBRARY})
    SET(SSL_SHARED_LIBRARY    ${CMAKE_SHARED_LIBRARY_PREFIX}ssl${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
    SET(SSL_IMPORT_LIBRARY    ${SSL_SHARED_LIBRARY})
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

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_OPENSSL_PLUGIN)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(OpenSSL ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} d19ce0fa534a50f0c3ca377e1adacb31106cad05
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 79fb6b732873bb8b305564c58c7bb058c55d25a2
                            cae8c550a333c9f9cd0852b1a46bf293a65a7b3b
                            55fceaaf36d27d9fbbd7bfa2fa4bb2a7d1d6ea1f
                            593c4f7ab3771229abb98a6847f3750e36e18e05
            )
        ELSE()
            RETRIEVE_PACKAGE_FILE(OpenSSL ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} bbd3f3b7ba9db734a2356db06a5d70151d707180
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES edd5cc505caf87cf218c463d3aa1a37b5b76550a
                            186b94792db9b09872fd53b4b3d596967175e4c1
                            8186d5f4db60680bc218e93e81d394273eca0bb6
                            5ed10c376daf596a026d6c03d7fe962df8f95876
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(OpenSSL ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 137b4201cb2d0bd2dfa5f01ddd04f62b23cd82d2
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 205881f147902223779976ccaedf2ab40077016d
                        6a91eaaeec5490f98371e7ec419d9d41a0fbba84
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(OpenSSL ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} ae07dd8526e627189c6cb995a3b42a9d4ef45070
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 29c32816ce9e98a13da9be7d001acd9342220507
                        88583b407a4625ee157d25b35ce9328e023d8584
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # We require perl for OpenSSL's configuration tool

    INCLUDE(FindPerl)

    IF(NOT PERL_FOUND)
        message(FATAL_ERROR "Perl is missing -- required to configure OpenSSL")
    ENDIF()

    # Set platform specific configuration options and tool names

    SET(CONFIG_OPTIONS "--prefix=${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        SET(PLATFORM "VC-WIN64A")
        SET(MAKE_EXECUTABLE nmake)
        SET(MAKEFILE ms/ntdll.mak)
        SET(QUIET_MAKE -c)
        SET(INSTALL_TARGET install)
    ELSE()
        LIST(APPEND CONFIG_OPTIONS "shared")
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

    # Build OpenSSL as an external project

    SET(PACKAGE_BUILD OpenSSLBuild)
    SET(PACKAGE_BUILD_DIR ${PROJECT_SOURCE_DIR}/ext/proj)

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
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE}
        INSTALL_COMMAND
            ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${QUIET_MAKE} ${INSTALL_TARGET}
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${CRYPTO_IMPORT_LIBRARY}
            <INSTALL_DIR>/lib/${SSL_IMPORT_LIBRARY}
    )

    # Do some post-processing that is specific to each platform

    IF(WIN32)
        # Windows requires an additional configuration step

        ExternalProject_Add_Step(${PACKAGE_BUILD}
            PostConfigure
            COMMAND "ms/do_win64a"
            COMMENT "Post configuration step for 64-bit Windows"
            WORKING_DIRECTORY ${PACKAGE_BUILD_DIR}
            DEPENDEES configure
            DEPENDERS build
        )

        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
            COMMAND ${CMAKE_COMMAND} -E copy bin/${CRYPTO_SHARED_LIBRARY} lib
            COMMAND ${CMAKE_COMMAND} -E copy bin/${SSL_SHARED_LIBRARY} lib
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
        )
    ELSE()
        # Fix up the newly created libraries

        ExternalProject_Add_Step(${PACKAGE_BUILD} FixupLibrary
            # The build process creates write-protected libraries
            COMMAND chmod 755 ${CRYPTO_SHARED_LIBRARY}
            COMMAND chmod 755 ${SSL_SHARED_LIBRARY}
            WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
            DEPENDEES install
        )

        IF(APPLE)
            # Set RPATH to enable linking independent of library location

            ExternalProject_Add_Step(${PACKAGE_BUILD} SetRPath
                COMMAND install_name_tool -id @rpath/${CRYPTO_SHARED_LIBRARY}
                                          ${CRYPTO_SHARED_LIBRARY}
                COMMAND install_name_tool -id @rpath/${SSL_SHARED_LIBRARY}
                                          ${SSL_SHARED_LIBRARY}
                COMMAND install_name_tool -change ${EXTERNAL_BINARIES_DIR}/${CRYPTO_SHARED_LIBRARY}
                                                  @rpath/${CRYPTO_SHARED_LIBRARY}
                                                  ${SSL_SHARED_LIBRARY}
                WORKING_DIRECTORY ${EXTERNAL_BINARIES_DIR}
                DEPENDEES FixupLibrary
            )
        ENDIF()
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(OpenSSL ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES include ${SHA1_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
    )

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our OpenSSL package

SET(OPENSSL_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)
SET(OPENSSL_INCLUDE_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/include PARENT_SCOPE)
SET(OPENSSL_CRYPTO_LIBRARY ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_SSL_LIBRARY ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY} PARENT_SCOPE)
SET(OPENSSL_LIBRARIES
    ${EXTERNAL_BINARIES_DIR}/${CRYPTO_IMPORT_LIBRARY}
    ${EXTERNAL_BINARIES_DIR}/${SSL_IMPORT_LIBRARY} PARENT_SCOPE
)

# Add the plugin

SET(OPENSSL_PLUGIN OpenSSL PARENT_SCOPE)

ADD_PLUGIN(OpenSSL
    SOURCES
        ../../plugininfo.cpp
        src/opensslplugin.cpp
    HEADERS_MOC
        src/opensslplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
