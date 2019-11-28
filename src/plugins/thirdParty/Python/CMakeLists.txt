project(PythonPlugin)

# Name and version of our package

set(PACKAGE_NAME Python)
set(PACKAGE_VERSION 3.7.5)

# Package repository and release tag

set(PACKAGE_REPOSITORY python)

# Version of our library

set(MAJOR_MINOR_LIBRARY_VERSION "3.7")
set(LIBRARY_VERSION "${MAJOR_MINOR_LIBRARY_VERSION}.5")
set(NUMERIC_LIBRARY_VERSION 37)

# Git tag of our Python sources

set(GIT_TAG "opencor")

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

    # Bundle liblzma with Python
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
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} c42fd1f8b7b6b353050f26c7f7bf4c8837feb94e
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES b91fa989e51c4fd707cd47a2892352dccffb4547
                                              83015dd7093c524670eed86ea1c2001ab09068df
                                              599a6df061280855fad67d7c775c242345b0bb2d
                                              3970ffe8aefe0c30daaec65b85fb103c0fc0f2a7
                                              62c54e5ddfeb5992696b973f88915958d3eb0c49)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 114569320cbde211238ac64030b558057869544e
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 64adb27b2f362b14174eb6e661badce125214f6d
                                              5d1eea99cf91fd272b0cd952a5d0c0bdc967e287
                                              7314ef1eb91964755ebc5f3d3544935b2b5cb1c0
                                              3970ffe8aefe0c30daaec65b85fb103c0fc0f2a7
                                              eaa161d1c56423bd41b37258da196946d0f0c4e4)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 85f8ea811ed7b3912486837e164e12318ed375d1
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 527efe36f3f59beb888f988e6039234f50dc87a2
                                          40af4e4c511a9a3a4b6a47446505c1fb60881d59)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 8a32d24a1275a1bbaf2cef7811f6cd5a21b370bf
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 06aec6cd236cefb7206f1f6f43fa6220e398adff
                                          dad9acba5c825edab5af6e825ae1715d0842e825
                                          57534423f92eb3a881b8dc781efb6857cc257874
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

    if(APPLE)
        # macOS provides bzip2

        find_package(BZip2 REQUIRED)
    else()
        # Build bzip2 for Windows and Linux

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

            if(WIN32)
                execute_process(COMMAND nmake -f makefile.msc lib
                                WORKING_DIRECTORY ${BZIP2_LIBRARY_DIR}
                                OUTPUT_QUIET)
            else()
                execute_process(COMMAND make -f Makefile-libbz2_so
                                WORKING_DIRECTORY ${BZIP2_LIBRARY_DIR}
                                OUTPUT_QUIET)

                # Rename bzip2's library and copy it to the plugin's external
                # binaries directory

                file(RENAME ${BZIP2_LIBRARY_DIR}/${BZIP2_LIBRARY}
                            ${BZIP2_LIBRARY_DIR}/${BZIP2_SHARED_LIBRARY})

                file(COPY ${BZIP2_LIBRARY_DIR}/${BZIP2_SHARED_LIBRARY}
                     DESTINATION ${EXTERNAL_BINARIES_DIR}
                     FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
            endif()
        endif()

        set(BZIP2_INCLUDE_DIR ${BZIP2_LIBRARY_DIR})

        if(WIN32)
            set(BZIP2_LIBRARIES ${BZIP2_LIBRARY_DIR}/${BZIP2_LIBRARY})
        else()
            set(BZIP2_LIBRARIES ${EXTERNAL_BINARIES_DIR}/${BZIP2_SHARED_LIBRARY})
        endif()
    endif()

    # Find or build the lzma library

    if(WIN32)
        # Download `xz` for Windows, which includes `lzma`

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

            message("Building LZMA import library...")

            execute_process(COMMAND lib /verbose /def:liblzma.def /out:${LZMA_IMPORT_LIBRARY} /machine:x64
                            WORKING_DIRECTORY ${LZMA_IMPORT_LIBRARY_DIR}
                            OUTPUT_QUIET)
        endif()

        set(LIBLZMA_INCLUDE_DIRS ${XZ_PACKAGE_DIR}/include)
        set(LIBLZMA_LIBRARIES ${LZMA_IMPORT_LIBRARY_DIR}/${LZMA_IMPORT_LIBRARY})

    elseif(APPLE)
        # Use Homebrew's LZMA and link with static library so we don't have to bundle it

        set(LIBLZMA_INCLUDE_DIRS /usr/local/include)
        set(LIBLZMA_LIBRARIES /usr/local/lib/liblzma.a)
    else()
        # Assume we have installed the `liblzma-dev` package

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

        if(NOT EXISTS ${SQLITE3_LIBRARY_DIR}/sqlite3.lib)
            set(SQLITE3_DLL_ZIP sqlite-dll-win64-x64-${SQLITE3_VERSION}.zip)

            message("Downloading '${SQLITE3_DLL_ZIP}'...")

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
        # Copy shared libraries to the plugin's external binaries directory

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_LIBRARY_DIR}/${LZMA_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})

        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_SHARED_LIBRARY}
                                                         ${EXTERNAL_BINARIES_DIR})

        if(WIN32)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${LZMA_IMPORT_LIBRARY_DIR}/${LZMA_IMPORT_LIBRARY}
                                                             ${EXTERNAL_BINARIES_DIR})

            execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}
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

            # Jupyter needs SQLite3

            -DENABLE_SQLITE3=ON
            -DSQLITE3_INCLUDE_PATH=${SQLITE3_INCLUDE_DIR}
            -DSQLITE3_LIBRARY=${SQLITE3_LIBRARY_DIR}/${SQLITE3_IMPORT_LIBRARY}

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

            # Use our OpenSSL

            -DUSE_SYSTEM_OpenSSL=OFF
            -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
            -DOPENSSL_LIBRARIES=${PYTHON_OPENSSL_LIBRARIES}

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
