PROJECT(LLVMPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(LLVM
    SOURCES
        ../../plugininfo.cpp

        src/llvmplugin.cpp
    HEADERS
        ../../plugininfo.h
    HEADERS_MOC
        src/llvmplugin.h
    PLUGIN_INCLUDE_DIRS
        src
    QT_DEPENDENCIES
        QtCore
)
