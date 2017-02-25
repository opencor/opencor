PROJECT(PythonQtAPIPlugin)

# Increment this number before building a new package to a value larger
# larger than its maximum value over all git branches. This is to ensure
# that the correct package archive is retrieved when the build configuration
# is changed (e.g. by changing the git branch).

SET(BUILD_NUMBER 1)

# Specify the version of PythonQt we are using.

SET(PYTHONQTAPI_VERSION 3.1)
SET(PYTHONQTAPI_PACKAGE_VERSION ${PYTHONQTAPI_VERSION}-${BUILD_NUMBER})

# Specify the PythonQt repository and branch that we want to use

SET(PYTHONQT_GIT_REPOSITORY https://github.com/dbrnz/PythonQt.git)
SET(PYTHONQT_GIT_TAG "opencor")

#SET(PYTHONQT_GIT_REPOSITORY https://github.com/commontk/PythonQt.git)
#SET(PYTHONQT_GIT_TAG "patched-6")

# The names of the libraries built by PythonQt's build process.

IF(WIN32)
    SET(PYTHONQT_SHARED_LIBRARY PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHONQT_LINK_LIBRARY PythonQt${CMAKE_IMPORT_LIBRARY_SUFFIX})
ELSE()
    SET(PYTHONQT_SHARED_LIBRARY ${CMAKE_SHARED_LIBRARY_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
    SET(PYTHONQT_LINK_LIBRARY ${PYTHONQT_SHARED_LIBRARY})
ENDIF()
SET(EXTERNAL_BINARIES ${PYTHONQT_SHARED_LIBRARY})

# Specify where our PythonQt will be installed.

SET(RELATIVE_INSTALL_DIR external/${REMOTE_EXTERNAL_BINARIES_DIR})
SET(FULL_INSTALL_DIR ${PROJECT_SOURCE_DIR}/${RELATIVE_INSTALL_DIR})

# Specify the files that will have their SHA1 checked when packaged.

SET(CHECKED_FILES lib/${PYTHONQT_SHARED_LIBRARY})

# We need to check the link library for Windows.

IF(WIN32)
    LIST(APPEND CHECKED_FILES lib/${PYTHONQT_LINK_LIBRARY})
ENDIF()

# Add headers and checked files to the package archive

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
                PythonQtAPI ${PYTHONQTAPI_PACKAGE_VERSION} ed02490f37869a8e4ee5ac665c758b091517a32f
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES fb88a31eabe31ff1a0e781c7f20899d4de7103ba
                            0ecda8d0289c9574b4282587b688856f206c5bd6
            )
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            PythonQtAPI ${PYTHONQTAPI_PACKAGE_VERSION} 4b45ab8aee5eb583e7b3e73f344dd511b79b503d
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES b9607e268edf3e5ba8c4c08b172a42196a2fbeb5
        )
    ELSE()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_PROJECT_SOURCE_DIR}
            PythonQtAPI ${PYTHONQTAPI_PACKAGE_VERSION} c1dfbdff0ca52f25f8e40324c46db5d5229d2f8c
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES 81086be94913b2b5b0f5ab7b6bcc2979da7c526d
        )
    ENDIF()
ELSE()
    # Build and install PythonQt as an external package

    INCLUDE(ExternalProject)

    ExternalProject_Add(
        PythonQtAPIBuild
        DEPENDS PythonPlugin
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/PythonQt
        INSTALL_DIR ${FULL_INSTALL_DIR}
        GIT_REPOSITORY ${PYTHONQT_GIT_REPOSITORY}
        GIT_TAG ${PYTHONQT_GIT_TAG}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
            -DCMAKE_MACOSX_RPATH=1
            -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}
            -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
            -DPythonQt_Wrap_QtAll=1
            -DQt5_DIR=1
        BUILD_BYPRODUCTS
            <INSTALL_DIR>/bin/${PYTHONQT_LINK_LIBRARY}
    )

    IF(APPLE)
        # Fix up the newly installed shared library

        ExternalProject_Add_Step(PythonQtAPIBuild set_rpath
            # Set RPATH in library to enable linking independent of location
            COMMAND install_name_tool -id @rpath/${PYTHONQT_SHARED_LIBRARY} ${PYTHONQT_SHARED_LIBRARY}
            WORKING_DIRECTORY ${FULL_INSTALL_DIR}/lib
            DEPENDEES install
            )
    ENDIF()

    # Package PythonQt's include files and libraries

    CREATE_PACKAGE_FILE(${RELATIVE_INSTALL_DIR}
        PythonQtAPI ${PYTHONQTAPI_PACKAGE_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        TARGET PythonQtAPIBuild
        )

    SET(PLUGIN_DEPENDS_ON PythonQtAPIBuild)
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
    DEPENDS
        ${PLUGIN_DEPENDS_ON}
)
