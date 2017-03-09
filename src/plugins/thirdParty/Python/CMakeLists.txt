PROJECT(PythonPlugin)

# Increment this number before building a new package to a value larger
# larger than its maximum value over all git branches. This is to ensure
# that the correct package archive is retrieved when the build configuration
# is changed (e.g. by changing the git branch).

SET(BUILD_NUMBER 5)

# Specify the version of Python we are using.

SET(PYTHON_VERSION 3.5.2)
SET(PYTHON_SHORT_VERSION 3.5)
SET(PYTHON_NUMERIC_VERSION 35)

# The versions of the Python packages we bundle.

SET(NUMPY_VERSION 1.12.0)
SET(MATPLOTLIB_VERSION 1.5.3)
SET(SCIPY_VERSION 0.18.1)

SET(IPYKERNEL_VERSION 4.5.2)
SET(IPYTHON_VERSION 5.2.2)

# We use our own copies of some Windows packages from
# http://www.lfd.uci.edu/~gohlke/pythonlibs/

SET(WHEEL_REPOSITORY "http://biosignalml.org/binaries/wheels")

# Set the ABI flag and version identifier

SET(PYTHON_ABIFLAGS "m")
IF(WIN32)
    IF(NOT RELEASE_MODE)
        MESSAGE(FATAL_ERROR "${CURRENT_PROJECT_NAME} can only be built in release mode...")
    ENDIF()
    SET(PYTHON_BUILT_VERSION ${PYTHON_NUMERIC_VERSION})
ELSE()
    SET(PYTHON_BUILT_VERSION ${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS})
ENDIF()

SET(PYTHON_PACKAGE_VERSION ${PYTHON_VERSION}-${BUILD_NUMBER})

# The names of the libraries built by Python's build process.

IF(WIN32)
    SET(PYTHON_SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_LINK_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(PYTHON_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_LINK_LIBRARY ${PYTHON_SHARED_LIBRARY})
ENDIF()

# Specify where our Python will be installed.

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Set the include directory and the Python runtime library directory

IF(WIN32)
    SET(PYTHON_INCLUDE_DIR include)
    SET(PYTHON_RUNTIME_DIR lib)
    SET(PYTHON_ZIPPED_LIBRARY lib/python${PYTHON_NUMERIC_VERSION}.zip)
ELSE()
    SET(PYTHON_INCLUDE_DIR include/python${PYTHON_BUILT_VERSION})
    SET(PYTHON_RUNTIME_DIR lib/python${PYTHON_SHORT_VERSION})
    SET(PYTHON_ZIPPED_LIBRARY bin/python${PYTHON_NUMERIC_VERSION}.zip)
ENDIF()

# The plugin's external binaries

SET(EXTERNAL_BINARIES ${PYTHON_SHARED_LIBRARY})

# And where they are

IF(WIN32)
    SET(EXTERNAL_BINARIES_DIR ${FULL_INSTALL_DIR}/Libs)
    SET(EXTERNAL_DEST_DIR Python)
ELSE()
    SET(EXTERNAL_BINARIES_DIR ${FULL_INSTALL_DIR}/lib)
    IF(APPLE)
        SET(EXTERNAL_DEST_DIR Python)
    ELSE()
        SET(EXTERNAL_DEST_DIR .)
    ENDIF()
ENDIF()
SET(FULL_DEST_PYTHON_ROOT_DIR ${FULL_DEST_EXTERNAL_BINARIES_DIR}/${EXTERNAL_DEST_DIR})

# We bundle SQLite3 with Python (we links statically when APPLE)

IF(NOT APPLE)
    SET(SQLITE3_SHARED_LIBRARY sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})
ENDIF()

# Specify the files that will have their SHA1 checked when packaged

IF(WIN32)
    SET(CHECKED_FILES Libs/${PYTHON_SHARED_LIBRARY}
                      Libs/${PYTHON_LINK_LIBRARY}
                      Libs/${SQLITE3_SHARED_LIBRARY}
        )
