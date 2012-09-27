PROJECT(ComputerPlugin)

# Retrieve some LLVM settings

RETRIEVE_LLVM_SETTINGS()

# Add the plugin

ADD_PLUGIN(Computer
    SOURCES
        ../../i18ninterface.cpp
        ../../interface.cpp
        ../../plugininfo.cpp

        src/computerengine.cpp
        src/computermath.cpp
        src/computerplugin.cpp
    HEADERS_MOC
        src/computerengine.h
        src/computerplugin.h
    INCLUDE_DIRS
        src
    DEFINITIONS
        ${LLVM_DEFINITIONS}
    OPENCOR_DEPENDENCIES
        ${LLVM_PLUGIN}
    OPENCOR_BINARY_DEPENDENCIES
        ${LLVM_BINARY_PLUGIN}
    QT_DEPENDENCIES
        QtCore
        QtGui
    TESTS
        test
)
