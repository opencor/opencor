PROJECT(OpenSSLPlugin)

# libgit2 doesn't like OpenSSL 1.1 so use 1.0 until git is fixed...

set(OPENSSL_VERSION "1.0")
set(OPENSSL_LIBRARY_VERSION "1.0.0")

## Override main CMakeLists.txt
##set(USE_PREBUILT_OPENSSL_PLUGIN OFF)

# Reset OpenSSL variables in FIND_PACKAGE() cache...

unset(OPENSSL_FOUND CACHE)
unset(OPENSSL_INCLUDE_DIR CACHE)
unset(OPENSSL_SSL_LIBRARY CACHE)
unset(OPENSSL_CRYPTO_LIBRARY CACHE)
unset(OPENSSL_LIBRARIES CACHE)
unset(OPENSSL_ROOT_DIR CACHE)
unset(OPENSSL_VERSION CACHE)

set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/packages/${REMOTE_EXTERNAL_BINARIES_DIR}")
string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_ROOT_DIR ${OPENSSL_ROOT_DIR})

if(USE_PREBUILT_OPENSSL_PLUGIN)

    # Retrieve the plugin's file(s)

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
        else()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
            OpenSSL ${OPENSSL_VERSION} 0e7337e56ed9d6580c1437ce0fdd3963641d74e3
            FILES lib/libcrypto.1.0.0.dylib
                  lib/libssl.1.0.0.dylib
            SHA1  e6db7d7e402091527ef72f7825a16a35566ebad5
                  8fe62552e8ebd8e430ff8051e3e6e2ec3430aab1
            )
    else()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
    endif()

else()
    # Build and install OpenSSL as an external package

    include(cmake/BuildOpenSSL.cmake)
    Build_OpenSSL(${OPENSSL_ROOT_DIR})

    if(APPLE)
        # The build process creates write-protected libraries

        EXECUTE_PROCESS(COMMAND chmod 755 libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
                        WORKING_DIRECTORY ${OPENSSL_ROOT_DIR}/lib)

        # Having `@rpath` in the id allows other plugins to find the library.

        EXECUTE_PROCESS(COMMAND install_name_tool -id @rpath/libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
                                                      libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
                        WORKING_DIRECTORY ${OPENSSL_ROOT_DIR}/lib)

        EXECUTE_PROCESS(COMMAND chmod 755 libssl.${OPENSSL_LIBRARY_VERSION}.dylib
                        WORKING_DIRECTORY ${OPENSSL_ROOT_DIR}/lib)
        EXECUTE_PROCESS(COMMAND install_name_tool -id @rpath/libssl.${OPENSSL_LIBRARY_VERSION}.dylib
                                                      libssl.${OPENSSL_LIBRARY_VERSION}.dylib
                        WORKING_DIRECTORY ${OPENSSL_ROOT_DIR}/lib)
        EXECUTE_PROCESS(COMMAND install_name_tool -change ${OPENSSL_ROOT_DIR}/lib/libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
                                                          @rpath/libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
                                                          libssl.${OPENSSL_LIBRARY_VERSION}.dylib
                        WORKING_DIRECTORY ${OPENSSL_ROOT_DIR}/lib)
    endif()

    CREATE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
        OpenSSL ${OPENSSL_VERSION}
        FILES lib include
        SHA1 "lib/libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib"
             "lib/libssl.${OPENSSL_LIBRARY_VERSION}.dylib"
        )
endif()

# FIND_PACKAGE should now succeed. Settings will be exported
# via CMake's cache.

find_package(OpenSSL ${OPENSSL_VERSION} MODULE REQUIRED QUIET)

# A sanity check, just in case CMake has found another OpenSSL...

if(NOT "${OPENSSL_INCLUDE_DIR}" STREQUAL "${OPENSSL_ROOT_DIR}/include")
    message(FATAL_ERROR "Couldn't find our OpenSSL package...")
endif()

# Ensure external packages build by other plugins can find us using CMake

set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} PARENT_SCOPE)


# Add the plugin

ADD_PLUGIN(OpenSSL
    THIRD_PARTY
    SOURCES
        ../../plugininfo.cpp

        src/opensslplugin.cpp
    HEADERS_MOC
        src/opensslplugin.h
    INCLUDE_DIRS
        src
    QT_MODULES
        Core
    EXTERNAL_BINARIES
        libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
        libssl.${OPENSSL_LIBRARY_VERSION}.dylib
    EXTERNAL_BINARIES_DIR
        ${OPENSSL_ROOT_DIR}/lib
    )
