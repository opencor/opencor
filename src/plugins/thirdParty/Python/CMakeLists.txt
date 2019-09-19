project(PythonPlugin)

# Name and version of our package

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION 3.7.4)

# Package release tag

set(RELEASE_TAG opencor_0.7)

# Version of our library

set(MAJOR_MINOR_LIBRARY_VERSION "3.7")
set(LIBRARY_VERSION "${MAJOR_MINOR_LIBRARY_VERSION}.4")
set(NUMERIC_LIBRARY_VERSION 37)

# Git tag of our Python sources

set(GIT_TAG "support-build-37-and-38-rebased")

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Set the ABI flag and version identifier

set(PYTHON_ABIFLAGS "m")

if(WIN32)
    set(PYTHON_BUILT_VERSION ${NUMERIC_LIBRARY_VERSION})
    set(PYTHON_ABI_VERSION ${NUMERIC_LIBRARY_VERSION})
else()
    set(PYTHON_BUILT_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS})
    set(PYTHON_ABI_VERSION ${NUMERIC_LIBRARY_VERSION}${PYTHON_ABIFLAGS})
endif()

# Name of our Python executable and shared library (and import library, if any)

if(WIN32)
    set(PYTHON_EXECUTABLE bin/python3.exe)
    set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(PYTHON_EXECUTABLE bin/python3)
    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# The base Python directory and library and script directories

if(WIN32)
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/../Python)
    set(LIBRARY_DIR Libs)
    set(RUNTIME_DIR lib)
    set(SCRIPT_DIR Scripts)
else()
    if (APPLE)
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/Python)
    else()
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/../python)
    endif()
    set(LIBRARY_DIR lib)
    set(RUNTIME_DIR lib/python${MAJOR_MINOR_LIBRARY_VERSION})
    set(SCRIPT_DIR bin)
endif()

# The version of SQLite3 we build for Windows

