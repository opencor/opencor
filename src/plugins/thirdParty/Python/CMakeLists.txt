project(PythonPlugin)

# Name and version of our package

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION 3.5.2)

# Version of our library

set(MAJOR_MINOR_LIBRARY_VERSION "3.5")
set(LIBRARY_VERSION "${MAJOR_MINOR_LIBRARY_VERSION}.2")

# Git tag of our Python sources

set(GIT_TAG "opencor")

# Git tags of Python package installers

set(PIP_GIT_TAG "10.0.1")
set(SETUPTOOLS_GIT_TAG "v39.0.1")
set(WHEEL_GIT_TAG "0.31.0")

# Specify where our local package will be installed

set(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Set the ABI flag and version identifier

set(PYTHON_ABIFLAGS "m")

if(WIN32)
    string(REPLACE "." ""
           NUMERIC_LIBRARY_VERSION ${MAJOR_MINOR_LIBRARY_VERSION})

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
    # On Windows and Linux, we bundle SQLite3 with Python
    # Note: it is statically linked on macOS...

    set(SQLITE3_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})

    list(APPEND EXTERNAL_BINARIES
        ${SQLITE3_SHARED_LIBRARY}
    )

    list(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
    )

    if(WIN32)
        # On Windows, we also need the import library

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

if(WIN32)
    list(APPEND PACKAGED_FILES Scripts)
elseif()
    list(APPEND PACKAGED_FILES ${RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION})
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's package

    string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} 80db75c6e1d5352dd161f9ca45d0b035f50492c7
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 113534a88c69e5f2e42610780241cc4c360e8a9e
                                              d77a93a60a9836b4b8bed7e78351c5cdfae560be
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              d8977d62ed146e8772e130b4e2cfd89ae39038a2)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b8e73e7881b5638b18c292eb3b264b13ae9f3418
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 85067a1029b2b4bc7b2c07feccffe764ba75c237
                                              bce763074ed196f584130b6f0f673ba765e1155e
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              af95b843f3bfd71c0db3d0ed4965d07e27191e5b)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} a9780980aea194ea5a0c50b571a6bd9a3c81ecd6
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES b6eb87411dd4397064294cbc38b0cd15a2e62ca7)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 8ac72b1f77188cd73b5ac70ff04aaaa714b800d6
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES da682e29567161cccbc3a9b457835be9ecbfb431
                                          81aa5a7f1370dab5bd5c5f4e7d94c71c9c57f834)
    endif()
