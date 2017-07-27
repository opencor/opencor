PROJECT(PythonPlugin)

# Version of our library

SET(MAJOR_MINOR_LIBRARY_VERSION "3.5")
SET(LIBRARY_VERSION "${MAJOR_MINOR_LIBRARY_VERSION}.2")
STRING(REPLACE "." "" NUMERIC_LIBRARY_VERSION ${MAJOR_MINOR_LIBRARY_VERSION})

# Git tag for our library

SET(GIT_TAG "opencor")

# Name and version of our package

SET(PACKAGE_NAME Python)
SET(PACKAGE_VERSION ${LIBRARY_VERSION})

IF(WIN32)
    # The version of SQLite to bundle

    SET(SQLITE3_VERSION "3170000")
ENDIF()

# Set the ABI flag and version identifier

SET(PYTHON_ABIFLAGS "m")
IF(WIN32)
    IF(NOT RELEASE_MODE)
        MESSAGE(FATAL_ERROR "${CURRENT_PROJECT_NAME} can only be built in release mode...")
    ENDIF()
    SET(PYTHON_BUILT_VERSION ${NUMERIC_LIBRARY_VERSION})
ELSE()
    SET(PYTHON_BUILT_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS})
ENDIF()

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# The root directory of the installed Python

IF(WIN32)
    SET(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/../Python)
ELSEIF(APPLE)
    SET(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/Python)
ELSE()
    SET(ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/..)
ENDIF()

# Include and library directories

IF(WIN32)
    SET(LIBRARY_DIR Libs)
    SET(RUNTIME_DIR lib)
ELSE()
    SET(LIBRARY_DIR lib)
    SET(RUNTIME_DIR lib/python${MAJOR_MINOR_LIBRARY_VERSION})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LIBRARY_DIR})

SET(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    ${LIBRARY_DIR}/${SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${IMPORT_LIBRARY}
    )
ENDIF()

IF(NOT APPLE)
    # We bundle SQLite3 with Python (we link statically when APPLE)

    SET(SQLITE3_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})

    LIST(APPEND EXTERNAL_BINARIES
        ${SQLITE3_SHARED_LIBRARY}
    )

    LIST(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
    )
ENDIF()

# Add headers, scripts and libraries to the package archive

SET(PACKAGED_FILES
    bin
    include
    ${RUNTIME_DIR}
    ${SHA1_FILES}
    )

IF(UNIX)
    LIST(APPEND PACKAGED_FILES ${RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION})
ELSEIF(WIN32)
    LIST(APPEND PACKAGED_FILES Scripts)
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f4d857d42e585223c71bc542653372353767b1fb
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 64d44c4da3bc594cd7e9768c1218bbea1999e462
                                          811688579aeec4bcc9a8b52ee9d60ce72d6e7288
                                          3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 22786346f56097485b8542593599ff90c3005d71
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES d87312719ed58dddbb4e16a3ad9db28776b2a4b5)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 0c8b8b07d6af7cdc8e6746cb472d9be5f45f20f0
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES d755a6bc25f4864a57ade7fa9666f628fd6b24b9
                        d17857a22bd55fe4146169a1f64a9a696b5265ab
        )
    ENDIF()
