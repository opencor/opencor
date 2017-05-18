PROJECT(PythonQtAPIPlugin)

# Version of our library

SET(LIBRARY_VERSION "3.1")

# Git tag for our library

SET(GIT_TAG "opencor_patched-7")

# Name and version of our package

SET(PACKAGE_NAME PythonQt)
SET(PACKAGE_VERSION 3.1)

# Specify where our local package will be installed

SET(FULL_LOCAL_EXTERNAL_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_PACKAGE_DIR})

# Name of our package's shared libraries (and of their import libraries, if any)

IF(WIN32)
    SET(SHARED_LIBRARY PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(IMPORT_LIBRARY PythonQt${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
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
            RETRIEVE_PACKAGE_FILE(PythonQt ${PACKAGE_VERSION}
                ${RELATIVE_PROJECT_SOURCE_DIR} 70a7fa3a122cf095e601584a167f55df0bbec966
                SHA1_FILES ${SHA1_FILES}
                SHA1_VALUES 819971242aefcbe52bdd43c4031de5f3d7b9fd98
                            0ecda8d0289c9574b4282587b688856f206c5bd6
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(PythonQt ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 70a7fa3a122cf095e601584a167f55df0bbec966
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 819971242aefcbe52bdd43c4031de5f3d7b9fd98
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(PythonQt ${PACKAGE_VERSION}
            ${RELATIVE_PROJECT_SOURCE_DIR} 70a7fa3a122cf095e601584a167f55df0bbec966
            SHA1_FILES ${SHA1_FILES}
            SHA1_VALUES 819971242aefcbe52bdd43c4031de5f3d7b9fd98
        )
    ENDIF()
ELSE()
    # Rely on the ExternalProject module to build our package

    INCLUDE(ExternalProject)

    SET(PACKAGE_BUILD PythonQtBuild)

    ExternalProject_Add(${PACKAGE_BUILD}
        DEPENDS
            PythonLibraryPlugin
        SOURCE_DIR
            ${PROJECT_SOURCE_DIR}/ext/proj
        INSTALL_DIR
            ${FULL_LOCAL_EXTERNAL_PACKAGE_DIR}
        GIT_REPOSITORY
            https://github.com/dbrnz/PythonQt-CommonTk
        GIT_TAG
            ${GIT_TAG}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}
            -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
            -DPythonQt_Wrap_QtAll=1
            -DQt5_DIR=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${IMPORT_LIBRARY}
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
                        PACKAGED_FILES include ${SHA1_FILES}
                        SHA1_FILES ${SHA1_FILES}
                        TARGET ${PACKAGE_BUILD})

    SET(PLUGIN_DEPENDS_ON ${PACKAGE_BUILD})
ENDIF()


# Allow other external projects to find our PythonQt package

SET(PYTHONQT_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)

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
        PythonLibrary
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARIES_DIR}
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    DEPENDS_ON
        ${PLUGIN_DEPENDS_ON}
)
