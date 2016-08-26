PROJECT(PythonPlugin)

## Build Python
set(USE_PREBUILT_PYTHON ON)

set(PYTHON_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Python)

if(USE_PREBUILT_PYTHON)
    # Download and untar into PYTHON_ROOT_DIR
else()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
    execute_process(COMMAND cmake -DCMAKE_INSTALL_PREFIX=${PYTHON_ROOT_DIR}
                                  -G Ninja ${CMAKE_CURRENT_SOURCE_DIR}/build
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build
                    RESULT_VARIABLE PYTHON_CONFIGURE
                    )
    if(PYTHON_CONFIGURE)
        message(FATAL_ERROR "Unable to configure Python: ${PYTHON_CONFIGURE}")
    endif()
    execute_process(COMMAND ninja install
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build
                    RESULT_VARIABLE PYTHON_BUILD
                    )
    if(PYTHON_BUILD)
        message(FATAL_ERROR "Unable to build and install Python: ${PYTHON_BUILD}")
    endif()
endif()

set(DEFINITIONS QT_NO_KEYWORDS)
set(PYTHON_DEFINITIONS ${DEFINITIONS} PARENT_SCOPE)

set(PYTHON_USE_STATIC_LIB ON)
find_package(Python EXACT 3.5.2 REQUIRED
                    NO_DEFAULT_PATH
                    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Python
           )
get_property(PYTHON_LIBRARY TARGET ${PYTHON_LIBRARIES}
                            PROPERTY IMPORTED_LOCATION_RELEASE
            )
set(PYTHON_PLUGIN_BINARY ${PYTHON_LIBRARY} PARENT_SCOPE)

message("PYTHON LIBRARY: ${PYTHON_LIBRARY}")
message("PYTHON INCLUDES: ${PYTHON_INCLUDE_DIRS}")

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
