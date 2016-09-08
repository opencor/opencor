PROJECT(PythonPlugin)

## Overide global setting for development
##set(USE_PREBUILT_PYTHON_PACKAGE OFF)

# The version of Python we want to build

set(PYTHON_VERSION 3.5.2)
set(PYTHON_SHORT_VERSION 3.5)
set(PYTHON_NUMERIC_VERSION 35)

# Reset Python variables in FIND_PACKAGE() cache...

unset(PYTHON_FOUND CACHE)
unset(PYTHON_INCLUDE_DIR CACHE)
unset(PYTHON_LIBRARY CACHE)
unset(PYTHON_ROOT_DIR CACHE)
unset(PYTHON_VERSION CACHE)

set(PYTHON_ROOT_DIR "${PROJECT_SOURCE_DIR}/packages/${REMOTE_EXTERNAL_BINARIES_DIR}")
string(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" RELATIVE_ROOT_DIR ${PYTHON_ROOT_DIR})

# CMake uses this when trying to find Python
set(Python_DIR ${PYTHON_ROOT_DIR})

set(CHECKED_FILES)
if(WIN32)
    set(PYTHON_LIBRARY python${PYTHON_SHORT_VERSION}.lib)
    list(APPEND CHECKED_FILES lib/python.${PYTHON_SHORT_VERSION}.dll)
elseif(APPLE)
    set(PYTHON_LIBRARY libpython${PYTHON_SHORT_VERSION}m.dylib)
else()
    set(PYTHON_LIBRARY libpython${PYTHON_SHORT_VERSION}.so)
endif()
list(APPEND CHECKED_FILES
    lib/${PYTHON_LIBRARY}
    lib/python${PYTHON_NUMERIC_VERSION}.zip
    )

set(PACKAGED_FILES include bin share
    lib/python${PYTHON_SHORT_VERSION}/lib-dynload
    ${CHECKED_FILES}
    )


if(USE_PREBUILT_PYTHON_PACKAGE)

    # Retrieve the plugin's file(s)

    if(WIN32)
        if(RELEASE_MODE)
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
                Python ${PYTHON_VERSION} 3351428b9aef566ce5ba3441ad5d7b5256e63280
                CHECKED_FILES ${CHECKED_FILES}
                SHA1_VALUES 710a0520607377a1429933cd91399a3200f45314
                    b9895617dc6c321eaaa7d5eef7e4a922f6fb226a
                    a18cbb0332fcef7ce3f3a73fbeb9b463a69632f7
                )
        else()
            RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
        endif()
    elseif(APPLE)
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
            Python ${PYTHON_VERSION} cee991e70e3ac8d7cf69a91be922cdc4ebf0e99d
            CHECKED_FILES ${CHECKED_FILES}
            SHA1_VALUES a0ac06cdfd7468742994f29d333c7bfe4a53216a
                3f0b02b632937280a684362aae9dbc7d90f1dd69
                )
    else()
        RETRIEVE_PACKAGE_FILE(${RELATIVE_ROOT_DIR} Sample sha...)
    endif()

else()
    # Build and install Python as an external package

    include(cmake/BuildPython.cmake)

    Build_Python(${PYTHON_ROOT_DIR} FALSE)

    # Precompile the Python sources in the library
    # and package everything as a ZIP file

    execute_process(COMMAND ${PYTHON_ROOT_DIR}/bin/python
                                ${PROJECT_SOURCE_DIR}/bin/ziplibrary.py
                                ${PYTHON_ROOT_DIR}/lib/python${PYTHON_NUMERIC_VERSION}.zip
                                ${PYTHON_ROOT_DIR}/lib/python${PYTHON_SHORT_VERSION}
                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     OUTPUT_QUIET RESULT_VARIABLE EXECUTE_RESULT
                    )
    if(EXECUTE_RESULT)
        message(FATAL_ERROR "Unable to precompile and package Python library: ${EXECUTE_RESULT}")
    endif()


    if(APPLE)
        # Ensure Python library has a relative ID

##        find_package(Python ${PYTHON_VERSION} EXACT REQUIRED
##                     PATHS ${PYTHON_ROOT_DIR} NO_DEFAULT_PATH)
##
##        if("${CMAKE_BUILD_TYPE}" STREQUAL "")
##            set(BUILD_TYPE "RELEASE")
##        else()
##            string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
##        endif()
##        get_property(PYTHON_LIBRARY_PATH TARGET ${PYTHON_LIBRARIES}
##                                         PROPERTY IMPORTED_SONAME_${BUILD_TYPE}
##                    )
##        get_filename_component(PYTHON_LIBRARY ${PYTHON_LIBRARY_PATH} NAME)

        execute_process(COMMAND install_name_tool -id @rpath/${PYTHON_LIBRARY}
                                                      ${PYTHON_ROOT_DIR}/lib/${PYTHON_LIBRARY})
    endif()

    ## We need to put everything into the application bundle and set PYTHONHOME

    # $ export PYTHONHOME=/Users/dave/build/OpenCOR/src/plugins/thirdParty/Python/Python/osx/
    # $ ./OpenCOR.app/Contents/MacOS/OpenCOR -c Python::shell

    CREATE_PACKAGE_FILE(${RELATIVE_ROOT_DIR}
        Python ${PYTHON_VERSION}
        PACKAGED_FILES ${PACKAGED_FILES}
        CHECKED_FILES ${CHECKED_FILES}
        )
endif()

# FIND_PACKAGE should now succeed. Settings will be exported
# via CMake's cache.

find_package(Python ${PYTHON_VERSION} EXACT REQUIRED
             PATHS ${PYTHON_ROOT_DIR} NO_DEFAULT_PATH
             QUIET)

# A sanity check, just in case CMake has found another Python...

if(NOT "${PYTHON_INCLUDE_DIR}" STREQUAL "${PYTHON_ROOT_DIR}/include/python${PYTHON_SHORT_VERSION}")
    message(FATAL_ERROR "Couldn't find our Python package...")
endif()

set(EXTERNAL_BINARY_DIR ${PYTHON_ROOT_DIR}/lib)

if(WIN32)
    set(EXTERNAL_BINARIES)
    foreach(LIBRARY ${PYTHON_LIBRARIES})
        string(REPLACE "${EXTERNAL_BINARY_DIR}/" "" LIBNAME ${LIBRARY})
        string(REGEX REPLACE "${CMAKE_IMPORT_LIBRARY_SUFFIX}$" "${CMAKE_SHARED_LIBRARY_SUFFIX}"
            EXTERNAL_BINARY ${LIBNAME})
        list(APPEND EXTERNAL_BINARIES ${EXTERNAL_BINARY})
    endforeach()
else()
    set(EXTERNAL_BINARIES ${PYTHON_LIBRARY})
endif()

# Ensure external packages build by other plugins can find us using CMake

set(PYTHON_ROOT_DIR ${PYTHON_ROOT_DIR} PARENT_SCOPE)
set(Python_DIR ${Python_DIR} PARENT_SCOPE)


# Add the plugin

ADD_PLUGIN(Python
    SOURCES
        ../../plugininfo.cpp

        src/pythonplugin.cpp
    HEADERS_MOC
        src/pythonplugin.h
    INCLUDE_DIRS
        src
        ${PYTHON_INCLUDE_DIRS}
    PLUGINS
        Core
    DEFINITIONS
        QT_NO_KEYWORDS
    EXTERNAL_BINARIES
        ${EXTERNAL_BINARIES}
    EXTERNAL_BINARIES_DIR
        ${EXTERNAL_BINARY_DIR}
)
