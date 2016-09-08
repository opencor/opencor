macro(Build_Python INSTALL_DIR CLEAN_BUILD)

    # Build and install Python

    set(BUILD_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
    file(MAKE_DIRECTORY ${BUILD_DIRECTORY})

    set(BUILD_OPTIONS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
        -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
        -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
        -DPYTHON_VERSION=${PYTHON_VERSION}

        # Build a shared library
        -DBUILD_LIBPYTHON_SHARED=ON

        # We want development headers
        -DINSTALL_DEVELOPMENT=ON

        ## Statically link extensions into the Python library??
        #-DBUILD_EXTENSIONS_AS_BUILTIN=ON

        # Turn off things we don't want
        -DINSTALL_MANUAL=OFF
        -DBUILD_TESTING=OFF
        -DINSTALL_TEST=OFF

        # Disable extension modules that we don't use
        # or the platform doesn't provide
        -DENABLE_BSDDB=OFF
        -DENABLE_CTYPES_TEST=OFF
        -DENABLE_CURSES=OFF
        -DENABLE_CURSES_PANEL=OFF
        -DENABLE_DBM=OFF
        -DENABLE_GDBM=OFF
        -DENABLE_IPV6=OFF
        -DENABLE_SQLITE3=OFF
        -DENABLE_TESTCAPI=OFF
        -DENABLE_TKINTER=OFF
        # OS/X specific
        -DENABLE_SCPROXY=OFF

        # Use Python's built-in readline
        -DBUILTIN_READLINE=ON

        # Use Python's zlib
        -DUSE_SYSTEM_ZLIB=OFF
        -DUSE_BUILTIN_ZLIB=ON

        # SSL is used by pip
        -DUSE_SYSTEM_OpenSSL=ON
        )

    message(STATUS "Configuring Python...")
    execute_process(COMMAND ${CMAKE_COMMAND}
                            -G ${CMAKE_GENERATOR}
                            ${BUILD_OPTIONS}
                            ${CMAKE_CURRENT_SOURCE_DIR}/PythonCMakeBuild
                    WORKING_DIRECTORY ${BUILD_DIRECTORY}
                    RESULT_VARIABLE PYTHON_CONFIGURE
                    )
    if(PYTHON_CONFIGURE)
        message(FATAL_ERROR "Unable to configure Python: ${PYTHON_CONFIGURE}")
    endif()

    if($CLEAN_BUILD)
        set(CLEAN_OPTION "--clean-first")
    endif()

    message(STATUS "Building Python...")
    execute_process(COMMAND ${CMAKE_COMMAND}
                            ${CLEAN_OPTION}
                            --build ${BUILD_DIRECTORY}
                    WORKING_DIRECTORY ${BUILD_DIRECTORY}
                    RESULT_VARIABLE PYTHON_BUILD
                    )
    if(PYTHON_BUILD)
        message(FATAL_ERROR "Unable to build Python: ${PYTHON_BUILD}")
    endif()

    message(STATUS "Installing Python...")
    execute_process(COMMAND ${CMAKE_COMMAND}
                            --build ${BUILD_DIRECTORY}
                            -- install
                    WORKING_DIRECTORY ${BUILD_DIRECTORY}
                    RESULT_VARIABLE PYTHON_BUILD
                    OUTPUT_QUIET
                    )
    if(PYTHON_BUILD)
        message(FATAL_ERROR "Unable to install Python: ${PYTHON_BUILD}")
    endif()

endmacro()