ELSE()
    SET(CHECKED_FILES lib/${PYTHON_SHARED_LIBRARY})
    IF (NOT APPLE)
        LIST(APPEND CHECKED_FILES lib/${SQLITE3_SHARED_LIBRARY})
    ENDIF()
ENDIF()
LIST(APPEND CHECKED_FILES ${PYTHON_ZIPPED_LIBRARY})

# Add headers, scripts and libraries to the package archive

SET(PACKAGED_FILES
    bin
    include
    ${PYTHON_RUNTIME_DIR}/lib2to3
    ${PYTHON_RUNTIME_DIR}/lib-dynload
    ${PYTHON_RUNTIME_DIR}/site-packages
    ${CHECKED_FILES}
    )

IF(UNIX)
    LIST(APPEND PACKAGED_FILES ${PYTHON_RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION})
ELSEIF(WIN32)
    LIST(APPEND PACKAGED_FILES Scripts)
ENDIF()

IF(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                Python ${PYTHON_PACKAGE_VERSION} 1c833ffa55c9cb0ff589193d67b5f46bc131398e
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES fbb6cc35d424cb52b571dc1e0c8005a578a25db7
                            3027decc9b74d5868aef26025648cdd3f52d7366
                            3f54413d619bdc17271c9dc6b9cf0b2ed5df4e17
                            d551ee5fd523dc61d823e7ef6d2b76ae9934f0c8
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            Python ${PYTHON_PACKAGE_VERSION} e566b66f2fb81897bf9d6f275a418c9cbf40c5eb
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES f50b1663e2028a2a204561b2b6877d421cee279e
                        4ab6eed426536b6c90bb1f01ca96379a8dda67a2
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            Python ${PYTHON_PACKAGE_VERSION} 0c8b8b07d6af7cdc8e6746cb472d9be5f45f20f0
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES d755a6bc25f4864a57ade7fa9666f628fd6b24b9
                        d17857a22bd55fe4146169a1f64a9a696b5265ab
        )
    ENDIF()
