project(PythonQtPlugin)

# Our package characteristics

set(PACKAGE_NAME PythonQt)
set(PACKAGE_VERSION 3.2.2)
set(PACKAGE_REPOSITORY pythonqt)
set(GIT_TAG v3.2.2)
set(RELEASE_TAG v3.2.2)

get_full_local_external_package_dir()

# Version of our package's library

set(MAJOR_LIBRARY_VERSION 3)
set(LIBRARY_VERSION ${MAJOR_LIBRARY_VERSION}.2.0)

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

# Specify the files that can be stripped

set(STRIPPABLE_FILES
    lib/${SHARED_LIBRARY}
)

if(WIN32)
    list(APPEND STRIPPABLE_FILES
        lib/${IMPORT_LIBRARY}
    )
endif()

# Use the pre-built version of our package unless instructed otherwise

if(USE_PREBUILT_PYTHONQT_PACKAGE)
    # Retrieve the plugin's package

    if(WIN32)
        if(RELEASE_MODE)
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             54c3aff5546a1f8c998157872e9ae14949b937d1)
        else()
            retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                             ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                             1b460f3ece10121a4a0b023985a87618e703301f)
        endif()
    elseif(APPLE)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         6f736a8a2c680410d441a0ab756bd62e4fd95800)
    else()
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         8d60483fb922986edf0af7fd8cd6b9d5d9166798)
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

    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            ${PYTHON_DEPENDENCIES}
        SOURCE_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/${PACKAGE_REPOSITORY}
        GIT_TAG
            ${GIT_TAG}
        GIT_SHALLOW
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} ${QMAKE_DEBUG_OPTION} ${QMAKE_INCLUDE_PATH} ${QMAKE_LFLAGS_OPTION} ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/PythonQtOpenCOR.pro
        BUILD_COMMAND
            ${MAKE_COMMAND}
        INSTALL_COMMAND
            ${MAKE_COMMAND} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Make sure that we have access to our configured qmake project files

    set(PYTHONQT_INSTALL_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})

    configure_file(${PROJECT_SOURCE_DIR}/qmake/PythonQtOpenCOR.pro.in
                   ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                   @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/qmake/build/python.prf.in
                   ${PROJECT_BINARY_DIR}/build/python.prf
                   @ONLY)

    ExternalProject_Add_Step(${PACKAGE_BUILD} updateConfiguration
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                                         ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/PythonQtOpenCOR.pro
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/build/python.prf
                                         ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/build/python.prf
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/qmake/extensions/PythonQt_QtAll/PythonQt_QtAll.pri
                                         ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/extensions/PythonQt_QtAll
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

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   include ${STRIPPABLE_FILES})

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
