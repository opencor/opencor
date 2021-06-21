project(PythonPlugin)

# Name and version of our package

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION 3.7.5)

# Git tag for our library

set(GIT_TAG opencor-3.7.5)

# Package repository

set(PACKAGE_REPOSITORY python)

# Version of our library

set(MAJOR_MINOR_LIBRARY_VERSION 3.7)
set(LIBRARY_VERSION ${MAJOR_MINOR_LIBRARY_VERSION}.5)
set(NUMERIC_LIBRARY_VERSION 37)

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
    set(PYTHON_EXECUTABLE bin/python.exe)

    if(RELEASE_MODE)
        set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
    else()
        set(SHARED_LIBRARY python${PYTHON_BUILT_VERSION}_d${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(IMPORT_LIBRARY python${PYTHON_BUILT_VERSION}_d${CMAKE_IMPORT_LIBRARY_SUFFIX})
    endif()
else()
    set(PYTHON_EXECUTABLE bin/python3)

    set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}python${PYTHON_BUILT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# The base Python directory, library and script directories

if(WIN32)
    set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/../Python)

    set(LIBRARY_DIR Libs)
    set(RUNTIME_DIR lib)
    set(SCRIPT_DIR Scripts)
else()
    if (APPLE)
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/Python)
    else()
        set(ROOT_DIR ${FULL_DEST_EXTERNAL_LIBRARIES_DIR}/../python)
    endif()

    set(LIBRARY_DIR lib)
    set(RUNTIME_DIR lib/python${MAJOR_MINOR_LIBRARY_VERSION})
    set(SCRIPT_DIR bin)
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${LIBRARY_DIR})

# Make sure that our package's binaries directory exists
# Note: indeed, this is needed to prevent issues when building Python on
#       Windows...

file(MAKE_DIRECTORY ${EXTERNAL_BINARIES_DIR})

