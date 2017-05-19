PROJECT(PythonQtAPIPlugin)

# Version of our library

SET(LIBRARY_VERSION "3.1.0")

# Git tag for our library

SET(GIT_TAG "opencor")

# Name and version of our package

SET(PACKAGE_NAME PythonQt)
SET(PACKAGE_VERSION ${LIBRARY_VERSION})

# Package repository and release tag

SET(PACKAGE_REPOSITORY PythonQt)
SET(RELEASE_TAG v3.1.0)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY PythonQt${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt.${LIBRARY_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
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
                ${RELATIVE_PROJECT_SOURCE_DIR} 70a7fa3a122cf095e601584a167f55df0bbec966
                PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                RELEASE_TAG ${RELEASE_TAG}
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 819971242aefcbe52bdd43c4031de5f3d7b9fd98
                            0ecda8d0289c9574b4282587b688856f206c5bd6
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 3ed073926fbb8c8790cd250caeff004199cac353
            PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
            RELEASE_TAG ${RELEASE_TAG}
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES e1876fd6928547a3794a535aba14842c80317c0f
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 70a7fa3a122cf095e601584a167f55df0bbec966
            PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
            RELEASE_TAG ${RELEASE_TAG}
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 819971242aefcbe52bdd43c4031de5f3d7b9fd98
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    # Set platform specific configuration options and tool names

    IF(WIN32)
        SET(QMAKE_PLATFORM_FLAGS "-tp vc")
        SET(MAKE_COMMAND nmake)
        SET(QUIET_MAKE -c)
    ELSE()
        SET(MAKE_COMMAND make)
        SET(QUIET_MAKE --quiet)
    ENDIF()

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        SET(QMAKE_DEBUG_OPTION "CONFIG+=debug")
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
            PythonPlugin
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
            qmake ${QMAKE_DEBUG_OPTION} ${QMAKE_PLATFORM_FLAGS} -r PythonQtOpenCOR.pro
        BUILD_COMMAND
            ${MAKE_COMMAND} all
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

    IF(WIN32)
        # We want both our shared and import libraries to be in the same place

        ExternalProject_Add_Step(${PACKAGE_BUILD} CopyDLL
                                 COMMAND ${CMAKE_COMMAND} -E copy bin/${SHARED_LIBRARY} lib
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    ELSEIF(APPLE)
        # Set RPATH in library to enable linking independent of location

        ExternalProject_Add_Step(${PACKAGE_BUILD} set_rpath
                                 COMMAND install_name_tool -id @rpath/${SHARED_LIBRARY} lib/${SHARED_LIBRARY}
                                 WORKING_DIRECTORY ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
                                 DEPENDEES install)
    ENDIF()

    # Package our external project

    CREATE_PACKAGE_FILE(${PACKAGE_NAME} ${PACKAGE_VERSION}
                        ${LOCAL_EXTERNAL_PACKAGE_DIR}
                        PACKAGE_REPOSITORY ${PACKAGE_REPOSITORY}
                        RELEASE_TAG ${RELEASE_TAG}
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()


# Allow other external projects to find our PythonQt package

SET(PYTHONQT_ROOT_DIR ${LOCAL_EXTERNAL_PACKAGE_DIR} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(PythonQtAPI
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtapiplugin.cpp
    HEADERS_MOC
        src/pythonqtapiplugin.h
    INCLUDE_DIRS
        ${LOCAL_EXTERNAL_PACKAGE_DIR}/include

        src
    PLUGINS
        Core
        Python
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
