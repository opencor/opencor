project(PythonPlugin)

# Version of our library

set(MAJOR_MINOR_LIBRARY_VERSION "3.5")
set(LIBRARY_VERSION "${MAJOR_MINOR_LIBRARY_VERSION}.2")
string(REPLACE "." "" NUMERIC_LIBRARY_VERSION ${MAJOR_MINOR_LIBRARY_VERSION})

# Git tag of our Python sources

set(GIT_TAG "opencor")

# Git tags of Python package installers

set(PIP_GIT_TAG "9.0.3")
set(SETUPTOOLS_GIT_TAG "v39.0.1")
set(WHEEL_GIT_TAG "0.31.0")

# Name and version of our package

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION ${LIBRARY_VERSION})

if(WIN32)
    # The version of SQLite to bundle

    set(SQLITE3_VERSION "3170000")
endif()

# Set the ABI flag and version identifier

set(PYTHON_ABIFLAGS "m")
if(WIN32)
    set(PYTHON_BUILT_VERSION ${NUMERIC_LIBRARY_VERSION})
else()
    set(PYTHON_BUILT_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS})
endif()

# Name of our package's shared libraries (and of their import libraries, if any)

if(WIN32)
    set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# The root directory of the installed Python

if(WIN32)
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/../Python)
elseif(APPLE)
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/Python)
else()
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/..)
endif()

# Include, library and script directories

if(WIN32)
    set(LIBRARY_DIR Libs)
    set(RUNTIME_DIR lib)
    set(SCRIPT_DIR Scripts)
else()
    set(LIBRARY_DIR lib)
    set(RUNTIME_DIR lib/python${MAJOR_MINOR_LIBRARY_VERSION})
    set(SCRIPT_DIR bin)
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LIBRARY_DIR})

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    ${LIBRARY_DIR}/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${IMPORT_LIBRARY}
    )
endif()

if(NOT APPLE)
    # We bundle SQLite3 with Python (we link statically when APPLE)

    set(SQLITE3_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})

    list(APPEND EXTERNAL_BINARIES
        ${SQLITE3_SHARED_LIBRARY}
    )

    list(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
    )

    if(WIN32)
        # Windows also needs the import library

        set(SQLITE3_IMPORT_LIBRARY ${CMAKE_IMPORT_LIBRARY_PREFIX}sqlite3${CMAKE_IMPORT_LIBRARY_SUFFIX})

        list(APPEND EXTERNAL_BINARIES
            ${SQLITE3_IMPORT_LIBRARY}
        )

        list(APPEND SHA1_FILES
            ${LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}
        )
    endif()
endif()

# Add headers, scripts and libraries to the package archive

set(PACKAGED_FILES
    bin
    include
    ${RUNTIME_DIR}
    ${SHA1_FILES}
    )

if(UNIX)
    list(APPEND PACKAGED_FILES ${RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION})
elseif(WIN32)
    list(APPEND PACKAGED_FILES Scripts)
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 0bef54c1816b2492be5ed4fabf74fffadcd75e5e
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES a25177a2023aba0ef198daa29752e41ee47d5e82
                                              ef9568913a983473eb1013dc4dfba263de5b2c68
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              65a31aeafe260b7cb34e94da3a32c9d30916a2ce)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 80cf0c6ea628d3037d90ed3f458ae6b8026ff4b8
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 48d8641446d5294ef7557de796f70df57d9cdaa9)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 3d3fcc223d569729ab6fa2f11112fe5eb93e68b5
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 62c58fa8c94b094097ee41907dbd64d670e3dbee
                                          81aa5a7f1370dab5bd5c5f4e7d94c71c9c57f834)
    endif()
