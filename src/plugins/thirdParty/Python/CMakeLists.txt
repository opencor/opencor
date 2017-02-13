PROJECT(PythonPlugin)

# Specify the version of Python we are using.

SET(PYTHON_VERSION 3.5.2)
SET(PYTHON_SHORT_VERSION 3.5)
SET(PYTHON_NUMERIC_VERSION 35)

# The versions of the Python packages we bundle.

SET(NUMPY_VERSION 1.11.2)
SET(SCIPY_VERSION 0.18.1)
SET(MATPLOTLIB_VERSION 1.5.3)

# Set the ABI flag and version identifier

IF(WIN32)
    IF(RELEASE_MODE)
        SET(PYTHON_ABIFLAGS)
    ELSE()
        SET(PYTHON_ABIFLAGS "d")
    ENDIF()
ELSE()
    SET(PYTHON_ABIFLAGS "m")
ENDIF()
SET(PYTHON_BUILT_VERSION ${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS})

# The names of the libraries built by Python's build process.

IF(WIN32)
    SET(PYTHON_LIBRARY python.${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_GENERATED_LIBRARY python.${PYTHON_BUILT_VERSION}.lib)
ELSE()
    SET(PYTHON_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_GENERATED_LIBRARY ${PYTHON_LIBRARY})
ENDIF()

# Specify where our Python will be installed.

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# The Python runtime library directory

SET(PYTHON_LIBRARY_DIR lib/python${PYTHON_SHORT_VERSION})

# Specify the files that will have their SHA1 checked when packaged.

SET(CHECKED_FILES lib/${PYTHON_LIBRARY})

# We need to check the link library for Windows.

IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${PYTHON_GENERATED_LIBRARY})
ENDIF()

# Check the zipped Python runtime library.

LIST(APPEND CHECKED_FILES lib/python${PYTHON_NUMERIC_VERSION}.zip)

# Add headers and... to the package archive

SET(PACKAGED_FILES
    include
    ${PYTHON_LIBRARY_DIR}/config-${PYTHON_BUILT_VERSION}
    ${PYTHON_LIBRARY_DIR}/lib2to3
    ${PYTHON_LIBRARY_DIR}/lib-dynload
    ${PYTHON_LIBRARY_DIR}/site-packages
    ${CHECKED_FILES}
    )

IF(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                Python ${PYTHON_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                    b9895617dc6c321eaaa7d5eef7e4a922f6fb226a
                    a18cbb0332fcef7ce3f3a73fbeb9b463a69632f7
                )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Sample sha...)
        ENDIF()
    ELSEIF(APPLE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            Python ${PYTHON_VERSION} 1573360f0476cfc2cbe6d30e680fd2d4914a65d4
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES b9c0cc945323fbf49fcb879f9d844ce94b9fd584
                        ecded0273f84c712a4b3f0d6503106f07efb44aa
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            Python ${PYTHON_VERSION} 129d11e3f6a7bf4452e2afc748a8b759947b1cf4
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 79919deb65bfe038d17122def0fbbb132e0de2a0
                        54a3aab285b64741df54f8e5fd2d8a2416b70608
        )
    ENDIF()
