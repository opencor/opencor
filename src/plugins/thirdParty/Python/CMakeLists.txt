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

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

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
    set(PYTHON_EXECUTABLE bin/python.exe)
    set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(PYTHON_EXECUTABLE bin/python)
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
    ${PYTHON_EXECUTABLE}
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
    list(APPEND PACKAGED_FILES ${SCRIPT_DIR})
elseif()
    list(APPEND PACKAGED_FILES ${RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION})
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 201c3a0a7621901c7c5da8c121aabff69800ab93
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b45336c90f5a8d39067c8873c46eed73e0596d82
                                              1fb636519bc0d86f181febbac697c842945f645b
                                              41fcf7de51b0150cb5d526696ab4ea6943fff4aa
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              b9d15e7df553a66f8f0a42f27016ff878051196d)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 1651cf41ba9aca0767eecdea908d34217ec653ce
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES eaec9798ba6c5a1bf4b1cc161b6c7dfc59d1ea80
                                              6e79fbfa322c9c56189932c9d6007b9e1f55d3af
                                              c0d0bd4c50ad3f0760b95696d50abe9336ecfe6c
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              41e1905467d98d2f0ddfdb273795dfac9e3402f7)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} e3452e617fa525ee46c42ba3fcbb5bff88b6ba97
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c5dbf4f4ff5355481cec920ab09bcab2257408e0
                                          c1faf41d91676096de6e3d5ac9f2f6cd0177eed0)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} f6c20cf56a3adbe2cc2d4003e90ddf17f3068c83
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 15c502637e33f86db2ad9f8bc7b14b24a5c76844
                                          67b894553bdf5156219d52a9f1656bc1168d23a3
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
        #  - Set RPATH in the Python library to enable linking independent of
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

    # Install the Python package manager and its dependencies

    if(WIN32)
        # On Windows, we use our version of pip that is modified to create
        # *-script.py files when installing packages with console scripts, so
        # that they can then be updated to reference OpenCOR's Python executable

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
                https://github.com/pypa/setuptools.git
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

        # Install our Python package installer

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
                opencor_v${PIP_GIT_TAG}
            CONFIGURE_COMMAND
                ""
            BUILD_COMMAND
                ${BUILT_PYTHON} setup.py build
            INSTALL_COMMAND
                ${BUILT_PYTHON} setup.py install
            DEPENDS
                installSetuptools
        )

        set(CREATE_PACKAGE_TARGET installPip)
    else()
        ExternalProject_Add_Step(${PACKAGE_BUILD} installPip
                                 COMMAND ${BUILT_PYTHON} -s ${PROJECT_SOURCE_DIR}/scripts/get-pip.py --no-wheel
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin
                                 DEPENDEES cleanSitePackagesDirectory)

        set(CREATE_PACKAGE_TARGET installPip)
    endif()

    # Package Python's include files and libraries

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        PACKAGED_FILES ${PACKAGED_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${CREATE_PACKAGE_TARGET})

    set(DEPENDS_ON ${CREATE_PACKAGE_TARGET})
endif()

# Allow other external projects to find our copy of Python

set(PYTHON_ROOT_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)

set(PYTHON_EXECUTABLE ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_EXECUTABLE} PARENT_SCOPE)

set(PYTHON_RELATIVE_SCRIPT_DIR ${SCRIPT_DIR} PARENT_SCOPE)
set(PYTHON_SCRIPT_DIR  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR} PARENT_SCOPE)

set(PYTHON_RELATIVE_RUNTIME_DIR ${RUNTIME_DIR} PARENT_SCOPE)
set(PYTHON_SITE_PACKAGES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${RUNTIME_DIR}/site-packages PARENT_SCOPE)

set(PYTHON_VERSION ${LIBRARY_VERSION} PARENT_SCOPE)
set(PYTHON_FULL_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS} PARENT_SCOPE)

# We are not relying on add_plugin() to build Python, so we need to specify some
# dependencies

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Copy a script to update the Python path in copied scripts

file(COPY ${PROJECT_SOURCE_DIR}/scripts/set_python_path.py
     DESTINATION ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR})

# And run it

add_custom_command(TARGET ${PROJECT_NAME}
                   COMMAND ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_EXECUTABLE}
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR}/set_python_path.py
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} -s)
