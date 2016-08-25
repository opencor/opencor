PROJECT(PythonPlugin)

# Build Python
set(PYTHON_VERSION 3.5.2)

set(INSTALL_MANUAL OFF)
set(INSTALL_TEST OFF)
set(BUILD_LIBPYTHON_SHARED OFF)
set(BUILD_EXTENSIONS_AS_BUILTIN ON)


#USE_LIB64

#option(USE_SYSTEM_LIBRARIES "Use system libraries" ON)
#SET(USE_SYSTEM_ZLIB OFF)
#SET(ZLIB_INCLUDE_DIR...)


# Remove when common.cmake is fixed...
remove_definitions(-DUNICODE)

# Only if we need to actually build the library...
add_subdirectory(cmake Python)

#SET(PYTHON_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Python-${PYTHON_VERSION}/Include)
#SET(PYTHON_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Python/lib)
##libpython3.5m.a)

SET(CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Python)
SET(PYTHON_USE_STATIC_LIB ON)
SET(PYTHON_TARGETS_IMPORTED ON)
FIND_PACKAGE(Python ${PYTHON_VERSION} EXACT REQUIRED)

message("INC: ${PYTHON_INCLUDE_DIRS}")
message("LIB: ${PYTHON_LIBRARIES}")


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
)