ELSE()
    # Set platform specific configuration options

    IF(UNIX AND NOT APPLE)
        SET(CMAKE_INSTALL_RPATH_OPTION -DCMAKE_INSTALL_RPATH=$ORIGIN/../lib)
        # Use the system's readline
        SET(PYTHON_READLINE_OPTION -DUSE_SYSTEM_READLINE=ON)
    ELSE()
        SET(CMAKE_INSTALL_RPATH_OPTION)
        # Use Python's built-in readline
        SET(PYTHON_READLINE_OPTION -DBUILTIN_READLINE=ON)
    ENDIF()

    # Jupyter and IPython require SQLite3
    # This could/should be in its own plugin...

    IF(WIN32)
        SET(SQLITE3_DIR ${PROJECT_BINARY_DIR}/sqlite3)

        IF(NOT EXISTS ${SQLITE3_DIR}/sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h)
            SET(SQLITE3_SOURCES_ZIP sqlite-amalgamation-${SQLITE3_VERSION}.zip)
            MESSAGE(STATUS "Downloading ${SQLITE3_SOURCES_ZIP}")
            FILE(DOWNLOAD https://sqlite.org/2017/${SQLITE3_SOURCES_ZIP}
                ${SQLITE3_DIR}/zips/${SQLITE3_SOURCES_ZIP}
                SHOW_PROGRESS
                )
            EXECUTE_PROCESS(
                COMMAND ${CMAKE_COMMAND} -E tar xf zips/${SQLITE3_SOURCES_ZIP}
                                                   sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h
                WORKING_DIRECTORY ${SQLITE3_DIR}
                )
        ENDIF()

        IF(NOT EXISTS ${SQLITE3_DIR}/sqlite3.lib)
            SET(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)
            MESSAGE(STATUS "Downloading ${SQLITE3_DLL_ZIP}")
            FILE(DOWNLOAD https://sqlite.org/2017/${SQLITE3_DLL_ZIP}
                ${SQLITE3_DIR}/zips/${SQLITE3_DLL_ZIP}
                SHOW_PROGRESS
                )
            EXECUTE_PROCESS(
                COMMAND ${CMAKE_COMMAND} -E tar xf zips/${SQLITE3_DLL_ZIP} --format=zip
                WORKING_DIRECTORY ${SQLITE3_DIR}
                )
            EXECUTE_PROCESS(
                COMMAND lib /verbose /def:sqlite3.def /out:sqlite3.lib /machine:x64
                WORKING_DIRECTORY ${SQLITE3_DIR}
                )
        ENDIF()

        SET(SQLITE3_INCLUDE_DIR ${SQLITE3_DIR}/sqlite-amalgamation-${SQLITE3_VERSION})
        SET(SQLITE3_IMPORT_LIBRARY ${SQLITE3_DIR}/sqlite3${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSEIF(APPLE)
        # Use Homebrew's SQLite

        SET(SQLITE3_INCLUDE_DIR /usr/local/opt/sqlite/include)

        # Link to static library so there is no need to bundle it

        SET(SQLITE3_IMPORT_LIBRARY /usr/local/opt/sqlite/lib/libsqlite3.a)
    ELSE()
        SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

        FIND_PACKAGE(SQLite3 REQUIRED)

        SET(SQLITE3_IMPORT_LIBRARY ${SQLITE3_LIBRARY})
    ENDIF()

    IF(NOT APPLE)
        # Copy the SQLite3 library to the plugin's external binaries directory

        GET_FILENAME_COMPONENT(SQLITE3_LIBRARY_PATH ${SQLITE3_IMPORT_LIBRARY} DIRECTORY)

        # First make sure the destination directory exists

        FILE(MAKE_DIRECTORY ${EXTERNAL_BINARIES_DIR})

        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_PATH}/${SQLITE3_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})
        IF(WIN32)
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        ENDIF()
    ENDIF()

    # The type of build

    IF(WIN32)
        SET(BUILD_TYPE Release)
    ELSE()
        SET(BUILD_TYPE ${CMAKE_BUILD_TYPE})
    ENDIF()

    # Build Python as an external project

    SET(PACKAGE_BUILD PythonBuild)

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
            -DSQLITE3_LIBRARY=${SQLITE3_IMPORT_LIBRARY}

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

    IF(NOT WIN32)
        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_headers
            COMMAND ${CMAKE_COMMAND} -E copy_directory include/python${PYTHON_BUILT_VERSION} include
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
            )
    ENDIF()

    IF(WIN32)
        # _freeze_importlib.exe, run as part of the Python build, needs our zlib DLL...

        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            ZLIB_DLL "${ZLIB_LIBRARY}")
        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_zlib
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} <BINARY_DIR>/CMakeBuild/libpython
            DEPENDEES configure
            DEPENDERS build
            )

        # ADD_PLUGIN expects our Python DLL in the same directory as the import libraries
        # and we must use our DLLs when we run Python in subsequent steps below

        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_CRYPTO_DLL "${OPENSSL_CRYPTO_LIBRARY}")
        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_SSL_DLL "${OPENSSL_SSL_LIBRARY}")

        ExternalProject_Add_Step(${PACKAGE_BUILD} copy_dlls
            COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} Libs
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_CRYPTO_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_SSL_DLL} bin
            WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
            DEPENDEES install
            )

        SET(INSTALL_STEP copy_dlls)
    ELSEIF(APPLE)
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

        SET(INSTALL_STEP set_rpath)
    ELSE()
        SET(INSTALL_STEP install)
    ENDIF()

    # Use our newly installed python

    IF(WIN32)
        SET(PYTHON_EXECUTABLE  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python.exe)
    ELSEIF(NOT APPLE)
        SET(PYTHON_EXECUTABLE "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}"  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    ELSE()
        SET(PYTHON_EXECUTABLE  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/bin/python)
    ENDIF()

    # Clean our site-packages directory

    ExternalProject_Add_Step(${PACKAGE_BUILD} clean_site
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${RUNTIME_DIR}/site-packages
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_DIR}/site-packages
        WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        DEPENDEES ${INSTALL_STEP}
        )

    # Install the Python package installer

    ExternalProject_Add_Step(${PACKAGE_BUILD} install_pip
        COMMAND ${PYTHON_EXECUTABLE} -s ${PROJECT_SOURCE_DIR}/scripts/get-pip.py
        DEPENDEES clean_site
        )

    # Package Python's include files and libraries

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
        ${LOCAL_EXTERNAL_PACKAGE_DIR}
        PACKAGED_FILES ${PACKAGED_FILES}
        SHA1_FILES ${SHA1_FILES}
        TARGET ${PACKAGE_BUILD}
        )

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()

# Allow other external projects to find our Python

IF(WIN32)
    SET(PYTHON_EXECUTABLE ${ROOT_DIR}/bin/python.exe PARENT_SCOPE)
ELSE()
    SET(PYTHON_EXECUTABLE ${ROOT_DIR}/bin/python PARENT_SCOPE)
ENDIF()

SET(PYTHON_INCLUDE_DIR ${ROOT_DIR}/include PARENT_SCOPE)
SET(PYTHON_LIBRARY ${ROOT_DIR}/${LIBRARY_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)
SET(PYTHON_ROOT_DIR ${ROOT_DIR} PARENT_SCOPE)
SET(PYTHON_SITE_PACKAGES_DIR ${ROOT_DIR}/${RUNTIME_DIR}/site-packages PARENT_SCOPE)
SET(PYTHON_VERSION ${LIBRARY_VERSION} PARENT_SCOPE)
SET(PYTHON_FULL_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}${PYTHON_ABIFLAGS} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(Python
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
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_DESTINATION_DIR
        ${ROOT_DIR}
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