# Specify the package's files

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

        list(APPEND SHA1_FILES
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

    list(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${LZMA_SHARED_LIBRARY}
    )

    # Bundle SQLite3 with Python
    # Note: it is statically linked on macOS...

    set(SQLITE3_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}sqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})

    list(APPEND EXTERNAL_BINARIES
        ${SQLITE3_SHARED_LIBRARY}
    )

    list(APPEND SHA1_FILES
        ${LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
    )

    if(WIN32)
        # On Windows, we also need import libraries

        set(LZMA_IMPORT_LIBRARY liblzma${CMAKE_IMPORT_LIBRARY_SUFFIX})

        list(APPEND EXTERNAL_BINARIES
            ${LZMA_IMPORT_LIBRARY}
        )

        list(APPEND SHA1_FILES
            ${LIBRARY_DIR}/${LZMA_IMPORT_LIBRARY}
        )

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
    list(APPEND PACKAGED_FILES
        ${SCRIPT_DIR}
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
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 10d28302ba9a75544c0c65704ab9dd192ba6fb8d
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 7b48002df4a74dee491edc654295769f6a3965c6
                                              51e8b112ec4429be34a6585b810768ceaa33150b
                                              40357926b13a9c18b849c4ce875579ea5f9ee5e7
                                              0b14ddd87cbe6a7c186ee55737eca4cee363dc47
                                              3970ffe8aefe0c30daaec65b85fb103c0fc0f2a7
                                              aed440552b24a5971891533938feff48a74bc2e0
                                              f0f8aac6a1ebd6111129bdc3287cf6834a790f06)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 31cedabe85a1f28e435f64bc5842e52b1055b3d6
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES ae8ace1ba76098ed84fbb1aa10d9a07ac17a9379
                                              352113e80e2136ef0374c437c7cc4be5c0987403
                                              323c7afb3139adb71c098079dfc029ac2ea698d1
                                              0b14ddd87cbe6a7c186ee55737eca4cee363dc47
                                              3970ffe8aefe0c30daaec65b85fb103c0fc0f2a7
                                              d7074d2f2662088a6eef3b8438321bb1236a75d8
                                              16e6b4d74e222f1f3339fa655d8bd90741135c2f)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 76a9567f0e9b6ab0284047dda301add5704b04a9
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES abb51c13f494869383d6fb5f4546e29da328c926
                                          0984ed1d0834eb67b3a2f2b39589663dd020d19a)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 0982331f1719017596c28e010cc5b46825346d99
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 50b47e10df44209b3e78f145350a2e1e35b960f7
                                          920daa60a973fbc24cc05f7ad12ee90dd9e69a56
                                          ee42ca256c6feb7412c37313e1935d2e31a0a1de
                                          d850f87b53d12db9434476fa84977e69c8ceb243
                                          38161dee3a32f4b3a70ffa997aa42c9c8a8b2aa3)
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
        set(LIBLZMA_INCLUDE_DIRS /usr/local/include)
        set(LIBLZMA_LIBRARIES /usr/local/lib/liblzma.a)
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
        set(SQLite3_INCLUDE_DIRS /usr/local/opt/sqlite3/include)
        set(SQLite3_LIBRARIES /usr/local/opt/sqlite3/lib/libsqlite3.a)
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

        # On Windows, we also need the LZMA and SQLite3 import libraries while
        # the bzip2 library on Linux

        if(WIN32)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_IMPORT_LIBRARY_DIR}/${LZMA_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        else()
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${BZIP2_LIBRARY_DIR}/${BZIP2_SHARED_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})
        endif()
    endif()

    # The type of build
    # Note: on Windows, we always build a release version...

    if(WIN32)
        set(APPLY_PATCHES ON)
    else()
        set(APPLY_PATCHES OFF)
    endif()

    # Ignore some warnings

    if(APPLE)
        set(PACKAGE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
    endif()

    # Build Python as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    set(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    string(REPLACE ";" "|"
           PYTHON_OPENSSL_LIBRARIES "${OPENSSL_LIBRARIES}")

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
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_FLAGS:STRING=${PACKAGE_C_FLAGS}
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

            # Turn off things we don't want

            -DINSTALL_MANUAL=OFF
            -DBUILD_TESTING=OFF
            -DINSTALL_TEST=OFF

            # Optionally patch sources (on Windows)

            -DPYTHON_APPLY_PATCHES=${APPLY_PATCHES}

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

            # Build with an appropriate readline

            ${PYTHON_READLINE_OPTION}

            # Link to the bzip2 library

            -DUSE_SYSTEM_BZip2=OFF
            -DBZIP2_INCLUDE_DIR=${BZIP2_INCLUDE_DIR}
            -DBZIP2_LIBRARIES=${BZIP2_LIBRARIES}

            # Link to the LZMA library

            -DUSE_SYSTEM_LZMA=OFF
            -DLIBLZMA_INCLUDE_DIRS=${LIBLZMA_INCLUDE_DIRS}
            -DLIBLZMA_LIBRARIES=${LIBLZMA_LIBRARIES}

            # Use our copy of OpenSSL

            -DUSE_SYSTEM_OpenSSL=OFF
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_LIBRARIES=${PYTHON_OPENSSL_LIBRARIES}

            # Jupyter needs SQLite3

            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLite3_INCLUDE_DIRS}
            -DSQLITE3_LIBRARY=${SQLite3_LIBRARIES}

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
                                 COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_LIBRARY_DIR}/${LZMA_SHARED_LIBRARY} bin
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
                                 COMMAND install_name_tool -add_rpath ${FULL_DEST_EXTERNAL_LIBRARIES_DIR} ${PYTHON_EXECUTABLE}
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
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
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
set(PYTHON_SCRIPT_DIR ${ROOT_DIR}/${SCRIPT_DIR} PARENT_SCOPE)

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

# Copy a script to update the Python path in copied scripts

file(COPY ${PROJECT_SOURCE_DIR}/scripts/set_python_path.py
     DESTINATION ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# And run it

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${PYTHON_EXECUTABLE}
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/${SCRIPT_DIR}/set_python_path.py
                           ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} -s)

# Keep a list of Python specific dependencies

set(PYTHON_DEPENDENCIES ${PROJECT_NAME} PARENT_SCOPE)
