project(PythonPlugin)

# Our package characteristics

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION 3.12.2)
set(PACKAGE_REPOSITORY python)
set(GIT_TAG opencor-3.12.2)
set(RELEASE_TAG v3.12.2)

get_full_local_external_package_dir()

# Version of our package's library

set(MAJOR_MINOR_LIBRARY_VERSION 3.12)
set(LIBRARY_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}.2)
set(NUMERIC_LIBRARY_VERSION 312)

# Set the version identifiers

if(WIN32)
    set(PYTHON_BUILT_VERSION ${NUMERIC_LIBRARY_VERSION})
else()
    set(PYTHON_BUILT_VERSION ${MAJOR_MINOR_LIBRARY_VERSION})
endif()
set(PYTHON_ABI_VERSION ${NUMERIC_LIBRARY_VERSION})

# Name of our Python executable and shared library (and import library, if any)

if(WIN32)
    set(PYTHON_EXECUTABLE python.exe)

    if(RELEASE_MODE)
        set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
    else()
        set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}_d${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}_d${CMAKE_IMPORT_LIBRARY_SUFFIX})
    endif()
else()
    set(PYTHON_EXECUTABLE python3)

    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# The base Python directory, library and script directories

if(WIN32)
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/../Python)

    set(BINARY_DIR bin)
    set(LIBRARY_DIR Libs)
    set(RUNTIME_DIR lib)
    set(SCRIPTS_DIR Scripts)
else()
    if (APPLE)
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/Python)
    else()
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/../python)
    endif()

    set(BINARY_DIR bin)
    set(LIBRARY_DIR lib)
    set(RUNTIME_DIR lib/python${MAJOR_MINOR_LIBRARY_VERSION})
    set(SCRIPTS_DIR bin)
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LIBRARY_DIR})

# Make sure that our package's binaries directory exists
# Note: indeed, this is needed to prevent issues when building Python on
#       Windows...

if(NOT USE_PREBUILT_PYTHON_PACKAGE)
    file(MAKE_DIRECTORY ${EXTERNAL_BINARIES_DIR})
endif()

# RedHat distributions have their own pre-built Python

if(LINUX_RHEL)
    set(TARGET_PLATFORM ${TARGET_PLATFORM}.rhel)
endif()

# Specify the package's files

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    ${BINARY_DIR}/${PYTHON_EXECUTABLE}
    ${LIBRARY_DIR}/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        ${LIBRARY_DIR}/${IMPORT_LIBRARY}
    )
endif()

# Do a few things that are specific to Windows and Linux

