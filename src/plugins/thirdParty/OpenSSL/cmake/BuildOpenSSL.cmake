macro(Build_OpenSSL INSTALL_DIR)

    # Build OpenSSL using its perl based configuration tool

    include(FindPerl)

    if(NOT PERL_FOUND)
        message(FATAL_ERROR "Perl is missing -- required to configure OpenSSL")
    endif()

    ## set(CONFIG_OPTIONS "--no-asm") ???
    set(CONFIG_OPTIONS "--prefix=${INSTALL_DIR}")
    if(NOT WIN32)
        list(APPEND CONFIG_OPTIONS "shared")
    endif()

## Note Windows requirements -- Activestate Perl and VC 2014 assembler
## Also need to patch crypto/x86_64cpuid.pl

## Not for OPENSSL_VERSION < 1.1
#    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#        set(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--debug")
#    else()
#        set(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--release")
#    endif()

    if(WIN32)
        set(PLATFORM "VC-WIN64A")
        set(MAKE_EXECUTABLE nmake)
        set(MAKEFILE ms/ntdll.mak)
        set(INSTALL_TARGET install)
    elseif(APPLE)
        set(PLATFORM "darwin64-x86_64-cc")
        set(MAKE_EXECUTABLE make)
        set(MAKEFILE Makefile)
        set(INSTALL_TARGET install_sw)
    else()
        set(PLATFORM "linux-x86_64")
        list(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        set(MAKE_EXECUTABLE make)
        set(MAKEFILE Makefile)
        set(INSTALL_TARGET install_sw)
    endif()

    # Run configuration script

    execute_process(COMMAND ${PERL_EXECUTABLE} ./Configure ${PLATFORM} ${CONFIG_OPTIONS}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to configure OpenSSL: ${OPTIONS}")
    endif()

    # 64-bit Windows needs patching and has another configuration stage

    if(WIN32)
        # See https://github.com/openssl/openssl/issues/1545 for patch details

        execute_process(COMMAND git apply --check --apply ../patches/windows/OpenSSL.patch
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL)

        execute_process(COMMAND ms/do_win64a.bat
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL)
    endif()

    # Ensure we start from a clean build

    execute_process(COMMAND ${MAKE_EXECUTABLE} -f ${MAKEFILE} clean
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to build OpenSSL dependencies.")
    endif()

    # Compile sources

    execute_process(COMMAND ${MAKE_EXECUTABLE} -f ${MAKEFILE}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to build OpenSSL.")
    endif()

    # Install (into REMOTE_EXTERNAL_BINARIES_DIR)

    execute_process(COMMAND ${MAKE_EXECUTABLE} -f ${MAKEFILE} ${INSTALL_TARGET}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to install OpenSSL.")
    endif()

endmacro()
