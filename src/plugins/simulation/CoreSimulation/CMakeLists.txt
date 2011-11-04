PROJECT(CoreSimulationPlugin)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_FILES_DIR}/common.cmake)

# Add the plugin

ADD_PLUGIN(CoreSimulation
    SOURCES
        ../../coreinterface.cpp
        ../../guiinterface.cpp
        ../../plugininfo.cpp

        src/coresimulationplugin.cpp
    HEADERS
        ../../coreinterface.h
        ../../guiinterface.h
        ../../plugininfo.h
    HEADERS_MOC
        src/coresimulationplugin.h
    DEFINITIONS
        __STDC_LIMIT_MACROS      # These two definitions
        __STDC_CONSTANT_MACROS   # are for LLVM
    INCLUDE_DIRS
        ../../3rdparty/Qwt/src

        src
    OPENCOR_DEPENDENCIES
        Core
        LLVM
        Qwt
    QT_DEPENDENCIES
        QtCore
        QtGui
        QtSvg
        QtXml
)