if(NOT APPLE)
    # Build bzip2
    # Note: bzip2 is provided by macOS...

    set(BZIP2_VERSION "1.0.8")

    # Statically link bzip2 on Windows while bundle it on Linux

    if(WIN32)
        set(BZIP2_LIBRARY libbz2${CMAKE_STATIC_LIBRARY_SUFFIX})
    else()
        set(BZIP2_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}bz2${CMAKE_SHARED_LIBRARY_SUFFIX})

        list(APPEND EXTERNAL_BINARIES
            ${BZIP2_SHARED_LIBRARY}
        )

        list(APPEND STRIPPABLE_FILES
            ${LIBRARY_DIR}/${BZIP2_SHARED_LIBRARY}
        )

        set(BZIP2_LIBRARY ${BZIP2_SHARED_LIBRARY}.${BZIP2_VERSION})
    endif()

    # Bundle LZMA with Python
    # Note: it is statically linked on macOS...

    if(WIN32)
        set(LZMA_SHARED_LIBRARY liblzma${CMAKE_SHARED_LIBRARY_SUFFIX})
    else()
        set(LZMA_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}lzma${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()

    list(APPEND EXTERNAL_BINARIES
        ${LZMA_SHARED_LIBRARY}
    )

    list(APPEND STRIPPABLE_FILES
        ${LIBRARY_DIR}/${LZMA_SHARED_LIBRARY}
    )

    # Bundle SQLite3 with Python
    # Note: it is statically linked on macOS...

    set(SQLITE3_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})

    list(APPEND EXTERNAL_BINARIES
        ${SQLITE3_SHARED_LIBRARY}
    )

    list(APPEND STRIPPABLE_FILES
        ${LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
    )

    if(WIN32)
        # On Windows, we use a prebuilt libffi

        set(WINDOWS_LIBFFI_VERSION 3.4.4)
        set(WINDOWS_LIBFFI_LIBRARY libffi-8)

        set(LIBFFI_SHARED_LIBRARY ${WINDOWS_LIBFFI_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX})

        list(APPEND EXTERNAL_BINARIES
            ${LIBFFI_SHARED_LIBRARY}
        )

        list(APPEND STRIPPABLE_FILES
            ${LIBRARY_DIR}/${LIBFFI_SHARED_LIBRARY}
        )

        # On Windows, we also need import libraries

        set(LIBFFI_IMPORT_LIBRARY ${WINDOWS_LIBFFI_LIBRARY}${CMAKE_STATIC_LIBRARY_SUFFIX})
        set(LZMA_IMPORT_LIBRARY liblzma${CMAKE_IMPORT_LIBRARY_SUFFIX})
        set(SQLITE3_IMPORT_LIBRARY ${CMAKE_IMPORT_LIBRARY_PREFIX}sqlite3${CMAKE_IMPORT_LIBRARY_SUFFIX})
    endif()
endif()

# Add headers, scripts and libraries to the package archive

set(PACKAGED_FILES
    bin
    include
    ${RUNTIME_DIR}
    ${STRIPPABLE_FILES}
)

if(WIN32)
    list(APPEND PACKAGED_FILES
        ${SCRIPTS_DIR}
    )
elseif()
    list(APPEND PACKAGED_FILES
        ${RUNTIME_DIR}/config-${PYTHON_BUILT_VERSION}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHON_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             ac9514d781387bb06d16986545e3c14ce06d4b41)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             3291133813343f958c0f527688e7bfb368c5f9ef)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         1853ff1d55e49d57b239b64338a23ffb1101d454)
    elseif(LINUX_RHEL)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         809ca065a37db5f6cc36690136773505a32b753a)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         4770b2a6ae1263ab9c075543b80d4300840ec304)
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

    # Use a prebuilt libffi for Windows

    if(WIN32)
        set(CPYTHON_BIN_DEPS cpython-bin-deps)
        set(WINDOWS_LIBFFI_ARCHIVE libffi-${WINDOWS_LIBFFI_VERSION})

        set(LIBFFI_LIBRARY_DIR ${PROJECT_BINARY_DIR}/${CPYTHON_BIN_DEPS}-${WINDOWS_LIBFFI_ARCHIVE}/amd64)

        if(NOT EXISTS ${LIBFFI_LIBRARY_DIR})

            message("Downloading '${WINDOWS_LIBFFI_ARCHIVE}'...")

            file(DOWNLOAD
                https://github.com/python/${CPYTHON_BIN_DEPS}/archive/${WINDOWS_LIBFFI_ARCHIVE}.zip
                ${PROJECT_BINARY_DIR}/zips/${WINDOWS_LIBFFI_ARCHIVE}.zip
                SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${WINDOWS_LIBFFI_ARCHIVE}.zip --format=zip
                            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(WINDOWS_LIBFFI_OPTIONS -DUSE_SYSTEM_LibFFI=OFF)
        list(APPEND WINDOWS_LIBFFI_OPTIONS -DLibFFI_INCLUDE_DIR=${LIBFFI_LIBRARY_DIR}/include)
        list(APPEND WINDOWS_LIBFFI_OPTIONS -DLibFFI_LIBRARY=${LIBFFI_LIBRARY_DIR}/${LIBFFI_IMPORT_LIBRARY})
    endif()

    # Build bzip2 on Windows and find it on Linux and macOS

    if(WIN32)
        set(BZIP2_LIBRARY_DIR ${PROJECT_BINARY_DIR}/bzip2-opencor-${BZIP2_VERSION})

        if(NOT EXISTS ${BZIP2_LIBRARY_DIR}/bzlib.h)
            set(BZIP2_SOURCE_ARCHIVE bzip2-${BZIP2_VERSION}.tar.gz)

            message("Downloading '${BZIP2_SOURCE_ARCHIVE}'...")

            file(DOWNLOAD https://github.com/opencor/bzip2/archive/opencor-${BZIP2_VERSION}.tar.gz ${PROJECT_BINARY_DIR}/downloads/${BZIP2_SOURCE_ARCHIVE}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xzf downloads/${BZIP2_SOURCE_ARCHIVE}
                            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                            OUTPUT_QUIET)

            message("Building '${BZIP2_LIBRARY}'...")

            execute_process(COMMAND nmake -f makefile.msc lib
                            WORKING_DIRECTORY ${BZIP2_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(BZIP2_INCLUDE_DIR ${BZIP2_LIBRARY_DIR})
        set(BZIP2_LIBRARIES ${BZIP2_LIBRARY_DIR}/${BZIP2_LIBRARY})
    else()
        find_package(BZip2 REQUIRED)

        if(NOT APPLE)
            get_filename_component(BZIP2_LIBRARY_DIR ${BZIP2_LIBRARIES} DIRECTORY)
        endif()
    endif()

    # Build LZMA on Windows and find it on Linux and macOS
    # Note: on Windows, we download XZ since it includes LZMA...

    if(WIN32)
        set(XZ_VERSION 5.2.4)
        set(XZ_PACKAGE_DIR ${PROJECT_BINARY_DIR}/xz-${XZ_VERSION})
        set(LZMA_IMPORT_LIBRARY_DIR ${XZ_PACKAGE_DIR}/doc)
        set(LZMA_LIBRARY_DIR ${XZ_PACKAGE_DIR}/bin_x86-64)

        if(NOT EXISTS ${XZ_PACKAGE_DIR}/include/lzma.h)
            set(XZ_ARCHIVE xz-${XZ_VERSION}-windows.7z)

            message("Downloading '${XZ_ARCHIVE}'...")

            file(DOWNLOAD https://tukaani.org/xz/${XZ_ARCHIVE} ${PROJECT_BINARY_DIR}/downloads/${XZ_ARCHIVE}
                 SHOW_PROGRESS)

            file(MAKE_DIRECTORY ${XZ_PACKAGE_DIR})

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xJf ${PROJECT_BINARY_DIR}/downloads/${XZ_ARCHIVE}
                            WORKING_DIRECTORY ${XZ_PACKAGE_DIR}
                            OUTPUT_QUIET)

            message("Building '${LZMA_IMPORT_LIBRARY}'...")

            execute_process(COMMAND lib /verbose /def:liblzma.def /out:${LZMA_IMPORT_LIBRARY} /machine:x64
                            WORKING_DIRECTORY ${LZMA_IMPORT_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(LIBLZMA_INCLUDE_DIRS ${XZ_PACKAGE_DIR}/include)
        set(LIBLZMA_LIBRARIES ${LZMA_IMPORT_LIBRARY_DIR}/${LZMA_IMPORT_LIBRARY})
    elseif(APPLE)
        # Location of Homebrew's installed packages depends on architecture

        if("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            set(LIBLZMA_INCLUDE_DIRS /usr/local/include)
            set(LIBLZMA_LIBRARIES /usr/local/lib/liblzma.a)
        else()
            set(LIBLZMA_INCLUDE_DIRS /opt/homebrew/opt/xz/include)
            set(LIBLZMA_LIBRARIES /opt/homebrew/opt/xz/lib/liblzma.a)
        endif()
    else()
        find_package(LibLZMA REQUIRED)

        get_filename_component(LZMA_LIBRARY_DIR ${LIBLZMA_LIBRARIES} DIRECTORY)
    endif()

    # Jupyter and IPython require SQLite3

    if(WIN32)
        set(SQLITE3_VERSION "3290000")
        set(SQLITE3_RELEASE_YEAR "2019")
        set(SQLITE3_LIBRARY_DIR ${PROJECT_BINARY_DIR}/sqlite3)

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h)
            set(SQLITE3_SOURCES_ZIP sqlite-amalgamation-${SQLITE3_VERSION}.zip)

            message("Downloading '${SQLITE3_SOURCES_ZIP}'...")

            file(DOWNLOAD https://sqlite.org/${SQLITE3_RELEASE_YEAR}/${SQLITE3_SOURCES_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_SOURCES_ZIP}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${SQLITE3_SOURCES_ZIP}
                                                                sqlite-amalgamation-${SQLITE3_VERSION}/sqlite3.h
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY})
            set(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)

            message("Downloading '${SQLITE3_DLL_ZIP}'...")

            file(DOWNLOAD https://sqlite.org/${SQLITE3_RELEASE_YEAR}/${SQLITE3_DLL_ZIP} ${SQLITE3_LIBRARY_DIR}/zips/${SQLITE3_DLL_ZIP}
                 SHOW_PROGRESS)

            execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf zips/${SQLITE3_DLL_ZIP} --format=zip
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
            execute_process(COMMAND lib /verbose /def:sqlite3.def /out:${SQLITE3_IMPORT_LIBRARY} /machine:x64
                            WORKING_DIRECTORY ${SQLITE3_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(SQLite3_INCLUDE_DIRS ${SQLITE3_LIBRARY_DIR}/sqlite-amalgamation-${SQLITE3_VERSION})
        set(SQLite3_LIBRARIES ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY})
    elseif(APPLE)
        # Location of Homebrew's installed packages depends on architecture

        if("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            set(SQLite3_INCLUDE_DIRS /usr/local/opt/sqlite3/include)
            set(SQLite3_LIBRARIES /usr/local/opt/sqlite3/lib/libsqlite3.a)
        else()
            set(SQLite3_INCLUDE_DIRS /opt/homebrew/opt/sqlite/include)
            set(SQLite3_LIBRARIES /opt/homebrew/opt/sqlite/lib/libsqlite3.a)
        endif()
    else()
        find_package(SQLite3 REQUIRED)

        get_filename_component(SQLITE3_LIBRARY_DIR ${SQLite3_LIBRARIES} DIRECTORY)
    endif()

    if(NOT APPLE)
        # Copy the LZMA and SQLite3 libraries to the plugin's external binaries
        # directory

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_LIBRARY_DIR}/${LZMA_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})

        if(WIN32)
            # On Windows, we need the libFFI shared library

            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${LIBFFI_LIBRARY_DIR}/${LIBFFI_SHARED_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        else()
            # For Linux we need the bzip2 library

            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${BZIP2_LIBRARY_DIR}/${BZIP2_SHARED_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        endif()
    endif()

    # Ignore some warnings

    if(APPLE)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
    elseif(LINUX)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEFAULT_SOURCE")
    endif()

    # Build Python as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)

    string(REPLACE ";" "|"
           PYTHON_OPENSSL_LIBRARIES "${OPENSSL_LIBRARIES}")

    ExternalProject_Add(${PACKAGE_BUILD}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        URL
            https://github.com/opencor/portable-python-cmake-buildsystem/archive/opencor-python.3.12.tar.gz
        CMAKE_GENERATOR
            ${CMAKE_GENERATOR}
        LIST_SEPARATOR
            |
        CMAKE_ARGS
            ${CMAKE_ARGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
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

            # Turn off things we don't want

            -DINSTALL_MANUAL=OFF
            -DBUILD_TESTING=OFF
            -DINSTALL_TEST=OFF

            # We don't want the Window's distutils installer
            # (which in fact doesn't build for Python 3.10)

            -DBUILD_WININST=OFF

            # Patch sources for portable Python CMake build

            -DPYTHON_APPLY_PATCHES=ON

            # Disable extension modules that we don't use or that the platform
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

            # Link to the downloaded libffi for Windows

            ${WINDOWS_LIBFFI_OPTIONS}

            # Build with an appropriate readline

            ${PYTHON_READLINE_OPTION}

            # Link to the bzip2 library

            -DUSE_SYSTEM_BZip2=OFF
            -DBZIP2_INCLUDE_DIR=${BZIP2_INCLUDE_DIR}
            -DBZIP2_LIBRARIES=${BZIP2_LIBRARIES}

            # Link to the LZMA library

            -DLZMA_INCLUDE_PATH=${LIBLZMA_INCLUDE_DIRS}
            -DLZMA_LIBRARY=${LIBLZMA_LIBRARIES}

            # Use our copy of OpenSSL

            -DUSE_SYSTEM_OpenSSL=OFF
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_LIBRARIES=${PYTHON_OPENSSL_LIBRARIES}

            # Jupyter needs SQLite3

            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLite3_INCLUDE_DIRS}
            -DSQLite3_LIBRARY=${SQLite3_LIBRARIES}

            # Use our copy of zlib

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
        # We need to ship the VC runtime with our copy of Python

        set(VCRUNTIME_DLL vcruntime140${DEBUG_TAG}.dll)

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyVCRuntime
                                 COMMAND ${CMAKE_COMMAND} -E copy ${MSVC_DIR}/${VCRUNTIME_DLL} <BINARY_DIR>/${VCRUNTIME_DLL}
                                 DEPENDEES install)

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
                                 COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${SHARED_LIBRARY} Libs
                                 COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_DLL} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_CRYPTO_DLL} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${OPENSSL_SSL_DLL} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${LIBFFI_LIBRARY_DIR}/${LIBFFI_SHARED_LIBRARY} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_LIBRARY_DIR}/${LZMA_SHARED_LIBRARY} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY} bin
                                 COMMAND ${CMAKE_COMMAND} -E copy_directory ${ROOT_DIR}/Scripts Scripts
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
                                 COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_LIBRARIES_DIR} ${BINARY_DIR}/${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -add_rpath @executable_path/.. ${BINARY_DIR}/${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -add_rpath @executable_path/../.. ${BINARY_DIR}/${PYTHON_EXECUTABLE}
                                 COMMAND install_name_tool -change ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LIBRARY_DIR}/${SHARED_LIBRARY}
                                                                   @rpath/${LIBRARY_DIR}/${SHARED_LIBRARY}
                                                                   ${BINARY_DIR}/${PYTHON_EXECUTABLE}
                                 COMMAND chmod 755 ${LIBRARY_DIR}/${SHARED_LIBRARY}
                                 COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} ${LIBRARY_DIR}/${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    endif()

    # Package Python's include files and libraries

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   ${PACKAGED_FILES})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our copy of Python

