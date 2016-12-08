PROJECT(PythonPlugin)


# Specify the version of Python we are using.

set(PYTHON_VERSION 3.5.2)
set(PYTHON_SHORT_VERSION 3.5)
set(PYTHON_NUMERIC_VERSION 35)

# The names of the libraries built by Python's build process.

IF(WIN32)
    IF(RELEASE_MODE)
        SET(PYTHON_ABIFLAGS)
    ELSE()
        SET(PYTHON_ABIFLAGS "d")
    ENDIF()
    SET(PYTHON_LIBRARY python.${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_GENERATED_LIBRARY python.${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS}.lib)
ELSEIF(APPLE)
    SET(PYTHON_ABIFLAGS "m")
    SET(PYTHON_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHON_GENERATED_LIBRARY ${PYTHON_LIBRARY})
ELSE()
    SET(PYTHON_ABIFLAGS "m")
    SET(PYTHON_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${CMAKE_SHARED_LIBRARY_SUFFIX}.${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS})
    SET(PYTHON_GENERATED_LIBRARY ${PYTHON_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${PYTHON_LIBRARY})

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

list(APPEND CHECKED_FILES lib/python${PYTHON_NUMERIC_VERSION}.zip)

# Add headers and... to the package archive

set(PACKAGED_FILES
    include
    share  ## So other packages can find use using CMake tools??
    ${PYTHON_LIBRARY_DIR}/config-${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS}
    ${PYTHON_LIBRARY_DIR}/lib2to3
    ${PYTHON_LIBRARY_DIR}/lib-dynload
    ${PYTHON_LIBRARY_DIR}/site-packages
    ${CHECKED_FILES}
    )

if(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                Python ${PYTHON_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                    b9895617dc6c321eaaa7d5eef7e4a922f6fb226a
                    a18cbb0332fcef7ce3f3a73fbeb9b463a69632f7
                )
        else()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Sample sha...)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            Python ${PYTHON_VERSION} c092e866be33664594689071d524112a8e8dcb78
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES fde7a24f5c786cbd5fae9ab49b7cb4a684098af1
                        0a85986027182a62e051820f0f754e5f285d3e40
        )
    else()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Sample sha...)
    endif()
else()
    # Build and install Python as an external package

    INCLUDE(ExternalProject)

    ExternalProject_Add(
        PythonBuild
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/PythonCMake
        INSTALL_DIR ${FULL_INSTALL_DIR}
#        GIT_REPOSITORY https://github.com/python-cmake-buildsystem/python-cmake-buildsystem.git
        GIT_REPOSITORY https://github.com/dbrnz/python-cmake-buildsystem.git
        GIT_TAG abiflags
        UPDATE_COMMAND ""
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
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

            # Use Python's built-in readline
            -DBUILTIN_READLINE=ON

            # Use our OpenSSL
            -DUSE_SYSTEM_OpenSSL=ON
            -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}

            # Use our zlib
            -DUSE_SYSTEM_ZLIB=ON
            -DZLIB_ROOT=${ZLIB_ROOT_DIR}

            # OS/X specific; needed to install pip
            -DENABLE_SCPROXY=ON

            # Ensure @rpath is set for MacOS
            -DCMAKE_MACOSX_RPATH=1

        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${PYTHON_GENERATED_LIBRARY}
    )

    IF(WIN32)
        SET(DEST_EXTERNAL_BINARIES_DIR bin)
    ELSEIF(APPLE)
        SET(DEST_EXTERNAL_BINARIES_DIR ${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)
    ELSE()
        SET(DEST_EXTERNAL_BINARIES_DIR lib)
    ENDIF()
    SET(FULL_DEST_EXTERNAL_BINARIES_DIR ${PROJECT_BUILD_DIR}/${DEST_EXTERNAL_BINARIES_DIR})

    SET(INSTALL_STEP install)

    if(APPLE)
        # Fix up the newly created Python executable and shared library

        ExternalProject_Add_Step(PythonBuild add_rpath
            # Add RPATH to executable so it can use other external libraries
            COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_BINARIES_DIR} ../bin/python
            # Set RPATH in library to enable linking independent of location
            COMMAND install_name_tool -id @rpath/${PYTHON_LIBRARY} ${PYTHON_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
            DEPENDEES install
            )

        SET(INSTALL_STEP add_rpath)
    endif()

    # Install the Python package installer

    ExternalProject_Add_Step(PythonBuild install_pip
        COMMAND ./python ${PROJECT_SOURCE_DIR}/bin/get-pip.py --prefix ..
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES ${INSTALL_STEP}
        )

    # Install some base packages

    ExternalProject_Add_Step(PythonBuild install_packages
        COMMAND ./python ./pip install numpy scipy matplotlib
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_pip
        )

    # Precompile the Python library sources and package as a ZIP file
    ## TODO: This needs a repeat run of the build to work...

    ExternalProject_Add_Step(PythonBuild compile_python_lib
        COMMAND ./python ${PROJECT_SOURCE_DIR}/bin/ziplibrary.py
            ${FULL_INSTALL_DIR}/lib/python${PYTHON_NUMERIC_VERSION}.zip
            ${FULL_INSTALL_DIR}/lib/python${PYTHON_SHORT_VERSION}
        WORKING_DIRECTORY ${FULL_INSTALL_DIR}/bin
        DEPENDEES install_packages
        )

    # Package Python's include files and libraries

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        Python ${PYTHON_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        DEPENDENCY PythonBuild
        )
endif()

# Ensure external packages built by other plugins can find us using CMake

SET(PYTHON_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)
##### CMake uses this when trying to find Python
####set(Python_DIR ${PYTHON_ROOT_DIR} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(Python
    SOURCES
        ../../plugininfo.cpp

        src/pythonplugin.cpp
    HEADERS_MOC
        src/pythonplugin.h
    INCLUDE_DIRS
        src
        ${FULL_INSTALL_DIR}/include/python${PYTHON_SHORT_VERSION}${PYTHON_ABIFLAGS}
    PLUGINS
        Core
    DEFINITIONS
        QT_NO_KEYWORDS
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${FULL_INSTALL_DIR}/lib
    EXTERNAL_DEST_DIR
        Python
    EXTERNAL_SOURCE_DIR
        ${FULL_INSTALL_DIR}
)

# We depend on Python being built if we are not using the prebuilt package

IF(NOT USE_PREBUILT_PYTHON_PACKAGE)
    ADD_DEPENDENCIES(PythonPlugin PythonBuild)
ENDIF()
