macro(Build_OpenSSL INSTALL_DIR)

    # Build OpenSSL using its perl based configuration tool

    include(FindPerl)

    if(NOT PERL_FOUND)
        message(FATAL_ERROR "Perl is missing -- required to configure OpenSSL")
    endif()

    ## set(CONFIG_OPTIONS "--no-asm") ???

    set(CONFIG_OPTIONS "shared" "--prefix=${INSTALL_DIR}")

## Not for OPENSSL_VERSION < 1.1
#    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
#        set(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--debug")
#    else()
#        set(CONFIG_OPTIONS ${CONFIG_OPTIONS} "--release")
#    endif()

    if(WIN32)
        set(PLATFORM "VC-WIN64A")
## Needs  > ms\do_win64a
##        > nmake -f ms\ntdll.mak
## See INSTALL.W64
        set(MAKE_EXECUTABLE nmake)
    elseif(APPLE)
        set(PLATFORM "darwin64-x86_64-cc")
        set(MAKE_EXECUTABLE make)
    else()
        set(PLATFORM "linux-x86_64")
        list(APPEND CONFIG_OPTIONS "-Wa,--noexecstack")
        set(MAKE_EXECUTABLE make)
    endif()

    # Run configuration script


#    set(CONFIG_OPTIONS ${PLATFORM} ${CONFIG_OPTIONS})
#    string(REPLACE ";" " " OPTIONS "${CONFIG_OPTIONS}")

    execute_process(COMMAND ${PERL_EXECUTABLE} ./Configure ${PLATFORM} ${CONFIG_OPTIONS}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to configure OpenSSL: ${OPTIONS}")
    endif()

    # Ensure we start from a clean build

    execute_process(COMMAND ${MAKE_EXECUTABLE} clean
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to build OpenSSL dependencies.")
    endif()

    # Compile sources

    execute_process(COMMAND ${MAKE_EXECUTABLE}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to build OpenSSL.")
    endif()

    # Install (into REMOTE_EXTERNAL_BINARIES_DIR)

    execute_process(COMMAND ${MAKE_EXECUTABLE} install_sw
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/OpenSSL
                    RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to install OpenSSL.")
    endif()

endmacro()