ELSE()
    # Build and install Python as an external package

    INCLUDE(ExternalProject)

    IF(UNIX AND NOT APPLE)
        SET(CMAKE_INSTALL_RPATH_OPTION -DCMAKE_INSTALL_RPATH=$ORIGIN/../lib)
        # Use the system's readline
        SET(PYTHON_READLINE_OPTION -DUSE_SYSTEM_READLINE=ON)
    ELSE()
        # Use Python's built-in readline
        SET(PYTHON_READLINE_OPTION -DBUILTIN_READLINE=ON)
    ENDIF()

    # Jupyter and IPython require SQLite3
    # This could/should be in its own plugin...

    IF(WIN32)
        SET(SQLITE3_VERSION "3170000")
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
                COMMAND lib /verbose /def:sqlite3.def /out:sqlite3.lib /machine:x64
                WORKING_DIRECTORY ${SQLITE3_DIR}
                )
        ENDIF()

        SET(SQLITE3_INCLUDE_DIR ${SQLITE3_DIR}/sqlite-amalgamation-${SQLITE3_VERSION})
        SET(SQLITE3_LIBRARY ${SQLITE3_DIR}/sqlite3${CMAKE_IMPORT_LIBRARY_SUFFIX})
    ELSEIF(APPLE)                   # Use Homebrew's SQLite

        SET(SQLITE3_INCLUDE_DIR /usr/local/opt/sqlite/include)
        # Use static library so there is no need to copy it
        SET(SQLITE3_LIBRARY /usr/local/opt/sqlite/lib/libsqlite3.a)
    ELSE()

        SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
        FIND_PACKAGE(SQLite3 REQUIRED)
    ENDIF()

    # Put the SQLite3 library into the python runtime install directory

    IF(NOT APPLE)
        GET_FILENAME_COMPONENT(SQLITE3_LIBRARY_PATH ${SQLITE3_LIBRARY} DIRECTORY)
        EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_PATH}/${SQLITE3_SHARED_LIBRARY}
                                                         ${FULL_INSTALL_DIR}/bin/${SQLITE3_SHARED_LIBRARY})
    ENDIF()

    # What our Python plugin depends upon being built

    IF(WIN32)
        SET(BUILD_TYPE Release)
        SET(BUILD_BYPRODUCTS ${FULL_INSTALL_DIR}/Libs/${PYTHON_LINK_LIBRARY})
    ELSE()
        SET(BUILD_TYPE ${CMAKE_BUILD_TYPE})
        SET(BUILD_BYPRODUCTS ${FULL_INSTALL_DIR}/lib/${PYTHON_LINK_LIBRARY})
    ENDIF()

    # Download, configure, build and install Python

    ExternalProject_Add(PythonBuild
        DEPENDS
            OpenSSLPlugin
            zlibPlugin
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/PythonCMake
        INSTALL_DIR
            ${FULL_INSTALL_DIR}

        # Uncomment next two lines and remove following four when our branch is merged upstream
        # GIT_REPOSITORY
        #     https://github.com/python-cmake-buildsystem/python-cmake-buildsystem.git
        GIT_REPOSITORY
            https://github.com/dbrnz/python-cmake-buildsystem.git
        GIT_TAG
            opencor

        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

            ${CMAKE_INSTALL_RPATH_OPTION}

            # Options for MacOS builds
            -DCMAKE_MACOSX_RPATH=1
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}

            # The version of Python to build
            -DPYTHON_VERSION=${PYTHON_VERSION}

            # Build a shared library
            -DBUILD_LIBPYTHON_SHARED=ON

            # We want development headers
            -DINSTALL_DEVELOPMENT=ON

            # IPython and Jupyter need SQLite
            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_DIR}
            -DSQLITE3_LIBRARY=${SQLITE3_LIBRARY}

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

            # Use our zlib
            -DUSE_SYSTEM_ZLIB=ON
            -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
            -DZLIB_LIBRARY=${ZLIB_LIBRARY}

            # OS/X specific; needed to install pip
            -DENABLE_SCPROXY=ON

        BUILD_BYPRODUCTS
            ${BUILD_BYPRODUCTS}
    )

    IF(WIN32)
        # _freeze_importlib.exe, run as part of the Python build, needs our zlib DLL...

        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            ZLIB_DLL "${ZLIB_LIBRARY}")
        ExternalProject_Add_Step(PythonBuild copy_zlib
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} <BINARY_DIR>/CMakeBuild/libpython
            DEPENDEES configure
            DEPENDERS build
            )

        # ADD_PLUGIN expects our Python DLL in the same directory as the import libraries
        # and we must wse our DLLs when we run Python in subsequent steps below

        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_CRYPTO_DLL "${OPENSSL_CRYPTO_LIBRARY}")
        STRING(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            OPENSSL_SSL_DLL "${OPENSSL_SSL_LIBRARY}")

        ExternalProject_Add_Step(PythonBuild copy_dlls
            COMMAND ${CMAKE_COMMAND} -E copy bin/${PYTHON_SHARED_LIBRARY} Libs
            COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_CRYPTO_DLL} bin
            COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_SSL_DLL} bin
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}
            DEPENDEES install
            )

        SET(INSTALL_STEP copy_dlls)
    ELSEIF(APPLE)
        # Fix up the newly installed Python executable and shared library
        # - add RPATH to executable so it can use installed libraries at build time
        # - and when installed (into Frameworks/Python/bin)
        # - also set RPATH in library to enable linking independent of location

        ExternalProject_Add_Step(PythonBuild set_rpath
            COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} bin/python
            COMMAND install_name_tool -add_rpath @executable_path/../.. bin/python
            COMMAND install_name_tool -id @rpath/${PYTHON_SHARED_LIBRARY} lib/${PYTHON_SHARED_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}
            DEPENDEES install
            )

        SET(INSTALL_STEP set_rpath)
    ELSE()
        SET(INSTALL_STEP install)
    ENDIF()

    # Use our newly installed python

    IF(WIN32)
        SET(PYTHON_EXECUTABLE ${FULL_INSTALL_DIR}/bin/python.exe)
    ELSEIF(NOT APPLE)
        SET(PYTHON_EXECUTABLE "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ${FULL_INSTALL_DIR}/bin/python)
    ELSE()
        SET(PYTHON_EXECUTABLE ${FULL_INSTALL_DIR}/bin/python)
    ENDIF()

    # Clean our site-packages directory

    ExternalProject_Add_Step(PythonBuild clean_site
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${PYTHON_RUNTIME_DIR}/site-packages
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_RUNTIME_DIR}/site-packages
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}
        DEPENDEES ${INSTALL_STEP}
        )

    # Precompile the Python library sources and package as a ZIP file

    ExternalProject_Add_Step(PythonBuild package_python_lib
        COMMAND ${CMAKE_COMMAND} -E
            remove -f ${FULL_INSTALL_DIR}/library.zip ${FULL_INSTALL_DIR}/${PYTHON_ZIPPED_LIBRARY}
        COMMAND ${PYTHON_EXECUTABLE} -s ${PROJECT_SOURCE_DIR}/bin/ziplibrary.py
            ${FULL_INSTALL_DIR}/tmp_library.zip
            ${FULL_INSTALL_DIR}/${PYTHON_RUNTIME_DIR}
        COMMAND ${CMAKE_COMMAND} -E
            rename ${FULL_INSTALL_DIR}/tmp_library.zip ${FULL_INSTALL_DIR}/${PYTHON_ZIPPED_LIBRARY}
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES clean_site
        )

    # Install the Python package installer

    ExternalProject_Add_Step(PythonBuild install_pip
        COMMAND ${PYTHON_EXECUTABLE} -s ${PROJECT_SOURCE_DIR}/bin/get-pip.py
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES package_python_lib
        )

    # Use our newly installed pip

    SET(PIP_EXECUTABLE ${PYTHON_EXECUTABLE} -s -m pip)

    # Install numpy

    IF(WIN32)
        # We use a version that is linked with the Intel Math Kernel Library
        SET(NUMPY_WHEEL "numpy-${NUMPY_VERSION}+mkl-cp${PYTHON_NUMERIC_VERSION}-cp${PYTHON_NUMERIC_VERSION}${PYTHON_ABIFLAGS}-win_amd64.whl")
        ExternalProject_Add_Step(PythonBuild install_numpy
            COMMAND ${PIP_EXECUTABLE} install ${WHEEL_REPOSITORY}/${NUMPY_WHEEL}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
            DEPENDEES install_pip
            )
    ELSE()
        ExternalProject_Add_Step(PythonBuild install_numpy
            COMMAND ${PIP_EXECUTABLE} install --only-binary all
                numpy==${NUMPY_VERSION}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
            DEPENDEES install_pip
            )
    ENDIF()

    # Install matplotlib and patch Qt backend so that it works with PythonQt

    ExternalProject_Add_Step(PythonBuild install_matplotlib
        COMMAND ${PIP_EXECUTABLE} install --only-binary all
            matplotlib==${MATPLOTLIB_VERSION}
        COMMAND ${PYTHON_EXECUTABLE} -s ${PROJECT_SOURCE_DIR}/bin/patch.py
            --strip=2
            --directory=${FULL_INSTALL_DIR}/${PYTHON_RUNTIME_DIR}/site-packages/matplotlib
            ${PROJECT_SOURCE_DIR}/src/matplotlib_${MATPLOTLIB_VERSION}.diff
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_numpy
        )

    # Install scipy

    IF(WIN32)
        SET(SCIPY_WHEEL "scipy-${SCIPY_VERSION}-cp${PYTHON_NUMERIC_VERSION}-cp${PYTHON_NUMERIC_VERSION}${PYTHON_ABIFLAGS}-win_amd64.whl")
        ExternalProject_Add_Step(PythonBuild install_scipy
            COMMAND ${PIP_EXECUTABLE} install ${WHEEL_REPOSITORY}/${SCIPY_WHEEL}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
            DEPENDEES install_matplotlib
            )
    ELSE()
        ExternalProject_Add_Step(PythonBuild install_scipy
            COMMAND ${PIP_EXECUTABLE} install --only-binary all
                scipy==${SCIPY_VERSION}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
            DEPENDEES install_matplotlib
            )
    ENDIF()

    # Install IPython and patch its kernel so that it works with PythonQt

    ExternalProject_Add_Step(PythonBuild install_ipython
        COMMAND ${PIP_EXECUTABLE} install --only-binary all
            ipython==${IPYTHON_VERSION}
            ipykernel==${IPYKERNEL_VERSION}
        COMMAND ${PYTHON_EXECUTABLE} -s ${PROJECT_SOURCE_DIR}/bin/patch.py
            --strip=1
            --directory=${FULL_INSTALL_DIR}/${PYTHON_RUNTIME_DIR}/site-packages/IPython
            ${PROJECT_SOURCE_DIR}/src/ipython_${IPYTHON_VERSION}.diff
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_scipy
        )

    # Package Python's include files and libraries

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        Python ${PYTHON_PACKAGE_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        TARGET PythonBuild
        )

    SET(PLUGIN_DEPENDS_ON PythonBuild)