else()
    # Set platform-specific configuration options

    if(WIN32 OR APPLE)
        set(CMAKE_INSTALL_RPATH_OPTION)

        set(PYTHON_READLINE_OPTION -DBUILTIN_READLINE=ON)
    else()
        set(CMAKE_INSTALL_RPATH_OPTION -DCMAKE_INSTALL_RPATH=$ORIGIN/../lib)

        set(PYTHON_READLINE_OPTION -DUSE_SYSTEM_READLINE=ON)
    endif()

    # Jupyter and IPython require SQLite3

    if(WIN32)
        set(SQLITE3_VERSION "3170000")
        set(SQLITE3_LIBRARY_DIR ${PROJECT_BINARY_DIR}/sqlite3)

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h)
            set(SQLITE3_SOURCES_ZIP sqlite-amalgamation-${SQLITE3_VERSION}.zip)

            message("Downloading ${SQLITE3_SOURCES_ZIP}...")

            file(DOWNLOAD https://sqlite.org/2017/${SQLITE3_SOURCES_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_SOURCES_ZIP}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${SQLITE3_SOURCES_ZIP}
                                                                sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite3.lib)
            set(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)

            message("Downloading ${SQLITE3_DLL_ZIP}...")

            file(DOWNLOAD https://sqlite.org/2017/${SQLITE3_DLL_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_DLL_ZIP}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${SQLITE3_DLL_ZIP} --format=zip
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
            execute_process(COMMAND lib /verbose /def:sqlite3.def /out:sqlite3.lib /machine:x64
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(SQLITE3_INCLUDE_DIR ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION})
    elseif(APPLE)
        # Use Homebrew's SQLite3

        set(SQLITE3_INCLUDE_DIR /usr/local/opt/sqlite3/include)
        set(SQLITE3_LIBRARY_DIR /usr/local/opt/sqlite3/lib)

        # Link to SQLite3's static version, so that we don't have to bundle it

        set(SQLITE3_IMPORT_LIBRARY libsqlite3.a)
    else()
        set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

        find_package(SQLite3 REQUIRED)

        get_filename_component(SQLITE3_LIBRARY_DIR ${SQLITE3_LIBRARY} DIRECTORY)
        get_filename_component(SQLITE3_IMPORT_LIBRARY ${SQLITE3_LIBRARY} NAME)
    endif()

    if(NOT APPLE)
        # Copy SQLite3's library to the plugin's external binaries directory

        file(MAKE_DIRECTORY ${EXTERNAL_BINARIES_DIR})

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})
        if(WIN32)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        endif()
    endif()

    # The type of build
    # Note: on Windows, we always build a release version...

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
        SOURCE_DIR
            ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/python-cmake-buildsystem.git
        GIT_TAG
            ${GIT_TAG}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            ${CMAKE_INSTALL_RPATH_OPTION}

            # Options for macOS builds

            -DCMAKE_MACOSX_RPATH=1
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}

            # The version of Python to build

            -DPYTHON_VERSION=${LIBRARY_VERSION}

            # Build a shared library

            -DBUILD_LIBPYTHON_SHARED=ON

            # We want development headers

            -DINSTALL_DEVELOPMENT=ON

            # IPython and Jupyter need SQLite3

            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_DIR}
            -DSQLITE3_LIBRARY=${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}

            # Turn off things we don't want

            -DINSTALL_MANUAL=OFF
            -DBUILD_TESTING=OFF
            -DINSTALL_TEST=OFF

            # Optionally patch sources (on Windows)

            -DPYTHON_APPLY_PATCHES=${APPLY_PATCHES}

            # Disable extension modules that we don't use or the platform
            # doesn't provide

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

            # Need to install pip on macOS

            -DENABLE_SCPROXY=ON
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/${LIBRARY_DIR}/${IMPORT_LIBRARY}
        DEPENDS
            OpenSSLPlugin
            zlibPlugin
    )

    if(NOT WIN32)
        ExternalProject_Add_Step(${PACKAGE_BUILD} copyHeaders
                                 COMMAND ${CMAKE_COMMAND} -E copy_directory include/python${PYTHON_BUILT_VERSION} include
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    endif()

    if(WIN32)
        # _freeze_importlib.exe, which is run as part of the Python build, needs
        # our zlib DLL

        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
               ZLIB_DLL "${ZLIB_LIBRARY}")

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyZlib
                                 COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} <BINARY_DIR>/CMakeBuild/libpython
                                 DEPENDEES configure
                                 DEPENDERS build)

        # add_plugin() expects our Python DLL in the same directory as the
        # import libraries and we must use our DLLs when we run Python in
        # subsequent steps below

        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
               OPENSSL_CRYPTO_DLL "${OPENSSL_CRYPTO_LIBRARY}")
        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
               OPENSSL_SSL_DLL "${OPENSSL_SSL_LIBRARY}")

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyDlls
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} Libs
                                 COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_CRYPTO_DLL} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_SSL_DLL} bin
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)

        set(INSTALL_STEP copyDlls)
    elseif(APPLE)
        # Fix up the newly installed Python executable and shared library:
        #  - Add RPATH to the Python executable, so that it can use the already
        #    installed libraries when run at build time;
        #  - Add RPATH to the Python executable, so that it can find the Python
        #    library when run at build time;
        #  - Add RPATH to the Python executable, so that it can use libraries
        #    when installed (into Frameworks/Python/bin);
        #  - Use a relative path to the Python library; and
        #  - set RPATH in the Python library to enable linking independent of
        #    location.

        ExternalProject_Add_Step(${PACKAGE_BUILD} setRpath
                                 COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} bin/python
                                 COMMAND install_name_tool -add_rpath @executable_path/.. bin/python
                                 COMMAND install_name_tool -add_rpath @executable_path/../.. bin/python
                                 COMMAND install_name_tool -change ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY}
                                                                   @rpath/lib/${SHARED_LIBRARY}
                                                                   bin/python
                                 COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} lib/${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)

        set(INSTALL_STEP setRpath)
    else()
        set(INSTALL_STEP install)
    endif()

    # Clean our site packages directory before installing any packages

    ExternalProject_Add_Step(${PACKAGE_BUILD} cleanSitePackagesDirectory
                             COMMAND ${CMAKE_COMMAND} -E remove_directory ${RUNTIME_DIR}/site-packages
                             COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_DIR}/site-packages
                             WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                             DEPENDEES ${INSTALL_STEP})

    # Use our newly built Python

    if(WIN32)
        set(BUILT_PYTHON  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python.exe)
    elseif(APPLE)
        set(BUILT_PYTHON  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    else()
        set(BUILT_PYTHON "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    endif()

    if(WIN32)
        # On Windows, we need to explicitly install setuptools, pip and wheel in
        # order to create *-script-py files that can then be modified during the
        # installation of OpenCOR, so that they reference our newly installed
        # Python executable

        # Install the Python setuptools package

        set(SETUPTOOLS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/setuptools)

        ExternalProject_Add(installSetuptools
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
            DEPENDS
                ${PACKAGE_BUILD}
        )

        # Install the Python package installer

        set(PIP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/pip)

        ExternalProject_Add(installPip
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
            DEPENDS
                installSetuptools
        )

        # Install the wheel Python package

        set(WHEEL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/wheel)

        ExternalProject_Add(installWheel
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
            DEPENDS
                installPip
        )

        set(CREATE_PACKAGE_TARGET installWheel)
    else()
        # On Linux and macOS, install the Python package manager, wheel, and
        # dependencies

        ExternalProject_Add_Step(${PACKAGE_BUILD} installPip
                                 COMMAND ${BUILT_PYTHON} -s ${PROJECT_SOURCE_DIR}/scripts/get-pip.py
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin
                                 DEPENDEES cleanSitePackagesDirectory)

        set(CREATE_PACKAGE_TARGET ${PACKAGE_BUILD})
    endif()

    # Package Python's include files and libraries

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES ${PACKAGED_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${CREATE_PACKAGE_TARGET})

    set(DEPENDS_ON ${CREATE_PACKAGE_TARGET})
endif()

# Allow other external projects to find our copy of Python

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

# We are not relying on add_plugin() to build Python, so we need to specify some
# dependencies

add_custom_target(${PROJECT_NAME})

add_dependencies(OpenCORBuild ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()
