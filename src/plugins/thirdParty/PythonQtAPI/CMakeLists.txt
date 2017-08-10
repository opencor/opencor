PROJECT(PythonQtAPIPlugin)

# Name and version of our package

SET(PACKAGE_NAME PythonQt)
SET(PACKAGE_VERSION "3.2.0")

# Version of our library

SET(MAJOR_LIBRARY_VERSION "3")
SET(LIBRARY_VERSION "${MAJOR_LIBRARY_VERSION}.2.0")

# Git tag for our library

SET(GIT_TAG "opencor")

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY PythonQt${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY PythonQt${MAJOR_LIBRARY_VERSION}${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    IF(APPLE)
        SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${MAJOR_LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
    ELSE()
        SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX}.${MAJOR_LIBRARY_VERSION})
    ENDIF()
    SET(IMPORT_LIBRARY ${SHARED_LIBRARY})
ENDIF()

# Retrieve our package's binaries

SET(EXTERNAL_BINARIES_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib)

SET(EXTERNAL_BINARIES
    ${SHARED_LIBRARY}
)

# Specify the files that need to have their SHA-1 value checked

SET(SHA1_FILES
    lib/${SHARED_LIBRARY}
)

IF(WIN32)
    LIST(APPEND SHA1_FILES
        lib/${IMPORT_LIBRARY}
    )
ENDIF()

# Use the pre-built version of our package unless instructed otherwise

IF(USE_PREBUILT_PYTHONQT_PACKAGE)
    # Retrieve the plugin's package

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_PACKAGE_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                  ${RELATIVE_PROJECT_SOURCE_DIR} b113f8faccd45529684960c489a239ae5d900e5a
                                  SHA1_FILES ${SHA1_FILES}
                                  SHA1_VALUES 79e213a4133960ad40c2016f3f0fb3ff5f3038c7
                                              24da18bba076d5718f7676f2c066b96cd487bae3)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} f775ed78016d1037eebd42c90be77a99b36535eb
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES 841689afb345946efa422ee787c97e672e6ffdcb)
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                              ${RELATIVE_PROJECT_SOURCE_DIR} 47870c345ae46a1e28e86edd585300de2fba927d
                              SHA1_FILES ${SHA1_FILES}
                              SHA1_VALUES e2ddd1bc147a0bb8f6c56e000f6a53695d45b403)
    ENDIF()
ELSE()
    # Set platform specific configuration options and tool names

    IF(WIN32)
        SET(QMAKE_LFLAGS_OPTION)
        SET(MAKE_COMMAND nmake)
        SET(QUIET_MAKE -c)
    ELSE()
        IF(NOT APPLE)
          # Make sure correct value of RPATH is set in library

            SET(QMAKE_LFLAGS_OPTION "QMAKE_LFLAGS+=\\'-Wl,-rpath,\\\$\\\$ORIGIN/../lib\\'")
        ELSEIF()
            SET(QMAKE_LFLAGS_OPTION)
        ENDIF()

        SET(MAKE_COMMAND make)
        SET(QUIET_MAKE --quiet)
    ENDIF()

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        SET(QMAKE_DEBUG_OPTION "CONFIG+=debug")
    ELSEIF()
        SET(QMAKE_DEBUG_OPTION)
    ENDIF()

    # Configure qmake project files with our settings

    SET(PYTHONQT_INSTALL_DIR ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR})
    SET(PYTHONQT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/proj)

    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/qmake/PythonQtOpenCOR.pro.in
                   ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                   @ONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/qmake/build/common.prf.in
                   ${PROJECT_BINARY_DIR}/build/common.prf
                   @ONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/qmake/build/python.prf.in
                   ${PROJECT_BINARY_DIR}/build/python.prf
                   @ONLY)

    # Build PythonQt as an external project

    SET(PACKAGE_BUILD PythonQtBuild)

    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            ${PYTHON_HEADERS_DEPENDENCIES}
        DOWNLOAD_DIR
            ${PYTHONQT_SOURCE_DIR}
        SOURCE_DIR
            ${PYTHONQT_SOURCE_DIR}
        BINARY_DIR
            ${PYTHONQT_SOURCE_DIR}
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/opencor/PythonQt
        GIT_TAG
            ${GIT_TAG}
        CONFIGURE_COMMAND
            ${QMAKE_COMMAND} ${QMAKE_DEBUG_OPTION} ${QMAKE_LFLAGS_OPTION} -r PythonQtOpenCOR.pro
        BUILD_COMMAND
            ${MAKE_COMMAND}
        INSTALL_COMMAND
            ${MAKE_COMMAND} ${MAKE_QUIET} install
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
    )

    # Copy our configured qmake project files into the build

    ExternalProject_Add_Step(${PACKAGE_BUILD} update_configuration
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/PythonQtOpenCOR.pro
                                         ${PYTHONQT_SOURCE_DIR}/PythonQtOpenCOR.pro
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/build/common.prf
                                         ${PYTHONQT_SOURCE_DIR}/build/common.prf
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/build/python.prf
                                         ${PYTHONQT_SOURCE_DIR}/build/python.prf
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/qmake/extensions/PythonQt_QtAll/PythonQt_QtAll.pri
                                         ${PYTHONQT_SOURCE_DIR}/extensions/PythonQt_QtAll
        DEPENDEES update
        DEPENDERS configure
        )

    # Do some post-processing that is specific to each platform

    IF(NOT WIN32)
        # Dereference the symbolic link to the library since CMake's tar can't
        # do this

        SET(FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${SHARED_LIBRARY})

        IF(APPLE)
            SET(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
        ELSE()
            SET(REAL_FULL_SHARED_LIBRARY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBRARY_VERSION})
        ENDIF()

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyLibrary
                                 COMMAND ${CMAKE_COMMAND} -E remove -f ${FULL_SHARED_LIBRARY}
                                 COMMAND ${CMAKE_COMMAND} -E copy ${REAL_FULL_SHARED_LIBRARY} ${FULL_SHARED_LIBRARY}
                                 DEPENDEES install)

        IF(APPLE)
            # Set RPATH in library to enable linking independent of location

            ExternalProject_Add_Step(${PACKAGE_BUILD} set_rpath
                                     COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} ${FULL_SHARED_LIBRARY}
                                     DEPENDEES CopyLibrary)
        ENDIF()

    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()


# Allow other external projects to find our PythonQt package

SET(PYTHONQT_ROOT_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)

SET(PYTHON_HEADERS_DEPENDENCIES ${PYTHON_HEADERS_DEPENDENCIES} ${PLUGIN_DEPENDS_ON} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(PythonQtAPI
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtapiplugin.cpp
    HEADERS_MOC
        src/pythonqtapiplugin.h
    PLUGINS
        Core
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