ENDIF()

# Ensure other plugins can find and use the Python installed into OpenCOR

SET(PYTHON_ROOT_DIR ${FULL_DEST_PYTHON_ROOT_DIR} PARENT_SCOPE)
SET(PYTHON_INCLUDE_DIR ${FULL_DEST_PYTHON_ROOT_DIR}/${PYTHON_INCLUDE_DIR} PARENT_SCOPE)
IF(WIN32)
    SET(PYTHON_EXECUTABLE ${FULL_DEST_PYTHON_ROOT_DIR}/bin/python.exe PARENT_SCOPE)
    SET(PYTHON_LIBRARY ${FULL_DEST_PYTHON_ROOT_DIR}/Libs/${PYTHON_LINK_LIBRARY} PARENT_SCOPE)
ELSE()
    SET(PYTHON_EXECUTABLE ${FULL_DEST_PYTHON_ROOT_DIR}/bin/python PARENT_SCOPE)
    SET(PYTHON_LIBRARY ${FULL_DEST_PYTHON_ROOT_DIR}/lib/${PYTHON_LINK_LIBRARY} PARENT_SCOPE)
ENDIF()

# Ensure other plugins can find our NumPy include directory

SET(NUMPY_INCLUDE_DIR ${FULL_DEST_PYTHON_ROOT_DIR}/${PYTHON_RUNTIME_DIR}/site-packages/numpy/core/include PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(Python
    SOURCES
        ../../cliinterface.cpp
        ../../plugininfo.cpp
        ../../plugininterface.cpp

        src/pythonplugin.cpp
    HEADERS_MOC
        src/pythonplugin.h
    INCLUDE_DIRS
        src
        ${RELATIVE_INSTALL_DIR}/${PYTHON_INCLUDE_DIR}
    PLUGINS
        Core
        OpenSSL
        zlib
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_DEST_DIR
        ${EXTERNAL_DEST_DIR}
    EXTERNAL_SOURCE_DIR
        ${FULL_INSTALL_DIR}
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)

# Copy the SQLite3 shared library to our runtime directory

IF(NOT APPLE)
    COPY_FILE_TO_BUILD_DIR(${PROJECT_NAME}
        ${FULL_INSTALL_DIR}/bin
        ${DEST_EXTERNAL_BINARIES_DIR}
        ${SQLITE3_SHARED_LIBRARY}
    )
ENDIF()
