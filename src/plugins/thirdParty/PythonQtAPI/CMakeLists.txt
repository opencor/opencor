PROJECT(PythonQtAPIPlugin)

# Specify the version of PythonQt we are using.

SET(PYTHONQTAPI_VERSION 3.1)

# Specify the git tag in the OpenSSL repository that we want to use

SET(PYTHONQT_GIT_TAG "patched-6")

# The names of the libraries built by PythonQt's build process.

IF(WIN32)
    SET(PYTHONQT_LIBRARY PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHONQT_GENERATED_LIBRARY PythonQt.lib)
ELSE()
    SET(PYTHONQT_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHONQT_GENERATED_LIBRARY ${PYTHONQT_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${PYTHONQT_LIBRARY})

# Specify where our PythonQt will be installed.

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged.

SET(CHECKED_FILES lib/${PYTHONQT_LIBRARY})

# We need to check the link library for Windows.

IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${PYTHONQT_GENERATED_LIBRARY})
ENDIF()

# Add headers and... to the package archive

SET(PACKAGED_FILES
    include
    ${CHECKED_FILES}
    )

IF(USE_PREBUILT_PYTHONQT_PACKAGE)
    # Retrieve the plugin's file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${FULL_INSTALL_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
                PythonQtAPI ${PYTHONQTAPI_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                )
        ELSE()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Sample sha...)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            PythonQtAPI ${PYTHONQTAPI_VERSION} a7f51745ea09b8b2daa6b466788317991fd1b04d
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 7addd9fd50a2c41da50288a976d8ab43d9c0da55
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Sample sha...)
    ENDIF()
ELSE()
    # Build and install PythonQt as an external package

    INCLUDE(ExternalProject)

    ExternalProject_Add(
        PythonQtAPIBuild
        DEPENDS PythonPlugin
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/PythonQt
        INSTALL_DIR ${FULL_INSTALL_DIR}
        GIT_REPOSITORY https://github.com/commontk/PythonQt.git
        GIT_TAG ${PYTHONQT_GIT_TAG}
        UPDATE_COMMAND ""
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            -DPYTHON_EXECUTABLE=${FULL_DEST_EXTERNAL_BINARIES_DIR}/Python/bin/python
            -DPythonQt_Wrap_QtAll=1
            -DQt5_DIR=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/lib/${PYTHONQT_GENERATED_LIBRARY}
    )

    IF(APPLE)
        # Fix up the newly installed shared library

        ExternalProject_Add_Step(PythonQtAPIBuild set_rpath
            # Set RPATH in library to enable linking independent of location
            COMMAND install_name_tool -id @rpath/${PYTHONQT_LIBRARY} ${PYTHONQT_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
            DEPENDEES install
            )
    ENDIF()

    # Package PythonQt's include files and libraries

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        PythonQtAPI ${PYTHONQTAPI_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        DEPENDENCY PythonQtAPIBuild
        )
ENDIF()

# Ensure other plugins can find us

SET(PYTHONQT_ROOT_DIR ${FULL_INSTALL_DIR} PARENT_SCOPE)

# Add the plugin

ADD_PLUGIN(PythonQtAPI
    SOURCES
        ../../plugininfo.cpp

        src/pythonqtapiplugin.cpp
    HEADERS_MOC
        src/pythonqtapiplugin.h
    INCLUDE_DIRS
        src
        ${RELATIVE_INSTALL_DIR}/include/PythonQt
    PLUGINS
        Core
        Python
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${FULL_INSTALL_DIR}/lib
)

# We depend on PythonQt being built if we are not using the prebuilt package

IF(NOT USE_PREBUILT_PYTHONQT_PACKAGE)
    ADD_DEPENDENCIES(PythonQtAPIPlugin PythonQtAPIBuild)
ENDIF()