set(PYTHON_ROOT_DIR ${ROOT_DIR} PARENT_SCOPE)

set(PYTHON_INCLUDE_DIR ${ROOT_DIR}/include PARENT_SCOPE)
set(PYTHON_LIBRARY ${ROOT_DIR}/${LIBRARY_DIR}/${IMPORT_LIBRARY} PARENT_SCOPE)

set(PYTHON_EXECUTABLE ${ROOT_DIR}/${BINARY_DIR}/${PYTHON_EXECUTABLE} PARENT_SCOPE)

set(PYTHON_RELATIVE_SCRIPTS_DIR ${SCRIPTS_DIR} PARENT_SCOPE)
set(PYTHON_SCRIPTS_DIR ${ROOT_DIR}/${SCRIPTS_DIR} PARENT_SCOPE)

set(PYTHON_RELATIVE_RUNTIME_DIR ${RUNTIME_DIR} PARENT_SCOPE)

set(PYTHON_VERSION ${LIBRARY_VERSION} PARENT_SCOPE)

if(WIN32)
    set(PYTHON_SET_PYTHON_PATH_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/set_python_path.py PARENT_SCOPE)
else()
    set(PYTHON_SET_PYTHON_PATH_SCRIPT ${PROJECT_SOURCE_DIR}/scripts/set_generic_python_path.py PARENT_SCOPE)
endif()

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
    EXTERNAL_SOURCE_DIR
        ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
    EXTERNAL_DESTINATION_DIR
        ${ROOT_DIR}
    DEPENDS_ON
        ${DEPENDS_ON}
    BYPRODUCTS
        ${ROOT_DIR}/${LIBRARY_DIR}/${IMPORT_LIBRARY}
)

add_dependencies(${PROJECT_BUILD_TARGET} ${PROJECT_NAME})

if(NOT "${DEPENDS_ON}" STREQUAL "")
    add_dependencies(${PROJECT_NAME} ${DEPENDS_ON})
endif()

# Copy the Python script that updates the Python path in our Python scripts
# Note: it's only needed on Windows since on Linux and macOS, we can use a
#       relative Python path thanks to set_generic_python_path.py...

if(WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/scripts/set_python_path.py
         DESTINATION ${ROOT_DIR}/${SCRIPTS_DIR}
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()

# Keep a list of Python specific dependencies

set(PYTHON_DEPENDENCIES ${PROJECT_NAME} PARENT_SCOPE)