ELSE()
    # Build and install Python as an external package

    INCLUDE(ExternalProject)

    IF(NOT APPLE)
        SET(CMAKE_INSTALL_RPATH_OPTION -DCMAKE_INSTALL_RPATH=$ORIGIN/../lib)
    ENDIF()

    IF(UNIX AND NOT APPLE)      # Use the system's readline
        SET(PYTHON_READLINE_OPTION -DUSE_SYSTEM_READLINE=ON)
    ELSE()                      # Use Python's built-in readline
        SET(PYTHON_READLINE_OPTION -DBUILTIN_READLINE=ON)
    ENDIF()

    ExternalProject_Add(
        PythonBuild
        DEPENDS OpenSSLPlugin zlibPlugin
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/PythonCMake
        INSTALL_DIR ${FULL_INSTALL_DIR}

        # Uncomment following line and remove next two lines when our branch is merged upstream
        #  GIT_REPOSITORY https://github.com/python-cmake-buildsystem/python-cmake-buildsystem.git
        GIT_REPOSITORY https://github.com/dbrnz/python-cmake-buildsystem.git
        GIT_TAG opencor

        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
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

            # Build with an appropriate readline
            ${PYTHON_READLINE_OPTION}

            # Use our OpenSSL
            -DUSE_SYSTEM_OpenSSL=ON
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}
            -DOPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}

            # Use our zlib
            -DUSE_SYSTEM_ZLIB=ON
            -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
            -DZLIB_LIBRARY=${ZLIB_LIBRARY}

            # OS/X specific; needed to install pip
            -DENABLE_SCPROXY=ON

        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${PYTHON_GENERATED_LIBRARY}
    )

    IF(APPLE)
        # Fix up the newly installed Python executable and shared library

        ExternalProject_Add_Step(PythonBuild set_rpath
            # Add RPATH to executable so it can use installed libraries at build time
            COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} bin/python
            # And also when installed (into Frameworks/Python/bin)
            COMMAND install_name_tool -add_rpath @executable_path/../.. bin/python
            # Set RPATH in library to enable linking independent of location
            COMMAND install_name_tool -id @rpath/${PYTHON_LIBRARY} lib/${PYTHON_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}
            DEPENDEES install
            )

        SET(INSTALL_STEP set_rpath)
    ELSE()
        SET(INSTALL_STEP install)
    ENDIF()

    # Clean our site-packages directory

    ExternalProject_Add_Step(PythonBuild clean_site
        COMMAND ${CMAKE_COMMAND} -E remove_directory site-packages
        COMMAND ${CMAKE_COMMAND} -E make_directory site-packages
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/${PYTHON_LIBRARY_DIR}
        DEPENDEES ${INSTALL_STEP}
        )

    # Install the Python package installer

    ExternalProject_Add_Step(PythonBuild install_pip
        COMMAND "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ./python -s ${PROJECT_SOURCE_DIR}/bin/get-pip.py --prefix ..
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES clean_site
        )

    # Install some base packages

    ExternalProject_Add_Step(PythonBuild install_packages
        COMMAND "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ./python -s ./pip install
            numpy==${NUMPY_VERSION}
            scipy==${SCIPY_VERSION}
            matplotlib==${MATPLOTLIB_VERSION}
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_pip
        )

    # Patch matplotlib backend so that it works with PythonQt

    ExternalProject_Add_Step(PythonBuild patch_matplotlib
        COMMAND "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ./python -s ${PROJECT_SOURCE_DIR}/bin/patch.py
            --strip=2
            --directory=${FULL_INSTALL_DIR}/${PYTHON_LIBRARY_DIR}/site-packages/matplotlib
            ${PROJECT_SOURCE_DIR}/src/matplotlib_${MATPLOTLIB_VERSION}.diff
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_packages
        )

    # Precompile the Python library sources and package as a ZIP file

    ExternalProject_Add_Step(PythonBuild compile_python_lib
        COMMAND ${CMAKE_COMMAND} -E
            remove -f ${FULL_INSTALL_DIR}/lib/library.zip ${FULL_INSTALL_DIR}/lib/python${PYTHON_NUMERIC_VERSION}.zip
        COMMAND "LD_LIBRARY_PATH=${FULL_DEST_EXTERNAL_BINARIES_DIR}" ./python -s ${PROJECT_SOURCE_DIR}/bin/ziplibrary.py
            ${FULL_INSTALL_DIR}/lib/library.zip
            ${FULL_INSTALL_DIR}/lib/python${PYTHON_SHORT_VERSION}
        COMMAND ${CMAKE_COMMAND} -E
            rename ${FULL_INSTALL_DIR}/lib/library.zip ${FULL_INSTALL_DIR}/lib/python${PYTHON_NUMERIC_VERSION}.zip
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES patch_matplotlib
        )

    # Package Python's include files and libraries

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        Python ${PYTHON_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        DEPENDENCY PythonBuild
        )
ENDIF()

# Ensure other plugins can find our Python installation

SET(PYTHON_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)
SET(PYTHON_INCLUDE_DIR ${FULL_INSTALL_DIR}/include/python${PYTHON_BUILT_VERSION} PARENT_SCOPE)
SET(PYTHON_LIBRARY ${FULL_INSTALL_DIR}/lib/${PYTHON_LIBRARY} PARENT_SCOPE)

# Ensure other plugins can our NumPy include directory

SET(NUMPY_INCLUDE_DIR ${FULL_INSTALL_DIR}/${PYTHON_LIBRARY_DIR}/site-packages/numpy/core/include PARENT_SCOPE)

IF(APPLE)
    SET(EXTERNAL_BINARIES ${PYTHON_LIBRARY})
    SET(EXTERNAL_BINARIES_DIR ${FULL_INSTALL_DIR}/lib)
    SET(EXTERNAL_DEST_DIR Python)
ELSE()
    SET(EXTERNAL_BINARIES)
    SET(EXTERNAL_BINARIES_DIR)
    SET(EXTERNAL_DEST_DIR .)
ENDIF()

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
        ${RELATIVE_INSTALL_DIR}/include/python${PYTHON_BUILT_VERSION}
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
)

# We depend on Python being built if we are not using the prebuilt package

IF(NOT USE_PREBUILT_PYTHON_PACKAGE)
    ADD_DEPENDENCIES(PythonPlugin PythonBuild)
ENDIF()