else()
    # Set platform specific configuration options

    if(UNIX AND NOT APPLE)
        set(CMAKE_INSTALL_RPATH_OPTION -DCMAKE_INSTALL_RPATH=$ORIGIN/../lib)
        # Use the system's readline
        set(PYTHON_READLINE_OPTION -DUSE_SYSTEM_READLINE=ON)
    else()
        set(CMAKE_INSTALL_RPATH_OPTION)
        # Use Python's built-in readline
        set(PYTHON_READLINE_OPTION -DBUILTIN_READLINE=ON)
    endif()

    # Jupyter and IPython require SQLite3
    # This could/should be in its own plugin...

    if(WIN32)
        set(SQLITE3_LIBRARY_DIR ${PROJECT_BINARY_DIR}/sqlite3)

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h)
            set(SQLITE3_SOURCES_ZIP sqlite-amalgamation-${SQLITE3_VERSION}.zip)
            message(STATUS "Downloading ${SQLITE3_SOURCES_ZIP}")
            file(DOWNLOAD https://sqlite.org/2017/${SQLITE3_SOURCES_ZIP}
                ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_SOURCES_ZIP}
                SHOW_PROGRESS
                )
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xf zips/${SQLITE3_SOURCES_ZIP}
                                                   sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h
                WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                )
        endif()

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite3.lib)
            set(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)
            message(STATUS "Downloading ${SQLITE3_DLL_ZIP}")
            file(DOWNLOAD https://sqlite.org/2017/${SQLITE3_DLL_ZIP}
                ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_DLL_ZIP}
                SHOW_PROGRESS
                )
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xf zips/${SQLITE3_DLL_ZIP} --format=zip
                WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                )
            execute_process(
                COMMAND lib /verbose /def:sqlite3.def /out:sqlite3.lib /machine:x64
                WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                )
        endif()

        set(SQLITE3_INCLUDE_DIR ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION})
    elseif(APPLE)
        # Use Homebrew's SQLite

        set(SQLITE3_INCLUDE_DIR /usr/local/opt/sqlite/include)
        set(SQLITE3_LIBRARY_DIR /usr/local/opt/sqlite/lib)

        # Link to static library so there is no need to bundle it

        set(SQLITE3_IMPORT_LIBRARY libsqlite3.a)
    else()
        set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

        find_package(SQLite3 REQUIRED)

        get_filename_component(SQLITE3_LIBRARY_DIR ${SQLITE3_LIBRARY} DIRECTORY)

        get_filename_component(SQLITE3_IMPORT_LIBRARY ${SQLITE3_LIBRARY} NAME)
    endif()

    if(NOT APPLE)
        # Copy the SQLite3 library to the plugin's external binaries directory

        # First make sure the destination directory exists

        file(MAKE_DIRECTORY ${EXTERNAL_BINARIES_DIR})

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})
        if(WIN32)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        endif()
    endif()

    # The type of build

    if(WIN32)
        set(APPLY_PATCHES ON)
        set(BUILD_TYPE Release)
    else()
        set(APPLY_PATCHES OFF)
        set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
    endif()

    # Build Python as an external project

    set(PACKAGE_BUILD PythonBuild)

    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            OpenSSLPlugin
            zlibPlugin
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/python-cmake-buildsystem.git
        GIT_TAG
            ${GIT_TAG}

        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

            ${CMAKE_INSTALL_RPATH_OPTION}

            # Options for MacOS builds
            -DCMAKE_MACOSX_RPATH=1
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}

            # The version of Python to build
            -DPYTHON_VERSION=${LIBRARY_VERSION}

            # Build a shared library
            -DBUILD_LIBPYTHON_SHARED=ON

            # We want development headers
            -DINSTALL_DEVELOPMENT=ON

            # IPython and Jupyter need SQLite
            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_DIR}
            -DSQLITE3_LIBRARY=${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}

            # Turn off things we don't want
            -DINSTALL_MANUAL=OFF
            -DBUILD_TESTING=OFF
            -DINSTALL_TEST=OFF

            # Optionally patch sources (for Windows)
            -DPYTHON_APPLY_PATCHES=${APPLY_PATCHES}

            # Disable extension modules that we don't use
            # or the platform doesn't provide
            -DENABLE_BSDDB=OFF
            -DENABLE_CTYPES_TEST=OFF
            -DENABLE_CURSES=OFF
            -DENABLE_CURSES_PANEL=OFF
            -DENABLE_DBM=OFF
            -DENABLE_GDBM=OFF
            -DENABLE_IPV6=OFF
            -DENABLE_TESTCAPI=OFF
            -DENABLE_TKINTER=OFF

            # Build with an appropriate readline
            ${PYTHON_READLINE_OPTION}

            # Use our OpenSSL
            -DUSE_SYSTEM_OpenSSL=ON
            -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
            -DOPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}
            -DOPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}

            # Use our zlib
            -DUSE_SYSTEM_ZLIB=ON
            -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
            -DZLIB_LIBRARY=${ZLIB_LIBRARY}

            # OS/X specific; needed to install pip
            -DENABLE_SCPROXY=ON

        BUILD_BYPRODUCTS
            <INSTALL_DIR>/${LIBRARY_DIR}/${IMPORT_LIBRARY}
    )

    if(NOT WIN32)
        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_headers
            COMMAND ${CMAKE_COMMAND} -E copy_directory include/python${PYTHON_BUILT_VERSION} include
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
            )
    endif()

    if(WIN32)
        # _freeze_importlib.exe, run as part of the Python build, needs our zlib DLL...

        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            ZLIB_DLL "${ZLIB_LIBRARY}")
        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_zlib
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} <BINARY_DIR>/CMakeBuild/libpython
            DEPENDEES configure
            DEPENDERS build
            )

        # ADD_PLUGIN expects our Python DLL in the same directory as the import libraries
        # and we must use our DLLs when we run Python in subsequent steps below

        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_CRYPTO_DLL "${OPENSSL_CRYPTO_LIBRARY}")
        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_SSL_DLL "${OPENSSL_SSL_LIBRARY}")

        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_dlls
            COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} Libs
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_CRYPTO_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_SSL_DLL} bin
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
            )

        set(INSTALL_STEP copy_dlls)
    elseif(APPLE)
        # Fix up the newly installed Python executable and shared library
        # - add RPATH to executable so it can use already installed libraries when run at build time
        # - add RPATH to executable so it can find Python library when run at build time
        # - add RPATH to executable so it can use libraries when installed (into Frameworks/Python/bin)
        # - use a relative path to the Python library
        # - set RPATH in Python library to enable linking independent of location

        ExternalProject_Add_Step(${PACKAGE_BUILD} set_rpath
            COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} bin/python
            COMMAND install_name_tool -add_rpath @executable_path/.. bin/python
            COMMAND install_name_tool -add_rpath @executable_path/../.. bin/python
            COMMAND install_name_tool -change ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY}
                                              @rpath/lib/${SHARED_LIBRARY}
                                              bin/python
            COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} lib/${SHARED_LIBRARY}
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
            )

        set(INSTALL_STEP set_rpath)
    else()
        set(INSTALL_STEP install)
    endif()

    # Clean our site-packages directory before installing any packages

    ExternalProject_Add_Step(${PACKAGE_BUILD} clean_site
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${RUNTIME_DIR}/site-packages
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_DIR}/site-packages
        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        DEPENDEES ${INSTALL_STEP}
        )

    # Use our newly built python

    if(WIN32)
        set(BUILT_PYTHON  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python.exe)
    elseif(NOT APPLE)
        set(BUILT_PYTHON "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    else()
        set(BUILT_PYTHON  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    endif()

    if(NOT WIN32)
        # Install the Python package manager, wheel, and dependencies

        ExternalProject_Add_Step(${PACKAGE_BUILD} install_pip
            COMMAND ${BUILT_PYTHON} -s ${PROJECT_SOURCE_DIR}/scripts/get-pip.py
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin
            DEPENDEES clean_site
            )

        set(CREATE_PACKAGE_TARGET ${PACKAGE_BUILD})
    else()
        # For Windows we need to explicitly install setuptools, pip and wheel in order to
        # create `*-script-py` files that we can then modify at OpenCOR installation time
        # to reference the newly installed Python executable.

        # Install the Python setuptools package

        set(SETUPTOOLS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/setuptools)

        ExternalProject_Add(install_setuptools
            DEPENDS
                ${PACKAGE_BUILD}
            DOWNLOAD_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            SOURCE_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            BINARY_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            INSTALL_DIR
                ${SETUPTOOLS_SOURCE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/setuptools.git
            GIT_TAG
                ${SETUPTOOLS_GIT_TAG}
            CONFIGURE_COMMAND
                ${BUILT_PYTHON} bootstrap.py
            BUILD_COMMAND
                ${BUILT_PYTHON} setup.py build
            INSTALL_COMMAND
                ${BUILT_PYTHON} setup.py install
            )

        # Install the Python package installer

        set(PIP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/pip)

        ExternalProject_Add(install_pip
            DEPENDS
                install_setuptools
            DOWNLOAD_DIR
                ${PIP_SOURCE_DIR}
            SOURCE_DIR
                ${PIP_SOURCE_DIR}
            BINARY_DIR
                ${PIP_SOURCE_DIR}
            INSTALL_DIR
                ${PIP_SOURCE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/pip.git
            GIT_TAG
                ${PIP_GIT_TAG}
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${BUILT_PYTHON} setup.py build
            INSTALL_COMMAND
                ${BUILT_PYTHON} setup.py install
            )

        # Install the wheel Python package

        set(WHEEL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/wheel)

        ExternalProject_Add(install_wheel
            DEPENDS
                install_pip
            DOWNLOAD_DIR
                ${WHEEL_SOURCE_DIR}
            SOURCE_DIR
                ${WHEEL_SOURCE_DIR}
            BINARY_DIR
                ${WHEEL_SOURCE_DIR}
            INSTALL_DIR
                ${WHEEL_SOURCE_DIR}
            GIT_REPOSITORY
                https://github.com/opencor/wheel.git
            GIT_TAG
                ${WHEEL_GIT_TAG}
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${BUILT_PYTHON} setup.py build
            INSTALL_COMMAND
                ${BUILT_PYTHON} setup.py install
            )

        set(CREATE_PACKAGE_TARGET install_wheel)
    endif()

    # Package Python's include files and libraries

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES ${PACKAGED_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${CREATE_PACKAGE_TARGET}
        )

    set(PLUGIN_DEPENDS_ON ${CREATE_PACKAGE_TARGET})
endif()

# Allow other external projects to find our Python

set(PYTHON_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)

if(WIN32)
    set(PYTHON_EXECUTABLE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python.exe)
else()
    set(PYTHON_EXECUTABLE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
endif()

set(PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} PARENT_SCOPE)

set(PYTHON_RELATIVE_SCRIPT_DIR ${SCRIPT_DIR} PARENT_SCOPE)
set(PYTHON_SCRIPT_DIR  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR} PARENT_SCOPE)

set(PYTHON_RELATIVE_RUNTIME_DIR ${RUNTIME_DIR} PARENT_SCOPE)
set(PYTHON_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${RUNTIME_DIR}/site-packages PARENT_SCOPE)

set(PYTHON_VERSION ${LIBRARY_VERSION} PARENT_SCOPE)
set(PYTHON_FULL_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS} PARENT_SCOPE)

# Add the plugin

add_plugin(Python
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/pythonplugin.cpp
    HEADERS_MOC
        src/pythonplugin.h
    PLUGINS
        Core
        OpenSSL
        zlib
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
