PROJECT(OpenSSLPlugin)

## Override main CMakeLists.txt
##set(USE_PREBUILT_OPENSSL_PLUGIN OFF)

# libgit2 doesn't like OpenSSL 1.1 so use 1.0 until git is fixed...

set(OPENSSL_VERSION "1.0")

# TODO: Used for changing names in dylibs. Should this be "1.0.2"??

set(OPENSSL_LIBRARY_VERSION "1.0.0")

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

set(PACKAGED_FILES include lib)
if(WIN32)
    set(CHECKED_FILES
        lib/libeay32.lib lib/libeay32.dll
        lib/ssleay32.lib lib/ssleay32.dll)
elseif(APPLE)
    set(CHECKED_FILES
        lib/libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
        lib/libssl.${OPENSSL_LIBRARY_VERSION}.dylib)
else()
    set(CHECKED_FILES
        lib/libcrypto.${OPENSSL_LIBRARY_VERSION}.so
        lib/libssl.${OPENSSL_LIBRARY_VERSION}.so)
endif()

if(USE_PREBUILT_OPENSSL_PLUGIN)

    # Retrieve the plugin's file(s)

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
                OpenSSL ${OPENSSL_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES lib/libeay32.lib
                    lib/libeay32.dll
                    lib/ssleay32.lib
                    lib/ssleay32.dll
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                    b9895617dc6c321eaaa7d5eef7e4a922f6fb226a
                    2efcf11e3e8a2a92280c2e267fc1ff3049cb577d
                    a18cbb0332fcef7ce3f3a73fbeb9b463a69632f7
                )
        else()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
            OpenSSL ${OPENSSL_VERSION} 0e7337e56ed9d6580c1437ce0fdd3963641d74e3
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES e6db7d7e402091527ef72f7825a16a35566ebad5
                8fe62552e8ebd8e430ff8051e3e6e2ec3430aab1
            )
    else()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
    endif()

else()
    # Build and install OpenSSL as an external package

    include(cmake/BuildOpenSSL.cmake)

    Build_OpenSSL(${OPENSSL_ROOT_DIR})

    if(WIN32)
        # The install process puts DLLs and import libraries in separate
        # directories. OpenCOR though (currently) expects them to all be
        # in the same directory, so we move the DLLs.

        file(RENAME ${OPENSSL_ROOT_DIR}/bin/libeay32.dll ${OPENSSL_ROOT_DIR}/lib/libeay32.dll)
        file(RENAME ${OPENSSL_ROOT_DIR}/bin/ssleay32.dll ${OPENSSL_ROOT_DIR}/lib/ssleay32.dll)

    elseif(APPLE)
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
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        )
endif()

# FIND_PACKAGE should now succeed. Settings will be exported
# via CMake's cache.

find_package(OpenSSL ${OPENSSL_VERSION} MODULE REQUIRED QUIET)

# A sanity check, just in case CMake has found another OpenSSL...

if(NOT "${OPENSSL_INCLUDE_DIR}" STREQUAL "${OPENSSL_ROOT_DIR}/include")
    message(FATAL_ERROR "Couldn't find our OpenSSL package...")
endif()

set(EXTERNAL_BINARY_DIR ${OPENSSL_ROOT_DIR}/lib)

if(WIN32)
    set(EXTERNAL_BINARIES)
    foreach(LIBRARY ${OPENSSL_LIBRARIES})
        string(REPLACE "${EXTERNAL_BINARY_DIR}/" "" LIBNAME ${LIBRARY})
        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            EXTERNAL_BINARY ${LIBNAME})
        list(APPEND EXTERNAL_BINARIES ${EXTERNAL_BINARY})
    endforeach()
elseif(APPLE)
    set(EXTERNAL_BINARIES
        libcrypto.${OPENSSL_LIBRARY_VERSION}.dylib
        libssl.${OPENSSL_LIBRARY_VERSION}.dylib)
else()
    set(EXTERNAL_BINARIES
        libcrypto.${OPENSSL_LIBRARY_VERSION}.so
        libssl.${OPENSSL_LIBRARY_VERSION}.so)
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
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARY_DIR}
    )