if(WIN32)
    set(SQLITE3_RELEASE_YEAR "2019")
    set(SQLITE3_VERSION "3290000")
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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 0de2d253e3313881819abf3c8151826bf29e956c
                                  RELEASE_TAG ${RELEASE_TAG}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b1be278f738a9711bd5b40cf30a707af74b90ec2
                                              4bf8513c6d031c6056bbca2286ea2834be96d807
                                              11e62b05b8508cb6785e014cc2f96e8532146a61
                                              3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                                              981012b78a22e0470f2d5acc349ac6745cc986c0)
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
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 88401973b4e0950eb178b3eadb3fca0ac5b199d4
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES f01b93384e9131de7861b3782e5a207794416e50
                                          250befd28ffbb238d85c84273c12a3b5b0ca32c4)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 8c998c6c384a25d2d1a6f299bef56efbf4893628
                              RELEASE_TAG ${RELEASE_TAG}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 2493b3a5ece5968fcd5d3647102d8b5eec8ac907
                                          12ad648b6162b8861436a54b26099b24acbfc98e
                                          561eee6b2f229c953eae711f40b25e834fb1124a)
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
        set(SQLITE3_LIBRARY_DIR ${PROJECT_BINARY_DIR}/sqlite3)

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h)
            set(SQLITE3_SOURCES_ZIP sqlite-amalgamation-${SQLITE3_VERSION}.zip)

            message("Downloading ${SQLITE3_SOURCES_ZIP}...")

            file(DOWNLOAD https://sqlite.org/${SQLITE3_RELEASE_YEAR}/${SQLITE3_SOURCES_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_SOURCES_ZIP}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${SQLITE3_SOURCES_ZIP}
                                                                sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite3.lib)
            set(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)

            message("Downloading ${SQLITE3_DLL_ZIP}...")

            file(DOWNLOAD https://sqlite.org/${SQLITE3_RELEASE_YEAR}/${SQLITE3_DLL_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_DLL_ZIP}
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

        # Link to SQLite3's static library so we don't have to bundle it

        set(SQLITE3_IMPORT_LIBRARY libsqlite3.a)
    else()
        find_package(SQLite3 REQUIRED)

        set(SQLITE3_INCLUDE_DIR ${SQLite3_INCLUDE_DIRS})
        get_filename_component(SQLITE3_LIBRARY_DIR ${SQLite3_LIBRARIES} DIRECTORY)
        get_filename_component(SQLITE3_IMPORT_LIBRARY ${SQLite3_LIBRARIES} NAME)
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

    # We can't use ``;`` as a list separator with ``ExternalProject_Add`` so replace
    # semicolons with ``|`` and use the ``LIST_SEPARATOR`` option

    string(REPLACE ";" "|"
           OPENSSL_LIBRARIES_escaped
           "${OPENSSL_LIBRARIES}"
           )

    # Build Python as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    set(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        SOURCE_DIR
            ${PACKAGE_BUILD_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/python-cmake-buildsystem.git
        GIT_TAG
            ${GIT_TAG}
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        LIST_SEPARATOR
            |
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

            -DUSE_SYSTEM_OpenSSL=OFF
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_LIBRARIES=${OPENSSL_LIBRARIES_escaped}

            # Use our zlib

            -DUSE_SYSTEM_ZLIB=OFF
            -DUSE_BUILTIN_ZLIB=OFF
            -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
            -DZLIB_LIBRARY=${ZLIB_LIBRARY}

            # Needed to install pip on macOS

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
                                 COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY} bin
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
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
                                 COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} ${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -add_rpath @executable_path/.. ${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -add_rpath @executable_path/../.. ${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -change ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY}
                                                                   @rpath/lib/${SHARED_LIBRARY}
                                                                   ${PYTHON_EXECUTABLE}
                                 COMMAND chmod 755 lib/${SHARED_LIBRARY}
                                 COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} lib/${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    endif()

    # Package Python's include files and libraries

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES ${PACKAGED_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our copy of Python

set(PYTHON_ROOT_DIR ${ROOT_DIR} PARENT_SCOPE)

set(PYTHON_INCLUDE_DIR ${ROOT_DIR}/include PARENT_SCOPE)
set(PYTHON_LIBRARY ${ROOT_DIR}/${LIBRARY_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

set(PYTHON_EXECUTABLE ${ROOT_DIR}/${PYTHON_EXECUTABLE} PARENT_SCOPE)

set(PYTHON_RELATIVE_SCRIPT_DIR ${SCRIPT_DIR} PARENT_SCOPE)
set(PYTHON_SCRIPT_DIR  ${ROOT_DIR}/${SCRIPT_DIR} PARENT_SCOPE)

set(PYTHON_RELATIVE_RUNTIME_DIR ${RUNTIME_DIR} PARENT_SCOPE)

set(PYTHON_VERSION ${LIBRARY_VERSION} PARENT_SCOPE)
set(PYTHON_ABI_VERSION ${PYTHON_ABI_VERSION} PARENT_SCOPE)
set(PYTHON_FULL_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS} PARENT_SCOPE)

# Add the plugin

add_plugin(Python
    SOURCES
        ../../plugininfo.cpp

        src/pythonplugin.cpp
    QT_MODULES
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_DESTINATION_DIR
        ${ROOT_DIR}
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    DEPENDS_ON
        ${DEPENDS_ON}
    BYPRODUCTS
        ${ROOT_DIR}/${LIBRARY_DIR}/${IMPORT_LIBRARY}
)

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Keep a list of Python specific dependencies

set(PYTHON_DEPENDENCIES ${PROJECT_NAME} PARENT_SCOPE)

# Copy a script to update the Python path in copied scripts

file(COPY ${PROJECT_SOURCE_DIR}/scripts/set_python_path.py
     DESTINATION ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# And run it

add_custom_command(TARGET ${PROJECT_NAME}
                   COMMAND ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_EXECUTABLE}
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR}/set_python_path.py
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} -s)
