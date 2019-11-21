project(PythonQtPlugin)

# Name and version of our package

set(PACKAGE_NAME PythonQt)
set(PACKAGE_VERSION 3.2.2)

# Repository in GitHub's OpenCOR organisation

set(PACKAGE_REPOSITORY pythonqt)

# Version of our library

set(MAJOR_LIBRARY_VERSION 3)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.2.0)

# Git tag for our library

set(GIT_TAG v3.2.2)

# Specify where our local package will be installed

string(REPLACE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ext"
       FULL_LOCAL_EXTERNAL_PACKAGE_DIR "${PROJECT_SOURCE_DIR}/${EXTERNAL_PACKAGE_DIR}")

# Name of our package's shared libraries (and of their import libraries, if any)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if(WIN32 OR NOT USE_PREBUILT_PYTHONQT_PACKAGE)
        # If we are not on Windows then we only use the debug library when we
        # are actually building PythonQt for debugging

        set(DEBUG_POSTFIX "_d")
    endif()
endif()

if(WIN32)
    set(SHARED_LIBRARY PythonQt${DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(IMPORT_LIBRARY PythonQt${DEBUG_POSTFIX}${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    if(APPLE)
        set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${DEBUG_POSTFIX}${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    else()
        set(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
    endif()
    set(IMPORT_LIBRARY ${SHARED_LIBRARY})
endif()

# Retrieve our package's binaries

set(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

set(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

set(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHONQT_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 54c3aff5546a1f8c998157872e9ae14949b937d1
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 0b5ef3fdab16ad993ed02897e411c4b4b557199e
                                              8ef254df6fe1b157d3e9da7a928c16887786f602)
        else()
            retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 1b460f3ece10121a4a0b023985a87618e703301f
                                  PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 5ba9235f69f43a5c7db15213d69846f09e888f2d
                                              1c634c8ba4685b29d3c7602f4d5f585313dc2c3c)
        endif()
    elseif(APPLE)
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 6f736a8a2c680410d441a0ab756bd62e4fd95800
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 6b7e81922581ae4a960478a7bbb9f94fa9bd2977)
    else()
        retrieve_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR} 8d60483fb922986edf0af7fd8cd6b9d5d9166798
                              PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES c71017eabf3a8d72092a63ae9bea707167dfcc1c)
    endif()
else()
    # Set platform specific configuration options and tool names

    if(WIN32)
        set(QMAKE_LFLAGS_OPTION)
        set(MAKE_COMMAND nmake)
        set(QUIET_MAKE -c)
    else()
        if(NOT APPLE)
            # Make sure that the correct value of RPATH is set

            set(QMAKE_LFLAGS_OPTION "QMAKE_LFLAGS+=\\'-Wl,-rpath,\\\$\\\$ORIGIN/../lib\\'")
        elseif()
            set(QMAKE_LFLAGS_OPTION)
        endif()

        set(MAKE_COMMAND make)
        set(QUIET_MAKE --quiet)
    endif()

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(QMAKE_DEBUG_OPTION "CONFIG+=debug")
    elseif()
        set(QMAKE_DEBUG_OPTION)
    endif()

    # Wrap our copy of QtWebKit

    set(QMAKE_INCLUDE_PATH "INCLUDEPATH+=${QTWEBKIT_INCLUDE_DIR}")

    # Build PythonQt as an external project

    set(PACKAGE_BUILD ${PACKAGE_NAME}Build)
    set(PACKAGE_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext/${PACKAGE_NAME})

    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            ${PYTHON_DEPENDENCIES}
        DOWNLOAD_DIR
            ${PACKAGE_BUILD_DIR}
        SOURCE_DIR
            ${PACKAGE_BUILD_DIR}
        BINARY_DIR
            ${PACKAGE_BUILD_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} ${QMAKE_DEBUG_OPTION} ${QMAKE_INCLUDE_PATH} ${QMAKE_LFLAGS_OPTION} PythonQtOpenCOR.pro
        BUILD_COMMAND
            ${MAKE_COMMAND}
        INSTALL_COMMAND
            ${MAKE_COMMAND} ${MAKE_QUIET} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Copy our configured qmake project files into the build

    set(PYTHONQT_INSTALL_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

    configure_file(${PROJECT_SOURCE_DIR}/qmake/PythonQtOpenCOR.pro.in
                   ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                   @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/qmake/build/python.prf.in
                   ${PROJECT_BINARY_DIR}/build/python.prf
                   @ONLY)

    ExternalProject_Add_Step(${PACKAGE_BUILD} updateConfiguration
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                                         ${PACKAGE_BUILD_DIR}/PythonQtOpenCOR.pro
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/build/python.prf
                                         ${PACKAGE_BUILD_DIR}/build/python.prf
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/qmake/extensions/PythonQt_QtAll/PythonQt_QtAll.pri
                                         ${PACKAGE_BUILD_DIR}/extensions/PythonQt_QtAll
        DEPENDEES update
        DEPENDERS configure
        )

    # Do some post-processing that is specific to each platform

    if(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        set(FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY})

        if(APPLE)
            set(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${DEBUG_POSTFIX}.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        else()
            set(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
        endif()

        ExternalProject_Add_Step(${PACKAGE_BUILD} copyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${REAL_FULL_SHARED_LIBRARY} ${FULL_SHARED_LIBRARY}
                                 DEPENDEES install)

        if(APPLE)
            # Set RPATH to enable linking independent of location

            ExternalProject_Add_Step(${PACKAGE_BUILD} setRpath
                                     COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} ${FULL_SHARED_LIBRARY}
                                     DEPENDEES copyLibrary)
        endif()

    endif()

    # Package our external project

    create_package_file(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    set(DEPENDS_ON ${PACKAGE_BUILD})
endif()

# Allow other external projects to find our PythonQt package

set(PYTHONQT_ROOT_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)

# Add the plugin

add_plugin(PythonQt
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtplugin.cpp
    PLUGINS
        Python
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${DEPENDS_ON}
)

# Update our list of Python specific dependencies
# Note: we use COPY_EXTERNAL_BINARIES_TARGET rather than PROJECT_NAME to prevent
#       a cyclic dependency...

set(PYTHON_DEPENDENCIES ${PYTHON_DEPENDENCIES} ${COPY_EXTERNAL_BINARIES_TARGET} PARENT_SCOPE)
