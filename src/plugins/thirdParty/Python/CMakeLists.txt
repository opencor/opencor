PROJECT(PythonPlugin)

## Build Python
set(USE_PREBUILT_PYTHON ON)

SET(PYTHON_VERSION 3.5.2)

set(PYTHON_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Python)

if(USE_PREBUILT_PYTHON)
    # Download and untar into PYTHON_ROOT_DIR

else()
    message(STATUS "Configuring Python...")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
    execute_process(COMMAND ${CMAKE_COMMAND}
                            -G ${CMAKE_GENERATOR}
                            -DPYTHON_VERSION=${PYTHON_VERSION}
                            -DCMAKE_INSTALL_PREFIX=${PYTHON_ROOT_DIR}
                            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
                            ${CMAKE_CURRENT_SOURCE_DIR}/build
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build
                    RESULT_VARIABLE PYTHON_CONFIGURE
                    OUTPUT_QUIET
                    )
    if(PYTHON_CONFIGURE)
        message(FATAL_ERROR "Unable to configure Python: ${PYTHON_CONFIGURE}")
    endif()

    message(STATUS "Building and installing Python...")
    execute_process(COMMAND ${CMAKE_COMMAND}
                            --build ${CMAKE_CURRENT_BINARY_DIR}/build
                            --clean-first
                            -- install
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build
                    RESULT_VARIABLE PYTHON_BUILD
                    OUTPUT_QUIET
                    )
    if(PYTHON_BUILD)
        message(FATAL_ERROR "Unable to build and install Python: ${PYTHON_BUILD}")
    endif()
endif()

set(DEFINITIONS QT_NO_KEYWORDS)
set(PYTHON_DEFINITIONS ${DEFINITIONS} PARENT_SCOPE)

set(PYTHON_USE_STATIC_LIB ON)
find_package(Python EXACT ${PYTHON_VERSION}
                    REQUIRED
                    NO_DEFAULT_PATH
                    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Python
           )
get_property(PYTHON_LIBRARY TARGET ${PYTHON_LIBRARIES}
                            PROPERTY IMPORTED_LOCATION_RELEASE
            )
set(PYTHON_PLUGIN_BINARY ${PYTHON_LIBRARY} PARENT_SCOPE)

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
        ${ZLIB_PLUGIN}
    PLUGIN_BINARIES
        ${ZLIB_PLUGIN_BINARY}
        ${PYTHON_LIBRARY}
    DEFINITIONS
        ${DEFINITIONS}
)
